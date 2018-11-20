#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h> 

int main() 
{ 
    int shmid; 
    int pid; 
 
    char *shared_txt = (char*)malloc(sizeof(char)*15); 
    void *shared_memory = (void *)0; 
 
 
    // 공유메모리 공간을 만든다. 
    // 인자 1
    // 키 1234는 공유 메모리 생성 시 필요한 임의 값.
    // 나중에 이 키를 통해서 공유 메모리에 접근한다.
    // 인자 2
    // sizeof(int)는 이 메모리의 최소 사이즈
    // 인자 3
    // IPC_CREAT는 생성하려는 키값의 공유 메모리가 없을 경우 새로 생성.
    shmid = shmget((key_t)1234, sizeof(char), 0666|IPC_CREAT); 
 
    if (shmid == -1) 
    { 
        perror("shmget failed : "); 
        exit(0); 
    } 
 
    // 공유메모리를 사용하기 위해 프로세스 메모리에 붙인다.  
    // 인자 1
    // 아까 생성한 공유 메모리 id값.
    // 인자 2
    // 메모리가 붙을 주소 설정
    // 0 = 커널이 알아서 설정함
    // 인자 3
    // 읽기/쓰기 모드 설정, SHM_RDONLY 는 읽기전용
    // 0 = 읽기/쓰기 모두 가능
    shared_memory = shmat(shmid, (void *)0, 0); 
    if (shared_memory == (void *)-1) 
    { 
        perror("shmat failed : "); 
        exit(0); 
    } 
 
    // 공유 메모리의 주소를 char로 캐스팅해서 사용.
    shared_txt = (char *)shared_memory; 
    pid = fork(); 
    // 자식 프로세스
    if (pid == 0) 
    { 
        // 아까 생성한 메모리 주소 불러옴
        shmid = shmget((key_t)1234, sizeof(char), 0); 
        if (shmid == -1) 
        { 
            perror("shmget failed : "); 
            exit(0); 
        } 
        shared_memory = shmat(shmid, (void *)0, 0666|IPC_CREAT); 
        if (shared_memory == (void *)-1) 
        { 
            perror("shmat failed : "); 
            exit(0); 
        } 
        shared_txt  = (char *)shared_memory; 
        
        FILE *f;
        f = fopen("ori.txt", "r");
        fscanf(f, "%[^\n]s", shared_txt);
        printf("자식 프로세스에서 ori.txt 파일 읽음.\n");
        fclose(f);
    } 
 
    // 부모 프로세스로 공유메모리의 내용을 보여준다.  
    else if(pid > 0) 
    { 
        FILE *f;
        f = fopen("new.txt", "w");
        fprintf(f, "%s", shared_txt);
        printf("부모 프로세스에서 new.txt 파일로 복사함.\n");
        fclose(f);
    } 
} 
