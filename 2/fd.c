#include <fcntl.h>
#include <unistd.h>

int main(char* argv[]) {
    int fd;
    ssize_t nread;
    char buf[1024];

    fd = open(argv[1], O_RDONLY);

    nread = read(fd, buf, 1024);

    close(fd);
}
