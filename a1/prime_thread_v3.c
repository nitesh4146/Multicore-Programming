#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS 8
long count[9] = {0};
unsigned long long sum[9] = {0};
//double last_two[16];

void *FindPrime(void* t)
{
	int tid;
	long n = (long)t;
	int flag = 1;
	long last_two[2];
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
//	printf("Welcome to thread %d, Sum = %d, Count = %d\n", tid, sum[tid],count[tid]);

//	FILE *fp;
//	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	long limit = 1000000; //(int) pow(10,7);
	long l1,l2;
	while(n<=limit){
		for(int j=2; j <= sqrt(n); j++){
			if(n%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
//			printf("%ld is a Prime Number\n", n);
			count[tid]++;
			sum[tid]+=n;
			l1 = l2;
			l2 = n;
//			long temp = last_two[tid+8];
//			last_two[tid] = temp;
//			last_two[5] = last_two[6];
//			last_two[5+8] = n;
		}
		n = n+20;
		flag = 1;
	}
//	last_two[7] = l1;
	//last_two[tid+8] = l2;
//	printf("%d : Sum = %d and Count = %d\n",tid,sum[tid],count[tid]);
//	printf("%d : Last two numbers are %ld and %ld\n",tid, last_two[tid],last_two[tid+8]);
	//fprintf(fp, "%ld\n", last_two[tid]);
	//fprintf(fp, "%ld\n", last_two[tid+7]);
//	fclose(fp);
}

int main(int argc, char* argv[])
{	clock_t start, end;
	double cpu_time_used;

	start = clock();
	
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	
	long tid = 0;
	long n=11;
	int rc;
	void* status;
	count[8] = 4;
	sum[8] = 17;
	
/*	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	fclose(fp);
*/	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(int t=0; t<2; t++){
		for(int i=0; i<2; i++){
//			printf("Main: Creating thread %ld\n", n);
			rc = pthread_create(&thread[tid], &attr, FindPrime, (void*)n);
			tid++;
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
			n = n +10;
		}
		n = n -20+2;
	}
	
	n = n+2;
	
	for(int t=0; t<2; t++){
		for(int i=0; i<2; i++){
//			printf("Main: Creating thread %ld\n", n);
			rc = pthread_create(&thread[tid], &attr, FindPrime, &n);
			tid++;
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
			n = n +10;
		}
		n = n -20+2;
	}
	
	pthread_attr_destroy(&attr);
	for(int t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t], &status);
		if(rc){
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
//		printf("Main: Join completed with thread %d having status of %ld\n", t, (long) status);
	}
	
	for(int i=0; i<8; i++){
		sum[8]+= sum[i];
	}
	for(int i=0; i<8; i++){
		count[8]+= count[i];
	}
	printf("Sum of all prime numbers is %llu and count is %ld\n", sum[8], count[8]);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Execution Time = %f\n",cpu_time_used);
	pthread_exit(NULL);
}
