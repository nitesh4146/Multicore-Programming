#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define PHILOS 5
#define FOOD 200
#define DELAY 5000
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct{
	int id;
	int state;
	int phil_count;	
	pthread_mutex_t chopstick;
	pthread_t philos;
} philos_struct;

philos_struct *pg;

//pthread_mutex_t chopstick[PHILOS];
//pthread_t philos[PHILOS];

void *philosopher(void *arg);

int main(int argn, char* argv[]){
	pthread_attr_t attr;
	int rc;
	long t;
	void* status;

	int N = 5;
	pp = initialize_v(N);

/*	for(int i=0;i<N;i++){
		pp[i].id=i;
		pp[i].state=THINKING;
		pp[i].phil_count=N;
	}
*/	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(t = 0; t<N; t++){
		pthread_mutex_init(&(pp[t].chopstick),NULL);	
	}
	
	for(t = 0; t<N; t++){
		rc = pthread_create(&(pp[t].philos), &attr, philosopher, (void*)t);
		if(rc){
			printf("ERROR: Return from pthread_create() is %d\n", rc);
		}
	}
	
	for(t=0; t<N; t++){
		rc = pthread_join((pp[t].philos), &status);
		if(rc){
			printf("ERROR: Return from pthread_join() is %d\n", rc);
		}
	}
	
	printf("FOOD Finished!\nExiting..");
	return 0;
}

void *philosopher(void* arg){
	long i=(long*)arg;
//	printf("%d Success\n..",pp[i]);
}

void *initialize_v(int phil_count)
{
  philos_struct *pi;
  int i;

  pi = (philos_struct *) malloc(sizeof(philos_struct));
  pi->phil_count = phil_count;
  pi->id = (int *) malloc(sizeof(int)*phil_count);
  pi->state = (int *) malloc(sizeof(int)*phil_count);
  if (pi->state == NULL) { perror("malloc"); exit(1); }
  for (i = 0; i < phil_count; i++) {
    pi->id[i] = i;
    pi->state[i] = THINKING;
  }

  return (void *) pi;
}
