#include <atomic>
 
using namespace std;

template<class T>
struct node
{
    T data;
    node* next;
    node(const T& data) : data(data), next(nullptr) {}
};
 
template<class T>
class stack
{
    atomic<node<T>*> head;
    atomic<node<T>*> tail;
 public:
    void push(const T& data)
    {
        node<T>* new_node = new node<T>(data);

        new_node->next = head.load(std::memory_order_relaxed);

        while(!atomic_compare_exchange_weak(&head, &new_node->next, new_node));
    }
};
 
int main()
{
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
}