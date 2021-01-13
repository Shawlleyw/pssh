#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
int main(){
    int fd_old = dup(1);
    //printf("%d\n", fd_old);
    int fd = open("a.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if(fd == -1){
        fprintf(stderr, "open error : %s\n", strerror(errno));
        return 0;
    }
    if(dup2(fd, 1) == -1){
        fprintf(stderr, "dup2 error : %s\n", strerror(errno));
        return 0;
    }
    printf("test\n");
    fflush(stdout);
    dup2(fd_old, 1);
    
    return 0;
}