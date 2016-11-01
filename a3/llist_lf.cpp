#include <iostream>
#include <pthread.h>
#include "List_head.h"

using namespace std;
#define NUM_THREADS 1

//Thread Functions
static void *add(void *obj){
	List *list = (List *) obj;
	list->add(5); 
}
static void *remove(void *obj){
	List *list = (List *) obj;
	list->remove(5);
}
static void *contains(void *obj){
	List *list = (List *) obj;
	list->contains(6);
}


int main(int argc, char *argv[]){
	printf("Hello World\n");
	List* ll;
	ll->add(5);

/*	uintptr_t c = (uintptr_t)curr;
	printf("%d\n", (int)c);
	printf("%d\n", getFlag(c));
	c = setFlag(c, true);
	printf("%d\n", (int)c);
	printf("%d\n", getFlag(c));
	//curr = getReference(curr);
	printf("%d\n", (int)((uintptr_t)(getReference(curr))));
	 c = (uintptr_t)curr->next;
	//c = setFlag(c, false);
	printf("%d\n", getFlag(c));
	curr->next = (Node*)c;*/

	//cout>>curr.key;
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
    int rc;
    void* status;
    pthread_attr_init(&attr);

    //Create threads
	pthread_create(&threads[0], NULL, add, (void*) ll);
	pthread_create(&threads[1], NULL, remove, (void*) ll);
	pthread_create(&threads[2], NULL, contains, (void*) ll);

    //Join threads
    for(int t=0; t<NUM_THREADS; t++){
        rc = pthread_join(threads[t], &status);
        if(rc){
            printf("ERROR: Return from pthread_join() is %d\n", rc);
        }
    }
	return 0;
}