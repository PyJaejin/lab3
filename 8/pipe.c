#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int pid, n, fd[2];
    char buf[255];

    if (pipe(fd) < 0) { //pipe[0] = 읽기파이프, pipe[1] = 쓰기파이프
        perror("pipe error : ");
        exit(0);
    }

    if ((pid = fork()) < 0) { //파이프 생성
        perror("fork error : ");
        exit(0);
    }

    else if (pid == 0) { //자식 프로세스
        close(fd[0]);
        while(1) {
            memset(buf, 0x00, 255);
            sprintf(buf, "Send to parent process. from child process pid(%d)\n", getpid());
            write(fd[1], buf, strlen(buf));
            sleep(1);
        }
    }
    else { //부모 프로세스
        close(fd[1]);
        memset(buf, 0x00, 255);
        n = read(fd[0], buf, 255);
        fprintf(stderr, "%s", buf);
    }
}
