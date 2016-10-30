/* Queue - Bounded Blocking Queue implementation in C/C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std; 
#define MAX_SIZE 10  //maximum size of the array that will store Queue. 
#define NUM_THREADS 1	//Total number of threads

int _random[MAX_SIZE];	//Pre-generated array of ramdom numbers
int A[MAX_SIZE];	//Queue object
int index = 0;
int total_cals = 0;	//total number of enq's and deq's
int test_bench;

pthread_mutex_t Elock;
pthread_mutex_t Dlock;

void *PrintHello(void*);
bool LockEnQ();
bool LockDeQ();
void UnlockEnQ();
void UnlockDeQ();


// Creating a class named Queue.
class Blocking_Queue
{
private:
	int head, tail; 
public:
	// Constructor - set head and tail as -1. 
	// We are assuming that for an empty Queue, both head and tail will be -1.
	Blocking_Queue()
	{
		head = -1; 
		tail = -1;
	}

	// Inserts an element in queue at tail end
	void enq(int x)
	{
		while(LockEnQ()!=0){};
			if((tail+1)%MAX_SIZE == head ? true : false)
			{
				//cout<<"Error: Queue is Full!"<<endl;
				UnlockEnQ();
				//this->Print();
				return;
			}
			if ((head == -1 && tail == -1))
			{ 
				head = tail = 0; 
			}
			else
			{
			    tail = (tail+1)%MAX_SIZE;
			}
		printf("Enq: %d, Head: %d, Tail: %d\n", x, head, tail);
		A[tail] = x;
		
		UnlockEnQ();
	}

	// Removes an element in Queue from head end. 
	void deq()
	{
		while(LockDeQ()!= 0){};
		//printf("Dequeuing... \n");
		if((head == -1 && tail == -1))
		{
			cout<<"Deq: Empty!, Head: "<<head<<", Tail: "<<tail<<endl;
			UnlockDeQ();
			return;
		}
		else if(head == tail ) 
		{
			tail = head = -1;
			printf("Dequeued last element!\n");
			UnlockDeQ();
		}
		else
		{
			int a = A[head];
			head = (head+1)%MAX_SIZE;
			printf("Deq: %d, Head: %d, Tail: %d\n", a, head, tail);
			//printf("deq: %d\n", a);
			UnlockDeQ();
		}
	}
	
	/* Printing the elements in queue from head to tail. */
	void Print()
	{
		// Finding number of elements in queue  
		int count = (tail+MAX_SIZE-head)%MAX_SIZE + 1;
		printf("Queue       : ");
		if(tail==-1 && head==-1){
			printf("Null");
		}else{
			for(int i = 0; i <count; i++)
			{
				int index = (head+i) % MAX_SIZE; // Index of element while travesing circularly from head
				printf("%d ", A[index]);
			}
		}
		printf("\n\n");
	}
}q;



void *threadFn(void *threadid)
{
	long tid;
	tid = (long)threadid;
	int val = (int) tid;
	//printf("Welcome to thread %ld\n", tid);

/*Test Bench (1)*/
	if(test_bench==1){
    	while(total_cals < MAX_SIZE)
    	{
			if (_random[index+tid] == 0)
        	{
            	q.enq(index);
	            index++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }else if (_random[index+tid] == 1)
	        {
    	        q.deq();    
        	    index++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }
/*Test Bench (2)*/
    else if (test_bench==2){
    	while(total_cals < MAX_SIZE)
    	{
    		if (_random[index+tid] == 0 || _random[index+tid]== 1)
        	{
            	q.enq(index);
	            index++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }
        	else if (_random[index+tid] == 2)
	        {
    	        q.deq();    
        	    index++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }
/*Test Bench (3)*/
    else{    
    	while (total_cals < (MAX_SIZE))
    	{
        	if (_random[index+tid] == 0)
        	{
            	q.enq(index);
	            index++ ;
    	        total_cals++;
        	    // printf("Total Calls: %d\n", total_cals);
	        }
        	else  if (_random[index+tid] == 1 || _random[index+tid]== 2)
	        {
    	        q.deq();    
        	    index++;
            	total_cals++;
            	// printf("Total Calls: %d\n", total_cals);
	        }
    	}
    }

	pthread_exit(NULL);

}

/* Functions to acquire and release locks */
bool LockEnQ(){
	bool r = pthread_mutex_lock(&Elock);
	return r;
}

bool LockDeQ(){
	bool r = pthread_mutex_lock(&Dlock);
	return r;
}

void UnlockEnQ(){
	pthread_mutex_unlock(&Elock);
}

void UnlockDeQ(){
	pthread_mutex_unlock(&Dlock);
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

    //q.Print();

    return 0;
}