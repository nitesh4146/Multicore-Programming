#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <cstdint>

//template<class T>

class Node{
public:
	int key;
	atomic<Node*> next;

	Node(int data){
		key = data;
		printf("%d\n", this->key);
	}
};

Node* AtomicMarkableReference(Node* N, bool value);

class Window{
public:
	Node* pred;
	Node* curr;
	Window(Node* myPred, Node* myCurr){
		pred = myPred; 
		curr = myCurr;
	}
};

Window* find(Node* head, int key){
	Node* pred = NULL;	Node* curr = NULL;	Node* succ = NULL;
	bool marked[] = {false};
	bool snip;
	retry: while(true){
		pred = head;
		curr = pred->next; //pred.next.getReference()
		while(true){
			succ = curr->next; //curr.next.get(marked);
			while(marked[0]){
				snip = pred->next.compare_exchange_weak(curr, succ, memory_order_release,
                                        memory_order_relaxed); //pred.next.compareAndSet(curr, succ, false, false)
				if(!snip) goto retry;
				curr = succ;
				succ = curr->next; //curr.next.get(marked)
			}
			if(curr->key >=key)
				return new Window(pred, curr);
			pred = curr;
			curr = succ;
		}
	}
}

class List{
	atomic<Node*> head;
	List(){ head.store(NULL,std::memory_order_relaxed); }
public:
	void print();
	bool add(int item);
	bool remove(int item);
	bool contains(int item);
};

bool List:: add(int item){
	printf("Hello add\n");
	while(true){
		int key=item; //item.hashCode();
		Window* window = find(head, key);
		Node* pred = window->pred; Node* curr = window->curr;
		if(curr->key == key){
			return false;
		}else{
			Node* node = new Node(item);
			node->next = AtomicMarkableReference(curr, false);
			if(pred->next.compare_exchange_weak(curr, node, memory_order_release,
                                        memory_order_relaxed)){
				return true;
			}
		}
	}
}

bool List:: remove(int item){
	printf("Hello remove\n");
	int key = item; //item.hashCode();
	bool snip;
	while(true){
		Window* window = find(head, key);
		Node* pred = window->pred; Node* curr = window->curr;
		if(curr->key != key){
			return false;
		}else{
			Node* succ = (curr->next); //curr.next.getReference()
			snip = 1; //curr.next.attemptMark(succ, true)
			if(!snip)
				continue;
			pred->next.compare_exchange_weak(curr, succ, memory_order_release,
                                        memory_order_relaxed);; //pred.next.compareAndSet(curr, succ, false, false)
			return true;
		}
	}

}

bool List:: contains(int item){
	printf("Hello Contains\n");
	bool marked[] = {false};
	int key = item; //item.hashCode()
	Node* curr = head;
	while(curr->key < key){
		curr = curr->next;
		Node* succ = curr->next; //curr.next.get(marked)
	}
	return (curr->key == key && !marked[0]);
}


//Bit Stealing Functions
bool getFlag(uintptr_t p){
	return (p & 1);
}

uintptr_t setFlag(uintptr_t p, bool value){
	return ((p & (UINTPTR_MAX ^ 1)) | value);
}

Node* AtomicMarkableReference(Node* N, bool value){
	uintptr_t p = (uintptr_t)N;
	return (Node*)((p & (UINTPTR_MAX ^ 1)) | value);
}

template <typename T>
T* xor_ptrs(T* t1, T* t2)
{
    return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(t1)^reinterpret_cast<uintptr_t>(t2));
}
