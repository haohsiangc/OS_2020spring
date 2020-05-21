#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
using namespace std;

#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N

#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_t thread[N];

class control{
public:
	void init();
	void show_state();
	void test(int i);
	void take_forks(int i);
	void put_forks(int i);
private:
	int state[N];
	pthread_cond_t cond[N];
	pthread_mutex_t mutex;
	pthread_attr_t attr;
};

control c;

void control::init(){
	pthread_mutex_init(&mutex, NULL); //initialize mutex
	for(int t=0; t<N; t++){
		pthread_cond_init(&cond[t], NULL); //initialize variable condition
	}
}

void control::show_state(){
	printf("\t\t");
	for(int t=0; t<N; t++){
		if(state[t]==THINKING)printf(" T ");
		else if(state[t]==HUNGRY)printf(" H ");
		else if(state[t]==EATING)printf(" E ");
	}
	printf("\n");
}

void control::test(int i){
	while(!(state[i]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING)){
		pthread_cond_wait(&cond[i], &mutex); //1.unlock 2.wait for conditional variable 3.lock
	}
	state[i]=EATING;
	printf("philosopher %d is EATING  ", i+1);
	show_state();
}

void control::take_forks(int i){
	pthread_mutex_lock(&mutex);
	state[i] = HUNGRY;
	printf("philosopher %d is HUNGRY  ", i+1);
	show_state();
	test(i);
	pthread_mutex_unlock(&mutex);
	sleep(1);
}

void control::put_forks(int i){
	pthread_mutex_lock(&mutex);
	state[i] = THINKING;
	printf("philosopher %d is THINKING", i+1);
	show_state();
	pthread_cond_signal(&cond[LEFT]);
	pthread_cond_signal(&cond[RIGHT]);
	pthread_mutex_unlock(&mutex);
	sleep(1);
}

void* philosopher(void* n){
	int i = *(int*)n;
	while(1){
		sleep(1);
		c.take_forks(i);
		sleep(3);
		c.put_forks(i);
	}
	pthread_exit(0);
}

int main(void){
	int p[N] = {0, 1, 2, 3, 4};
	c.init();
	printf("-----T represents THINKING, H represents HUNGRY, E represnts EATING-----\n");
	for(int t=0; t<N; t++){
		pthread_create(&thread[t], NULL, philosopher, (void*)&p[t]);
	}
	for(int t=0; t<N; t++){
		pthread_join(thread[t], NULL);
	}
	return 0;
}

