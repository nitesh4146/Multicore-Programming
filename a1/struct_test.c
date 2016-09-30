#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

struct timepass{
	int a;
	int b;
}t1[8];

int add(void* arg){
	struct timepass *tadd = (struct timepass*) arg;
	printf("Sum = %d\n", tadd[1].a + tadd[1].b);
}

int main(){
	struct timepass *tmain = t1;
	tmain[1].a = 5;
	tmain[1].b = 5;
	add(&tmain[1]);
	printf("a=%d\n",tmain[1].a);
	return 0;
}

