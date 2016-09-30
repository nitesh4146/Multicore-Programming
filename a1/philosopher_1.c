#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
	
#define PHILOS 5
#define FOOD 200
#define DELAY 5000

void *philosopher(void *arg);
int food_left();
void pick_chop(int p, int c, char *hand);
void drop_chop(int c1, int c2);

pthread_mutex_t food_lock;
pthread_mutex_t chopstick[PHILOS];
pthread_t philos[PHILOS];

int main(int argn, char* argv[]){
	pthread_attr_t attr;
	int rc;
	long t;
	void* status;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	pthread_mutex_init(&food_lock, NULL);
	for(t = 0; t<PHILOS; t++){
		pthread_mutex_init(&chopstick[t],NULL);	
	}
	
	for(t = 0; t<PHILOS; t++){
		rc = pthread_create(&philos[t], &attr, philosopher, (void*)t);
		if(rc){
			printf("ERROR: Return from pthread_create() is %d\n", rc);
		}
	}
	
	for(t=0; t<PHILOS; t++){
		rc = pthread_join(philos[t], &status);
		if(rc){
			printf("ERROR: Return from pthread_join() is %d\n", rc);
		}
	}
	
	printf("FOOD Finished!\nExiting..");
	return 0;
}

void *philosopher(void *arg){
	
	int left_chop, right_chop, f = FOOD;
	int tid = (int*)arg;
	right_chop = tid;
	left_chop = tid+1;
	printf("Food left = %d\n",f);
	if(left_chop == PHILOS){
		left_chop = 0;
	}
	while(f = food_left()){
		if(tid == 1){
			sleep(1);
		}
		printf("Philosopher %d is now thinking..\n", tid);
//		usleep(DELAY*(f-1));
		printf("Philosopher %d is now hungry..\n", tid);
		pick_chop(tid, right_chop, "Right hand");
		pick_chop(tid, left_chop, "Left hand");
		printf("Philosopher %d is now eating..\n", tid);
		usleep(DELAY * (FOOD - f + 1));
		drop_chop(left_chop, right_chop);
	}
	return (NULL);
}

int food_left(){
	static int food = FOOD;
	int myfood;
	pthread_mutex_lock(&food_lock);
	if(food>0){
		food--;
	}
	myfood = food;
	pthread_mutex_unlock(&food_lock);
	return myfood;
}

void pick_chop(int id, int c, char* hand){
	
	pthread_mutex_lock(&chopstick[c]);
	printf("Philosspher %d picked Chopstick %d in %s\n",id,c,hand);
}

void drop_chop(int c1, int c2){
	pthread_mutex_unlock(&chopstick[c1]);
	pthread_mutex_unlock(&chopstick[c2]);
}
