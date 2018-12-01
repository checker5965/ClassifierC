#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    FILE* fin = fopen("test.csv", "r");
    float *f = malloc(sizeof(float)*3);
    int c = 0;
    while(c!=3)
    {
        fscanf(fin, "%g%*c", &f[c]);
        printf("%g\n", f[c]);
        if(feof(fin))
            break;
        c++;
    }
    /*char* str = malloc(sizeof(char)*100);
    int i = 0;
    char* token;
    while(1)
    {
        fscanf(fin, "%s", str);
        if(feof(fin))
            break;
        
        printf("%s\n", str);
    }
    printf("Exit with error code 0.\n");*/
}