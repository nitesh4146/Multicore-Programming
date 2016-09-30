#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS 2
int count[8] = {0};
int sum[8] = {0};

struct arg_pass{
	long t;
	long num;
};

void *FindPrime(void* arguments)
{
	struct arg_pass* arg = arguments;
	long tid = arg -> t;
	long n = arg -> num;
	int flag = 1;
	printf("Welcome to thread %ld\n", tid);
/*
	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	
	for(int i=1; i<=10;i++){
		for(int j=2; j <= n/2; j++){
			if(n%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
			printf("%ld is a Prime Number\n", n);
			count[0]
//			fprintf(fp, "%ld\n", n);
		} else {
			//printf("%ld is not a Prime Number\n", n);
		}
		n = n+10;
		flag = 1;
	}
*/	
}

int main(int argc, char* argv[])
{
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	struct arg_pass args;
	
	args.t = 0;
	args.num=11;
	int rc;
	void* status;
	
	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	fclose(fp);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
//	for(int k=0; k<2; k++){
		for(int i=0; i<2; i++){
			printf("Main: Creating thread %ld\n, %ld\n", args.t, args.num);
			rc = pthread_create(&thread[args.t], &attr, FindPrime, (void*)&args);
			args.t++;
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
			args.num=args.num+10;
		}
//		args.num=args.num-20+2;
//	}
	
	printf("1st element is %d\n", count[7]);
	
	pthread_attr_destroy(&attr);
	for(args.t=0; args.t<2; args.t++){
		rc = pthread_join(thread[args.t], &status);
		if(rc){
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: Join completed with thread %ld having status of %ld\n", args.t, (long) status);
	}
	
	printf("Main: Program completed, Exiting.\n");
	pthread_exit(NULL);
}



