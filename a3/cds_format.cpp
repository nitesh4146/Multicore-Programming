
static void add(void *obj){
	List *list = (List *) obj;
	list->add(); 
}
static void remove(void *obj){
	List *list = (List *) obj;
	list->remove();
}
static void contains(void *obj){
	List *list = (List *) obj;
	list->contains();
}


int user_main(int argc, char *argv[]){
	thrd_t t1, t2, t3;
	pthread_attr_t attr;
    int rc;
    long i;
    void* status;
    pthread_attr_init(&attr);
    List* ll;

    //Create threads
	thrd_create(&t1, (thrd_start_t) &add, (void *) ll);
	thrd_create(&t2, (thrd_start_t) &remove, (void *) ll);
	thrd_create(&t3, (thrd_start_t) &contains, (void *) ll);

    //Join threads
	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

	return 0;
}