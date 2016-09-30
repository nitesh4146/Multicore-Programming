/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
using namespace std; 
#define MAX_SIZE 101
#define NUM_THREADS     2


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
            printf("Slot before CAS: %d\nTail before CAS: %d\n", slot, tail.load(memory_order_relaxed));
        
        }while(!atomic_compare_exchange_weak(&tail, &slot, slot+1));

        A[slot] = value;
        
        printf("Slot after CAS: %d\nValue: %d\nTail after CAS: %d\n", slot, value, tail.load(memory_order_relaxed));
        printf("A[%d]: %d\n",slot, A[slot]);
    }

    int deq(){
        int value;
        int slot;
        do{
            slot = head.load(memory_order_relaxed);
            value = A[slot];
            if(value == NULL){
                printf("Error: Queue is Empty.\n");
                exit(0);
            }
        }while (!atomic_compare_exchange_weak(&head, &slot, slot+1));
        return value;
    }
};

 
int main()
{
    NonBlocking_Queue q;
    q.enq(3);
    q.enq(9);
    q.enq(10);
    printf("Dequeued: %d\n", q.deq());
    printf("Dequeued: %d\n", q.deq());
    printf("Dequeued: %d\n", q.deq());

}