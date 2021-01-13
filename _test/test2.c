#include<stdio.h>
#include<stdlib.h>
const int MAXN = 100;
int main(int argc,char ** argv){
    FILE * fp = fdopen(1,"w+");
    fprintf(fp,"Yessir\n");
    return 0;
}