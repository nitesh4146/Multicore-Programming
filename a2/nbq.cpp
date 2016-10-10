#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<atomic>

using namespace std;

#define NUM_THREADS 1
#define MAX_SIZE 100

int randomArr[MAX_SIZE] ;
int arrHead=0 ;
int atomic_Method_Called =0 ;
// Make atomic head and tail
std::atomic<int> head ;
std::atomic<int> tail ;     
int theQueue[MAX_SIZE];



void init_Queue() ;
bool myq_enqueue(int arg) ;
bool  myq_dequeue();
void displayArr();

	bool myq_enqueue(int arg)
{	
	if((tail.load(memory_order_relaxed)) - (head.load(memory_order_relaxed)) >= MAX_SIZE) return false; // theQueue[] is full.
	else
	{	int slot ;	
		do
		{
		    slot = tail.load(memory_order_relaxed);        
        }
		while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));
        theQueue[slot] = arg;
        printf("Enqueued: %d Head: %d Tail:%d\n", arg, head.load(memory_order_relaxed), tail.load(memory_order_relaxed));        
		return true;
	}
}

bool myq_dequeue(){
		
       if((tail.load(memory_order_relaxed)) == (head.load(memory_order_relaxed)) )
		{	
			return false ; // que is empty
		}
		else
		{
		int start = head.load(memory_order_relaxed);
        int value;
        do{
            if((tail.load(memory_order_relaxed)) == (head.load(memory_order_relaxed)) )
				{	// que is empty
					return false ;
				}
            value = theQueue[start];
           	//printf("theQueue[%d]: %d\n", start, theQueue[start]);
        }while (!atomic_compare_exchange_weak(&head, &start, start+1));
        printf("Dequeued: %d\n", value);
        return true;	
		}
		
        
    }
void init_Queue()
{
    //head = tail = -1;
	 head.store(0,std::memory_order_relaxed); 
     tail.store(0,std::memory_order_relaxed);
}

void initRandom()
{
    for(int i=0;i<MAX_SIZE; i++)
    {
        //randomArr[i] = rand() %3 ; // Case 1 & Case 2 66% enque() or deque() ;
        randomArr[i] = rand() %2 ; // Case 3 50% enque() and deque() ;
    }
}

/*void displayArr()
{
	for(int i=0;i<(sizeof(theQueue)/sizeof(theQueue[0])); i++)
	{
		printf("Display %d !", theQueue[i]);
		
	}
}*/

    void displayArr(){

        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        printf("Queue: ");
        for(int i = start; i<end; i++){
            printf("%d ", theQueue[i]);
        }
        printf("\n");
    }


void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    while (atomic_Method_Called <= (MAX_SIZE*2))
    {
        if(arrHead == MAX_SIZE)
        {	//printf("RESET DONE RESET\n");
            arrHead = 0 ;
        }
        //if (randomArr[arrHead] == 0 || randomArr[arrHead]== 1) // case 1 66% are enque() ;
        	if (randomArr[arrHead] == 0) // case 2 66% are deque() and case 3 50% enq 50% deq;
					{
						if(myq_enqueue(arrHead))
								{
										//printf("Element Pushed is %d!", atomic_Method_Called);
										//printf("By Thread %ld!\n", tid);
								}
								else
								{
									//printf("Queue Is Full");
								}
								arrHead++ ;
								atomic_Method_Called++ ;
					}
        //else  if (randomArr[arrHead] == 2)  // case 1 66% are enque()
        //else  if (randomArr[arrHead] == 1 || randomArr[arrHead]== 2) //case 2 66% are deque() ;
        else  if (randomArr[arrHead] == 1) //case 3 50% enque() and deque() ;
				{	//int i ;
							if(myq_dequeue())
									{
										//printf("Deque Called!");
										//printf(" By Thread %ld!\n", tid);
									}
									else
									{
										//printf("Cannot Dequeue \n");
									}
									arrHead++;
									atomic_Method_Called++ ;
				}
    }
    // displayArr() ;
}



int main(int argc, char *argv[])
{	initRandom() ; 
	init_Queue() ;
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	void* status;

	for (t = 0; t<NUM_THREADS; t++) {
		//printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);

			exit(-1);
		}
	}

	for(int t=0; t<NUM_THREADS; t++){
        rc = pthread_join(threads[t], &status);
        if(rc){
            printf("ERROR: Return from pthread_join() is %d\n", rc);
        }
    }
	
	displayArr();
	pthread_exit(NULL);
}