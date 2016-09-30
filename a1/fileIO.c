#include <stdio.h>

int main() {
   
   FILE *fp;
   char buff[255];
   fp = fopen("/home/nitish/workspace/c/test.txt", "a+");
   fprintf(fp, "This is testing for fprintf...Nitish\n");
   fputs("This is testing for fputs...\n", fp);
   //fclose(fp);
 //  fp = fopen("/home/nitish/workspace/c/test.txt", "r+");
   
   fgets(buff, 10, fp);
   printf("%s\n", buff);
   fclose(fp);


/*
   FILE *fp;
   char buff[255];

   fp = fopen("/home/nitish/workspace/c/test.txt", "r");
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );

   fgets(buff, 255, (FILE*)fp);
   printf("2: %s\n", buff );
   
   fgets(buff, 255, (FILE*)fp);
   printf("3: %s\n", buff );
   fclose(fp);
*/
}
