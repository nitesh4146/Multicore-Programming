#include<stdio.h>
 
int main()
{
   int i = 3, count, c;
   unsigned long n;
   printf("Enter the number of prime numbers required\n");
   scanf("%lu",&n);
 
   if ( n >= 1 )
   {
      printf("First %lu prime numbers are :\n",n);
      printf("2\n");
   }
 
   for ( count = 2 ; count <= n ;  )
   {
      for ( c = 2 ; c <= i - 1 ; c++ )
      {
         if ( i%c == 0 )
            break;
      }
      if ( c == i )
      {
         count++;
      }
      i++;
   }
   printf("%d\n",count);
 
   return 0;
}
