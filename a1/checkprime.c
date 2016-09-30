#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(){
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	long count = 0;
	int flag = 1;
	unsigned long long sum = 0;
	for(int i=2; i<=1000000;i++){
		for(int j=2; j <= (i/2); j++){
			if(i%j == 0){
				flag=0;
				break;
			}	
		}
		if(flag==1){
//			printf("%d is a Prime Number\n", i);
			//count[tid]++;
//			sum+= i;
			count++;
//			fprintf(fp, "%ld\n", n);
		}else{
//			printf("%d is not a Prime Number\n", i);
		}
		flag = 1;
	}
	printf("Sum= %llu\nCount= %ld\n", sum, count);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Execution Time = %f\n",cpu_time_used);
}
