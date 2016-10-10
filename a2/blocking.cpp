/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std; 
#define MAX_SIZE 1000  //maximum size of the array that will store Queue. 
#define NUM_THREADS 32

int _random[1000];
int A[MAX_SIZE];

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

	// To check wheter Queue is empty or not
	bool IsEmpty()
	{
		return (head == -1 && tail == -1); 
	}

	// To check whether Queue is full or not
	bool IsFull()
	{
		return (tail+1)%MAX_SIZE == head ? true : false;
	}

	// Inserts an element in queue at tail end
	void enq(int x)
	{
		while(LockEnQ()!=0){};
			if(IsFull())
			{
				cout<<"Error: Queue is Full!"<<endl;
				UnlockEnQ();
				this->Print();
				exit(0);
			}
			if (IsEmpty())
			{ 
				head = tail = 0; 
			}
			else
			{
			    tail = (tail+1)%MAX_SIZE;
			}
		
		A[tail] = x;
		printf("enq: %d\n", x);
		UnlockEnQ();
	}

	// Removes an element in Queue from head end. 
	void deq()
	{
		while(LockDeQ()!= 0){};
		printf("Dequeuing... \n");
		if(IsEmpty())
		{
			cout<<"Error: Queue is Empty!"<<endl;
			UnlockDeQ();
			return;
		}
		else if(head == tail ) 
		{
			tail = head = -1;
			UnlockDeQ();
		}
		else
		{
			int a = A[head];
			head = (head+1)%MAX_SIZE;
			printf("deq: %d\n", a);
			UnlockDeQ();
		}
	}
	/* 
	   Printing the elements in queue from head to tail. 
	   This function is only to test the code. 
	   This is not a standard function for Queue implementation. 
	*/
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
	printf("Welcome to thread %ld\n", tid);

	for(int i=tid+50; ;i++){
		if(_random[i] == 1) q.enq(val);
		if(_random[i] == 0) q.deq();
		val++;
		if(i == 999) i=0;
	}

	pthread_exit(NULL);

}

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

int main ()
{
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int rc;
	long i;
	void* status;

	//random array declaration
	FILE *fp;
    fp = fopen("/home/nitish/Workspace/a2/random_1", "r");
	for(int t=0; t<1000; t++){
		fscanf(fp, "%d", &_random[t]);
	}
	fclose(fp);

	//thread creation and locks initialization
	pthread_attr_init(&attr);
	pthread_mutex_init(&Elock, NULL);
	pthread_mutex_init(&Dlock, NULL);
	for( i=0; i < NUM_THREADS; i++ ){
		rc = pthread_create(&threads[i], NULL, threadFn, (void*) i);
		if (rc){
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
	}

	//join main with threads
	for(int t=0; t<NUM_THREADS; t++){
		rc = pthread_join(threads[t], &status);
		if(rc){
			printf("ERROR: Return from pthread_join() is %d\n", rc);
		}
	}
	//q.Print();

	return 0;
}