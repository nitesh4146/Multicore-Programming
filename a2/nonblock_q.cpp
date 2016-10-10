/* Queue - Bounded Non-Blocking Queue implementation in C/C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <ctime>
using namespace std; 
#define MAX_SIZE 1000000  //maximum size of the array that will store Queue. 
#define NUM_THREADS 32   //Total number of threads

int _random[MAX_SIZE];  //Pre-generated array of ramdom numbers
int A[MAX_SIZE];    //Queue object
int index = 0;
int total_cals = 0; //total number of enq's and deq's
int test_bench;

class NonBlocking_Queue
{
    atomic<int> head;
    atomic<int> tail;
public:
    NonBlocking_Queue(){
        head.store(0,std::memory_order_relaxed); 
        tail.store(0,std::memory_order_relaxed); 
    }

    void enq(int value){

        int slot;
        do{
            if((tail.load(memory_order_relaxed) - head.load(memory_order_relaxed)) >= MAX_SIZE)
            {
                //printf("Queue is Full!\nExiting..\n");
                //this->print();
                return;
            }      
        
            slot = tail.load(memory_order_relaxed);        
        }while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));
        
        A[slot] = value;
        //printf("Enqueued: %d Head: %d Tail:%d\n", value, head.load(memory_order_relaxed), tail.load(memory_order_relaxed));        
    }

    void deq(){
        
        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        do{
            start = head.load(memory_order_relaxed);
            if(start == end){
                //printf("Error: Queue is Empty.\n");
                return;
            }
        }while (!atomic_compare_exchange_weak(&head, &start, start+1));
        //printf("Dequeued: %d\n", A[value]);
    }

    void print(){

        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        printf("Queue: ");
        for(int i = start; i<end; i++){
            printf("%d ", A[i]);
        }
        printf("\n");
    }

}q;


void *threadFn(void *threadid)
{
    long tid;
    tid = (long)threadid;
    int val = (int) tid;

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