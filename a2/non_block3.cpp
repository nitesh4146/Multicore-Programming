/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
using namespace std; 
#define MAX_SIZE 10
#define NUM_THREADS 32

int _random[1000];
int A[MAX_SIZE];


class NonBlocking_Queue
{

    atomic<int> head;
    atomic<int> tail;
public:
    NonBlocking_Queue(){
        head.store(-1,std::memory_order_relaxed); 
        tail.store(-1,std::memory_order_relaxed); 
    }

    bool IsEmpty()
    {
        return (head.load(memory_order_relaxed) == -1 && tail.load(memory_order_relaxed) == -1); 
    }

    // To check whether Queue is full or not
    bool IsFull()
    {
        return ((tail.load(memory_order_relaxed)+1)%MAX_SIZE == head.load(memory_order_relaxed) ? true : false);
    }

    void enq(int value){
        printf("checkE!\n");
        if(IsFull())
        {
            cout<<"Error: Queue is Full!"<<endl;
            this->print();
            exit(0);
        }
        if (IsEmpty())
        {   printf("Queue Empty!\n");
            head.store(0,std::memory_order_relaxed); 
            tail.store(0,std::memory_order_relaxed);
        }

        int slot;
        do{
            slot = tail.load(memory_order_relaxed);
        }while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));
        A[slot] = value;
        printf("Enqueued: %d\n", value);
    }

    void deq(){
        printf("CheckD!\n");
        if(IsEmpty())
        {
            cout<<"Error: Queue is Empty!"<<endl;
            return;
        }
        else if(head.load(memory_order_relaxed) == tail.load(memory_order_relaxed ) )
        {
            head.store(-1,std::memory_order_relaxed); 
            tail.store(-1,std::memory_order_relaxed); 
        }
        else
        {
            int start = head.load(memory_order_relaxed);
            int end = tail.load(memory_order_relaxed);
            int value;
            do{
                int start = head.load(memory_order_relaxed);
                value = A[start];
                if(value == 0){
                    printf("Error: Queue is Empty.\n");
                    printf("A[%d]: %d\n", start, A[start]);
                    return;
                    // exit(0);
                }
            }while (!atomic_compare_exchange_weak(&head, &start, start+1));
            printf("Dequeued..\n");
        }
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
    printf("Welcome to thread %ld\n", tid);

    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.enq(tid++);
    q.deq();

    pthread_exit(NULL);

}
 
int main()
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
    q.print();

    return 0;
}