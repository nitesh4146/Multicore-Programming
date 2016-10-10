/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <ctime>
using namespace std; 
#define MAX_SIZE 10000000
#define NUM_THREADS 2

int _random[MAX_SIZE];
int A[MAX_SIZE];
int index = 0;
int total_cals = 0;


class NonBlocking_Queue
{

    atomic<int> head;
    atomic<int> tail;
public:
    NonBlocking_Queue(){
        head.store(0,std::memory_order_relaxed); 
        tail.store(0,std::memory_order_relaxed); 
    }

    void enq(int value)
    {
        int slot;

        do{
            if((tail.load(memory_order_relaxed) - head.load(memory_order_relaxed)) >= MAX_SIZE)
            {
                //printf("Queue is Full!\nExiting..\n");
                this->print();
                exit(0);
            }      
        
            slot = tail.load(memory_order_relaxed);        
        }while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));
        
        A[slot] = value;
        //printf("Enqueued: %d Head: %d Tail:%d\n", value, head.load(memory_order_relaxed), tail.load(memory_order_relaxed));
        
        
    }

    void deq(){
        
        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        int value;
        do{
            //int start = head.load(memory_order_relaxed);
            value = head.load(memory_order_relaxed);
            if(value == end){
                //printf("Error: Queue is Empty.\n");
                return;
            }
        }while (!atomic_compare_exchange_weak(&head, &start, start+1));
        //printf("Dequeued: %d\n", A[value]);
    }

    void print(){
        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        //printf("Queue: ");
        for(int i = start; i<end; i++){
            //printf("%d ", A[i]);
        }
        //printf("\n");
    }
}q;


void *threadFn(void *threadid)
{
    long tid;
    tid = (long)threadid;
    int val = (int) tid;
    //printf("Welcome to thread %ld\n", tid);

/*    for(int i=0; i<500;i++){
        if(_random[i] == 1) q.enq(val);
        if(_random[i] == 0) q.deq();
        val++;
        if(i == 999) i=0;
    }
    //printf("last random number is %d\n",_random[999]);*/
    
    while (total_cals <= (MAX_SIZE))
    {
        //if (_random[index] == 0 || _random[index]== 1) // test_bench 1: 66% enq
        if (_random[index] == 0) // test_bench 2: 66% deq and test_bench 3: 50% enq and deq
        {
            q.enq(index);
            index++ ;
            total_cals++;
        }
        //else  if (_random[index] == 2)  // test_bench 1: 66% enq
        //else  if (_random[index] == 1 || _random[index]== 2) //test_bench 2: 66% deq
        else if (_random[index] == 1) // test_bench 3: 50% enq and deq
        {
            q.deq();    
            index++;
            total_cals++;
        }
    }
    pthread_exit(NULL);

}
 
int main()
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long i;
    void* status;
    pthread_attr_init(&attr);

    unsigned long micros = 0;
    float millis = 0.0;
    clock_t start, end;

    //random array declaration
    FILE *fp;
    //fp = fopen("/home/nitish/Workspace/a2/random_2", "r"); // test_bench
    fp = fopen("/home/nitish/Workspace/a2/random_1", "r");
    for(int t=0; t<NUM_THREADS; t++){
        fscanf(fp, "%d", &_random[t]);
    }
    fclose(fp);

    //Execution Time
    start = clock();

    for( i=0; i < NUM_THREADS; i++ ){
        rc = pthread_create(&threads[i], NULL, threadFn, (void*) i);
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    for(int t=0; t<NUM_THREADS; t++){
        rc = pthread_join(threads[t], &status);
        if(rc){
            //printf("ERROR: Return from pthread_join() is %d\n", rc);
        }
    }

    end = clock();
    micros = end - start;
    millis = micros / 1000;
    printf("Execution Time: %f msec or %lu usec\n", millis, micros);
    printf("Hello\n");
    //q.print();

    return 0;
}