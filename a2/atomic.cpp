#include <atomic>
 
using namespace std;

struct node
{
    int data;
    node* next;
    node(const int& data) : data(data), next(nullptr) {}
};

class Queue
{
    atomic<node*> head;
    atomic<node*> tail;
 public:
    void enq(const int& data)
    {
        node* new_node = new node(data);
        do{
            new_node->next = head.load(memory_order_relaxed);

        }while(!atomic_compare_exchange_weak(&head, &new_node->next, new_node));

    }
};
 
int main()
{
    Queue q;
    q.enq(1);
    q.enq(2);
    q.enq(3);
}