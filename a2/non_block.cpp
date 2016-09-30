/* Queue - Circular Array implementation in C++*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
using namespace std; 
#define MAX_SIZE 101  //maximum size of the array that will store Queue. 
#define NUM_THREADS     2

struct node
{
    int data;
    node* first;
    node* last;
    node(const int& data) : data(data) {}
};

class NonBlocking_Queue
{
private:
    atomic<node*> head;
    atomic<node*> tail;
 public:
    void enq(int value)
    {
        node* new_node = new node(value);
        do{
             new_node->first = head.load(memory_order_relaxed);

        }while(!atomic_compare_exchange_weak(&head, &new_node->first, new_node));

    printf("%d\n", new_node->data);
    }

/*    int deq(){
        int value;
        node* new_node;
    }*/
};
 
int main()
{
    NonBlocking_Queue q;
    q.enq(1);
    q.enq(2);
    q.enq(3);
    q.enq(9);
}