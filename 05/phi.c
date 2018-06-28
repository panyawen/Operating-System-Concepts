#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <semaphore.h>

#define NUM 5

pthread_mutex_t mutex[NUM];

void think(int order){
//	printf("Philosophers %d think\n", order);
	sleep(1.0 / (rand() % 100));
//	int cnt = rand() % 1000000;
//	for(int i = 0; i < cnt; i ++)
//		;
	return ;
}

void eat(int order){
	printf("Philosophers %d eat begin\n", order);
	sleep(1.0 / (rand() % 100));
//	int cnt = rand() % 1000000;
//	for(int i = 0; i < cnt; i ++)
//		;
	printf("Philosophers %d eat end\n", order);
	return ;
}

void * Philosophers(void * args){
	int order = *((int *)args);
	
	if(order % 2 == 0){
		think(order);
		pthread_mutex_lock(&mutex[order]);
		pthread_mutex_lock(&mutex[(order + 1) % NUM]);
		
		eat(order);

		pthread_mutex_unlock(&mutex[order]);
		pthread_mutex_unlock(&mutex[(order + 1) % NUM]);

	}else{
		think(order);
		pthread_mutex_lock(&mutex[(order + 1) % NUM]);
		pthread_mutex_lock(&mutex[order]);

		eat(order);

		pthread_mutex_unlock(&mutex[(order + 1) % NUM]);
		pthread_mutex_unlock(&mutex[order]);
	}
	return NULL;
}

int main(){
	int i = 0, id[NUM];
	pthread_t pid[NUM];

	for(i = 0; i < NUM; i ++)
		pthread_mutex_init(&mutex[i], NULL);
	
	for(i = 0; i < NUM; i ++){
		id[i] = i;
		if(pthread_create(&pid[i], NULL, &Philosophers, &id[i]) != 0){
			printf("create pthread error\n");
			exit(1);
		}
	}

	for(i = 0; i < NUM; i ++)
		pthread_join(pid[i], NULL);

	return 0;
}
