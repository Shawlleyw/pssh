#include<time.h>
#include<dirent.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>

int main(){
    int fd = open("a.txt", O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC );
    int save = dup(1);
    dup2(fd, 1);
    int n;
    scanf("%d",&n);
    time_t tt;
    time(&tt);
    char * buf = ctime(&tt);
    //write(fd, 4 +  buf, 12);
    write(fd, buf + 4, 20);
    dup2(save,1);
    close(fd);
    return 0;
}