#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS 1

void *FindPrime(void* t)
{
	long tid = (long)t;
	long n = (long)t;
	int flag = 1;
	printf("Welcome to thread %ld\n", tid);
	
	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	
	for(int i=1; i<=100000;i++){
		for(int j=2; j <= n/2; j++){
			if(n%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
//			printf("%ld is a Prime Number\n", n);
//			fprintf(fp, "%ld\n", n);
		} else {
//			printf("%ld is not a Prime Number\n", n);
		}
		n = n+10;
		flag = 1;
	}
	
}

int main(int argc, char* argv[])
{
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	long t;
	int rc;
	void* status;
	long n=1;
	
	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	fclose(fp);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(t=0; t<NUM_THREADS; t++){
		printf("Main: Creating thread %ld\n", t);
		rc = pthread_create(&thread[t], &attr, FindPrime, (void*) n);
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t], &status);
		if(rc){
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: Join completed with thread %ld having status of %ld\n", t, (long) status);
	}
	
	printf("Main: Program completed, Exiting.\n");
	pthread_exit(NULL);
}
