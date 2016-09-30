#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS 2
int count[8] = {0};
int sum[8] = {0};

void *FindPrime(void* t)
{
	int tid;
	long n = (long)t;
	int flag = 1;
	switch(n){
		case 11:
			tid = 0;
			break;
		case 21:
			tid = 1;
			break;
		case 13:
			tid = 2;
			break;
		case 23:
			tid = 3;
			break;
		case 17:
			tid = 4;
			break;
		case 27:
			tid = 5;
			break;
		case 19:
			tid = 6;
			break;
		case 29:
			tid = 7;
			break;
	}
	
	printf("Welcome to thread %d\n", tid);

	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	
	for(int i=1; i<100;i++){
		for(int j=2; j <= n/2; j++){
			if(n%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
			printf("%ld is a Prime Number\n", n);
			count[tid]++;
			sum[tid]+=n;
			fprintf(fp, "%ld\n", n);
		} else {
			//printf("%ld is not a Prime Number\n", n);
		}
		n = n+10;
		flag = 1;
	}
	printf("Total prime numbers found in thread %d are %d and sum is %d\n", tid, count[tid], sum[tid]);
	
}

int main(int argc, char* argv[])
{
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	
	long t = 0;
	long n=11;
	int rc;
	void* status;
	
	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	fclose(fp);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(int t=0; t<2; t++){
		for(int i=0; i<2; i++){
			printf("Main: Creating thread %ld\n", n);
			rc = pthread_create(&thread[t], &attr, FindPrime, (void*)n);
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
			n = n +10;
		}
		n = n -20+2;
	}
	
	printf("1st element is %d\n", count[7]);
	
	pthread_attr_destroy(&attr);
	for(t=0; t<2; t++){
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



