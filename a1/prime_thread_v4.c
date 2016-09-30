#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS 8
#define LIMIT 10000000

struct prime_para{
	int id;
	int start;
	unsigned long sum;
	int count;
	unsigned long last_ten[10];
}pr[8];

void merge_sort(long[],long[],long[]);
int sort_array(long[],int);

void *FindPrime(void* arg)
{
	struct prime_para *fp = (struct prime_para*) arg;
	int flag = 1;
	int t=0;
	
	long n=(long) fp[0].start;
	while(n<=LIMIT){
		for(int j=2; j <= sqrt(n); j++){
			if(n%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
			fp[0].count+=1;
			fp[0].sum+=n;
			fp[0].last_ten[t]=n;
			t++;
			if(t==10)t=0;
		}
		n = n+20;
		flag = 1;
	}
}

int main(int argc, char* argv[])
{	clock_t start, end;
	double cpu_time_used;
	unsigned long sum = 6;
	int count=1;
	long all_last_ten[80];

	start = clock();
	
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	
	for(int i=0;i<8;i++)
	{
		pr[i].count=0;
		pr[i].id=i;
		pr[i].sum=0;
	}
	pr[0].start = (1);
	pr[1].start=(3);
	pr[2].start=(7);
	pr[3].start=(9);
	pr[4].start=(11);
	pr[5].start=(13);
	pr[6].start=(17);
	pr[7].start=(19);

	long n=11;
	int rc;
	void* status;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(int t=0; t<NUM_THREADS; t++){
			rc = pthread_create(&thread[pr[t].id], &attr, FindPrime, &pr[t]);
		if(rc){
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
	}
	
	pthread_attr_destroy(&attr);
	for(int t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t], &status);
		if(rc){
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	
	for(int i=0;i<NUM_THREADS;i++){
		sum+= pr[i].sum;
	}

	for(int i=0; i<NUM_THREADS; i++){
		count+=pr[i].count;
	}
	
	printf("Sum is %lu\nCount is %d\n",sum,count);
	long temp[20];
	int t=0;
	for(int i=0;i<NUM_THREADS;i++) sort_array(pr[i].last_ten,10);
	
	for(int i=0;i<NUM_THREADS;i+=2){
		merge_sort(pr[i].last_ten, pr[i+1].last_ten,temp);
		for(int j=0;j<20;j++){
			all_last_ten[t+j]=temp[j];
		}
		t+=20;
	}
	
	sort_array(all_last_ten,80);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Execution Time = %f\n",cpu_time_used);

	FILE *fp;
	fp = fopen("/home/nitish/workspace/c/primes.txt", "w");
	fprintf(fp, "Execution Time: %lf\n", cpu_time_used);
	fprintf(fp, "Total number of primes found: %d\n", count);
	fprintf(fp, "Sum of all primes found: %lu\n", sum);
	fprintf(fp, "Top Ten maximum primes:\n");
	for(int i=70;i<80;i++){
		fprintf(fp, "%lu\n",all_last_ten[i]);
	}
	fclose(fp);
	
	pthread_exit(NULL);
}

int sort_array(long array[],int n)
{
  int c, d, swap;
 
  for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
  return 0;
}

void merge_sort(long array1[], long array2[], long array3[])
{
	int m=10, n=10;
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < m && j < n)
    {
        if (array1[i] < array2[j])
        {
            array3[k] = array1[i];
            i++;
        }
        else
        {
            array3[k] = array2[j];
            j++;
        }
        k++;
    }
    if (i >= m)
    {
        while (j < n)
        {
            array3[k] = array2[j];
            j++;
            k++;
        }
    }
    if (j >= n)
    {
        while (i < m)
        {
            array3[k] = array1[i];
            i++;
            k++;
        }
    }
}
