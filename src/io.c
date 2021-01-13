#include <stdio.h>
#include <string.h>

void Print(char* str, FILE* fp) { fprintf(fp, "%s\n", str); }
void Flush() { fflush(stdout); }