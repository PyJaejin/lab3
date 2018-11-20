#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int getargs(char *cmd, char *argv[]) {
    int argc = 0;
    
    while(*cmd) {
        if (*cmd == ' ' || *cmd == '\t' || *cmd == '\n') 
            *cmd++ = '\0';
        else {
            argv[argc++] = cmd++;
            while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n')
                cmd++;
        }
    }
    argv[argc] = NULL;
    
    return argc;
}

void my_exit(pid_t pid){                
	kill(pid, SIGINT);	
}

char* my_pwd(){
	char path[255];
	getcwd(path, 255);
    printf("%s Helo", path);

    return path;
}

/*
   void my_mkdir(){
    char dir = my_pwd();
}
*/



int main() {
    char buf[256];
    char *argv[50];
    char str[255];
    int argc;
    
    pid_t pid;
    while(1) {
        printf("현아하영재진유래shell> ");
        fgets(buf, sizeof(buf), stdin);
        argc = getargs(buf, argv);
        pid = fork();

        if(pid == 0) {

            //pwd
            if(strcmp(argv[0], "pwd")==0) {
                printf("%s", my_pwd());
		        strcpy(str, my_pwd());
                printf("%s\n", str);
            }
            
            //exit
            else if(strcmp(argv[0], "exit")==0) {
            	my_exit(pid);
	        }

            //ls
            else if(strcmp(argv[0], "ls")==0) {
                printf("ls 실행\n");
            }

            //mkdir
            /*
            else if(strcmp(argv[0], "mkdir")==0) {
                my_mkdir();
            }
            */

            //cd
            else if(strcmp(argv[0], "cd")==0) {
                printf("cd 실행\n");
            }
        }
        else if(pid > 0)
            wait((int*) 0);
        else
            perror("fork failed");
    }
}
