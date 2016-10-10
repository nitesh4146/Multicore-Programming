/* Queue - Intel TBB Bounded Concurrent Queue implementation in C/C++*/
#include <tbb/concurrent_queue.h>
#include "tbb/tbb.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;
#define MAX_SIZE 1000000  //maximum size of the array that will store Queue. 
#define NUM_THREADS 24	//Total number of threads

int _random[MAX_SIZE];	//Pre-generated array of ramdom numbers
int A[MAX_SIZE];	//Queue object
int index1 = 0;
int total_cals = 0;	//total number of enq's and deq's
int test_bench;

tbb::concurrent_bounded_queue< int > q;

void *threadFn(void *threadid)
{
    long tid;
    tid = (long)threadid;
    int val = (int) tid;
    int i;
    //printf("Welcome to thread %ld\n", tid);

/* Test Bench (1)*/
    if(test_bench==1){
    	while(total_cals < MAX_SIZE)
    	{
			if (_random[index1+tid] == 0)
        	{
            	q.push(index1);
	            index1++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }else if (_random[index1+tid] == 1)
	        {
    	        q.try_pop(i);    
        	    index1++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }
/* Test Bench (2)*/
    else if (test_bench==2){
    	while(total_cals < MAX_SIZE)
    	{
    		if (_random[index1+tid] == 0 || _random[index1+tid]== 1)
        	{
            	q.push(index1);
	            index1++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }
        	else if (_random[index1+tid] == 2)
	        {
    	        q.try_pop(i);    
        	    index1++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }
/* Test Bench (3)*/
    else{    
    	while (total_cals <= (MAX_SIZE))
    	{
        	if (_random[index1+tid] == 0)
        	{
            	q.push(index1);
	            index1++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }
        	else  if (_random[index1+tid] == 1 || _random[index1+tid]== 2)
	        {
    	        q.try_pop(i);    
        	    index1++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }

    pthread_exit(NULL);

}

/* Function to generate array of random number at the start of program based on test-bench*/
void random_gen(){
	int count = 0;
	if(test_bench==1){
		for(int i=0; i<MAX_SIZE-1; i++){
			_random[i] = rand()%2;
		}
	}else {
		for(int i=0; i<MAX_SIZE-1; i++){
			_random[i] = rand()%3;
		}
	}
}

int main ()
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long i;
    void* status;
    pthread_attr_init(&attr);

    //Execution time variable declarations
    unsigned long micros = 0;
    float millis = 0.0;
    clock_t start, end;

    //Select test bench to be executed
    printf("Number of Threads : %d\n", NUM_THREADS);
    printf("Select the Test Bench Ratio:\n(1) enq:deq = 1:1\n(2) enq:deq = 2:1\n(3) enq:deq = 1:2\n\nYour Selction: ");
    cin>>test_bench;

    random_gen(); // Generate random array based on test bench i.e. %2 or %3

    //Execution Time start
    start = clock();

    //Create threads
    for( i=0; i < NUM_THREADS; i++ ){
        rc = pthread_create(&threads[i], NULL, threadFn, (void*) i);
        if (rc){
            printf("ERROR: unable to create thread %d\n", rc);
            exit(-1);
        }
    }

    //Join threads
    for(int t=0; t<NUM_THREADS; t++){
        rc = pthread_join(threads[t], &status);
        if(rc){
            printf("ERROR: Return from pthread_join() is %d\n", rc);
        }
    }

    //Execution Time Stop
    end = clock();
    micros = end - start;
    millis = micros / 1000;
    printf("Execution Time: %f msec or %lu usec\n", millis, micros);

    return 0;
}