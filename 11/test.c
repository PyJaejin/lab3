#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

int getargs(char*, char**);
void handler_C(int);
void handler_Z(int);

int main() {
    char buf[1024];
    char *argv[50];
    int narg;
    int status, fd;
    pid_t pid;
	DIR *pdir;
	struct dirent *pde;
	int i = 0;
	//ln
    char cmd;
    char *src;
    char *target;
    char ch;
    FILE* fp;
    int cp_src, cp_dst;
    int r_size, w_size;
    //dir
    char dir_name[50];
    char dir_path[50];
    struct sigaction act_C;
    struct sigaction act_Z;

    act_C.sa_handler = handler_C;
    sigfillset(&(act_C.sa_mask));
    sigaction(SIGINT, &act_C, NULL);
 
    act_Z.sa_handler = handler_Z;
    sigfillset(&(act_Z.sa_mask));
    sigaction(SIGQUIT, &act_Z, NULL);
    

    while(1) {
        printf("shell> ");
        gets(buf);
        pid = fork();
        narg = getargs(buf, argv);

        if(pid == 0) {
            if(!strcmp(argv[0], "exit")) {
                exit(0);
                return 0;
            }

            else if(!strcmp(argv[0], "ls")) {
                pdir = opendir(getcwd(buf, 1024));
                printf("ls실행\n");
                while ((pde = readdir(pdir)) != NULL) {
			        printf("%s  ", pde->d_name);
                }
		        printf("\n");
		        closedir(pdir);
            }
        
            else if(!strcmp(argv[0], "pwd")) {
                printf("pwd실행\n");
                getcwd(buf, 1024);
                printf("%s\n", buf);
            
            }

            else if(!strcmp(argv[0], "cd")) {
                printf("cd실행\n");
                if((chdir(argv[1])) < 0)
                    printf("잘못된 디렉토리를 입력했습니다.\n");
            }

            else if(!strcmp(argv[0], "mkdir")) {
                printf("mkdir실행\n");
                
                strcpy(dir_name, argv[1]);
                strcpy(dir_path, getcwd(buf, 1024));

                strcat(dir_path, "/");
                strcat(dir_path, dir_name);

                if(mkdir(dir_path, 0755) > 0)
                    printf("디렉토리를 생성할 수 없습니다.");
            }

            else if(!strcmp(argv[0], "rmdir")) {
                printf("rmdir실행\n");
                strcpy(dir_name, argv[1]);
                strcpy(dir_path, getcwd(buf, 1024));

                strcat(dir_path, "/");
                strcat(dir_path, dir_name);

                if(rmdir(dir_path) > 0)
                    printf("디렉토리를 삭제할 수 없습니다.");
            }
            
            else if(!strcmp(argv[0], "ln")) {
                printf("ln실행\n");
                cmd = (char) *argv[1];
                printf("cmd : %c\n", cmd);
                if (cmd == '|' ) {
                    if (narg < 4) {
                        fprintf(stderr, "file_link l src target [link]\n");
                        exit(1);
                    }
                    src = argv[2];
                    target = argv[3];
                    
                    if (link(src, target) < 0) {
                        perror("link");
                        exit(1);
                    }
                }
                else if (cmd == 's') {
                    if (narg < 4) {
                        fprintf(stderr, "file_link l src target [link]\n"); 
                        exit(1);
                    }
                    src = argv[2];
                    target = argv[3];
                    
                    if (symlink(src, target) < 0) {
                        perror("symlink");
                        exit(1);
                    }
                }
                else if (cmd == 'u') {
                    src = argv[2];
                    if (unlink(src) < 0) {
                        perror("unlink");
                        exit(1);
                    }
                } else {
                fprintf(stderr, "Unknown command...\n");
                }
            }

            else if(!strcmp(argv[0], "cp")) {
                printf("strcpy실행\n");
                cp_src = open(argv[1], O_RDONLY);
                cp_dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                while(read(cp_src, &ch, 1))
                    write(cp_dst, &ch, 1);
                close(cp_src);
                close(cp_dst);
            }

            else if(!strcmp(argv[0], "rm")) {
                unlink(argv[1]);
            }

            else if(!strcmp(argv[0], "mv")) {
                char file_name[50];
                strcpy(file_name, argv[1]);
                cp_src = open(argv[1], O_RDONLY);
                cp_dst = open(argv[2], O_RDWR|O_CREAT|O_EXCL, 0664);
                r_size = read(cp_src,buf, 1024);
                w_size = write(cp_dst,buf,r_size);
                
                while(r_size == 1024) {
                    r_size = read(cp_src,buf,1024);
                    w_size = write(cp_dst,buf,r_size);
                }

                unlink(file_name);
                close(cp_src);
                close(cp_dst);
            }
            else if(!strcmp(argv[0], "cat")) {
                cp_src = open(argv[1], O_RDONLY);
                while(read(cp_src, &ch, 1))
                    write(1, &ch, 1);
                close(cp_src);
            }

            else if(!strcmp(argv[narg-2], "<")) {
                if( (fd = open(argv[1], O_RDONLY | O_CREAT, 0644)) < 0)
                        dup2(fd, STDIN_FILENO);
                close(fd);
            }
           
            if(!strcmp(argv[narg-2], ">")) {
                for(i=0; argv[i] != NULL; i++) {
                    if(!strcmp(argv[i], ">")) {
                        break;
                    }
                }
                fp = fopen(argv[i-1], "r");
                fscanf(fp, "%[^\n]s", buf);
                fclose(fp);
                fp = fopen(argv[i+1], "w");
                fprintf(fp, "%s\n", buf);
                fclose(fp);
            }
            
            else if(!strcmp(argv[narg-2], ">>")) {
                for(i=0; argv[i] != NULL; i++) {
                    if(!strcmp(argv[i], ">>")) {
                        break;
                    }
                }
                fp = fopen(argv[i-1], "r");
                fscanf(fp, "%[^\n]s", buf);
                fclose(fp);
                fp = fopen(argv[i+1], "a");
                fprintf(fp, "%s\n", buf);
                fclose(fp);
            }

            if(!strcmp(argv[narg-1], "&")) {
                printf("&실행\n");
            }
            else {
                exit(0); 
            }
        }
        else if(pid > 0) {
            wait((int*)0);
        }
        else
            perror("fork failed");
    }
}

int getargs(char *cmd, char **argv) {
     int narg = 0;

     while(*cmd) {
         if (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
             *cmd++ = '\0';
         else {
             argv[narg++] = cmd++;
             while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n')
                 cmd++;
         }
     }
     argv[narg] = NULL;

     return narg;
}

void handler_C(int signo) {
    printf("handler: signo = %d\n", signo);
    printf("pressed Ctrl + C\n");
    exit(2);
}

void handler_Z(int signo) {
    printf("handler: signo = %d\n", signo);
    printf("pressed Ctrl + Z\n");
    exit(3);
}
