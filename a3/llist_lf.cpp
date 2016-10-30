#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <atomic>
#include <cstdint>

using namespace std;

//template<class T>

class Node{
public:
	int key;
	Node* next;

	Node(int data){
		key = data;
		printf("%d\n", this->key);
	}
};

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
				snip = 1; //atomic_compare_exchange_weak(pred->next, curr, succ); //pred.next.compareAndSet(curr, succ, false, false)
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
	Node* head;
	List(){ head = NULL; }
	void print();
	bool add(int item);
	bool remove(int item);
	bool contains(int item);
};

bool List:: add(int item){
	while(true){
		int key=1; //item.hashCode();
		Window* window = find(head, key);
		Node* pred = window->pred; Node* curr = window->curr;
		if(curr->key == key){
			return false;
		}else{
			Node* node = new Node(item);
			node->next = curr; //new AtomicMarkableReference(curr, false)
			if(1){	//atomic_compare_exchange_weak(pred.next, curr, node)){
				return true;
			}
		}
	}
}

bool List:: remove(int item){
	int key = 1; //item.hashCode();
	bool snip;
	while(true){
		Window* window = find(head, key);
		Node* pred = window->pred; Node* curr = window->curr;
		if(curr->key != key){
			return false;
		}else{
			Node* succ = curr->next; //curr.next.getReference()
			snip = 1; //curr.next.attemptMark(succ, true)
			if(!snip)
				continue;
			pred->next = succ; //pred.next.compareAndSet(curr, succ, false, false)
			return true;
		}
	}

}

bool List:: contains(int item){
	bool marked[] = {false};
	int key = 1; //item.hashCode()+63
	Node* curr = head;
	while(curr->key < key){
		curr = curr->next;
		Node* succ = curr->next; //curr.next.get(marked)
	}
	return (curr->key == key && !marked[0]);
}

bool getFlag(uintptr_t p){
	return (p & 1);
}

uintptr_t setFlag(uintptr_t p, bool value){
	return ((p & (UINTPTR_MAX ^ 1)) | value);
}

template <typename T>
T* xor_ptrs(T* t1, T* t2)
{
     return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(t1)^reinterpret_cast<uintptr_t>(t2));
}

int main(){
	printf("Hello World\n");
	Node* curr = new Node(5);
	Node* pred = new Node(5);
	Window* window = new Window(curr, pred);
	uintptr_t c = (uintptr_t)curr;
	printf("%d\n", (int)c);
	printf("%d\n", getFlag(c));
	c = setFlag(c, true);
	printf("%d\n", (int)c);
	printf("%d\n", getFlag(c));
	c = setFlag(c, false);
	printf("%d\n", getFlag(c));
	curr = (Node*)c;

	//cout>>curr.key;
	return 0;
}