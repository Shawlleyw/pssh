#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../src/defines.h"
#include "../src/io.h"
#include<readline/readline.h>
#include<readline/history.h>
const char * DELIM = "/";
void Get(char * str){
    char ** argv = (char **)malloc(10* sizeof(char *));
    int cnt = 0;
    for(char * next = strtok(str, DELIM); next != NULL; next = strtok(NULL, DELIM))
        argv[cnt++]= next;
    printf("%d\n",cnt);
    for(int i = 0;i < cnt; i++)
        printf("%s\n", argv[i]);
    free(argv);argv = NULL;
}
int main(){
    //printf("%d ", );
    //strcpy(str2,str1);
    char * str4 = (char *)malloc(100*sizeof(char));
    scanf("%s",str4);
    printf("%p\n", str4);
    Get(str4);
    //free(str4);str4 = NULL;
    return 0;
}