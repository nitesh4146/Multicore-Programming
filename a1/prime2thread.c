#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define  THREADS 2
int limit = 100000;
pthread_t thread[THREADS];

void *FindPrime(void* t)
{
	int tid = (int*) t;
	int start1,end1;
	start1 = tid*(limit/THREADS);
	end1 = start1 + (limit/THREADS);
	
	long count = 0;
	int flag = 1;
	unsigned long long sum = 0;
	for(int i=start1; i<end1;i++){
		for(int j=2; j <= i/2; j++){
			if(i%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){

			sum+= i;
			count++;
		}
		
		flag = 1;
	}
	printf("Sum= %llu\nCount= %ld\n", sum, count);
	
}

int	main (int argn,char **argv)
{
		clock_t start, end;
	double cpu_time_used;

	start = clock();
	int i;
	
	for (i = 0; i < THREADS; i++)
		pthread_create (&thread[i], NULL, FindPrime, (void*) i);
	for (i = 0; i < THREADS; i++)
		pthread_join (thread[i], NULL);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Execution Time = %f\n",cpu_time_used);
	return 0;
}
