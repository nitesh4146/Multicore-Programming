#include <iostream>
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
using namespace std;

#define rdsize 1000

int main() {
    char c;
    int i=0;
    int A[rdsize];
    FILE *pFile;
    pFile= fopen("/home/nitish/Workspace/a2/random_3","r");
       /*for(int t=0; t<1000; t++)
       {
            fscanf(pFile, "%d", &A[t]);
       }*/
    if(pFile!=NULL)
       {
        while  (c != EOF )
        {
        c=fgetc(pFile);
        A[i]=atoi(&c);
        i++;
       }
           fclose(pFile);
       }
        else
        printf("File can not be opened");
       for(int x=0;x<rdsize;x++)
            printf("%d\n",A[x]);

}