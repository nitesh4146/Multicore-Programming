/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
using namespace std; 
#define MAX_SIZE 1000
#define NUM_THREADS 1


int A[MAX_SIZE];


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
            slot = tail.load(memory_order_relaxed);        
        }while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));
        A[slot] = value;
        printf("Enqueued: %d\n", value);
    }

    int deq(){
        int start = head.load(memory_order_relaxed);
        int end = tail.load(memory_order_relaxed);
        int value;
        do{
            int start = head.load(memory_order_relaxed);
            value = A[start];
            if(value == 0){
                printf("Error: Queue is Empty.\n");
                printf("A[%d]: %d\n", start, A[start]);
                // exit(0);
            }
        }while (!atomic_compare_exchange_weak(&head, &start, start+1));
        return value;
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
    printf("Welcome to thread %ld\n", tid);

    while (total_cals <= (MAX_SIZE*2))
    {
        if(index == MAX_SIZE)
        {
            index = 0 ;
        }
        //if (_random[index] == 0 || _random[index]== 1) // test_bench 1: 66% enq
        if (_random[index] == 0) // test_bench 2: 66% deq and test_bench 3: 50% enq and deq
        {
            q.enq(index);
            index++ ;
            total_cals++;
        }
        //else  if (_random[index] == 2)  // test_bench 1: 66% enq
        //else  if (_random[index] == 1 || _random[index]== 2) //test_bench 2: 66% deq
        else (_random[index] == 1) // test_bench 3: 50% enq and deq
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
            printf("ERROR: Return from pthread_join() is %d\n", rc);
        }
    }
    q.print();

    return 0;
}