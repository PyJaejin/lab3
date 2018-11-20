#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>

int main(int argc, char* argv[])
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10; //메시지 큐의 크기. 들어갈 수 있는 아이템의 수
    attr.mq_msgsize = 4; //메시지의 크기. 바이트 단위로 설정
    char msg[30];    
    int stat;
    mqd_t mfd;

    mfd = mq_open("/lab3_test", O_WRONLY, 0666, &attr);
    if(mfd == -1)
    {
        perror("open error");
        exit(0);
    }

    printf("write your message : ");
    scanf("%[^\n]s", msg);
    int len;
    len = strlen(msg);
    msg[len] = '\0';

    if((mq_send(mfd, msg, attr.mq_msgsize, 1)) == -1) {
        perror("send error");
        exit(-1);
    }
    printf("채팅을 종료합니다.\n");
}
