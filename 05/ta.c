#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>

#define STU_MAX 100
#define CHAIR 6

pthread_mutex_t mutex;
sem_t teach_a, stu;
int waiting = 0, ongoing = 0;

void teach(){
	int i = 0;
	for( ; i < 100; i ++) 
		;
}

void * teach_assitant(void * args){
	printf("%d  %d\n", getpid(), syscall(SYS_gettid));
	
	sem_post(&teach_a);
	while(1){
//		sem_post(&teach_a);
		sem_wait(&stu);
		teach();

//		printf("teach_assitant\n");
	}

	return NULL;
}

void * student(void * args){
	pthread_t * pid = (pthread_t *)args;
	while(1){
		pthread_mutex_lock(&mutex);
		if(waiting < CHAIR){
			waiting ++;
			pthread_mutex_unlock(&mutex);
//			sem_post(&stu);
			sem_wait(&teach_a);
			sem_post(&stu);

			pthread_mutex_lock(&mutex);
			waiting --;
			pthread_mutex_unlock(&mutex);
		
			ongoing ++;
			printf("%d\n", ongoing);
			teach();
			ongoing --;
		//	printf("student %d\n", syscall(SYS_gettid));

			sem_post(&teach_a);

			break;

		}else{
//			printf("wait\n");
			pthread_mutex_unlock(&mutex);
			sleep(1.0 / (rand()%100));
		}
	}
	return NULL;
}

int main(){
	pthread_t pta, pstu[STU_MAX];
	
	sem_init(&teach_a, 0, 0);
	sem_init(&stu, 0, 0);
	pthread_mutex_init(&mutex, NULL);

	if(pthread_create(&pta, NULL, &teach_assitant, NULL)){
		printf("create ta thread error\n");
		exit(1);
	}
	int i = 0;
	for( ; i < STU_MAX; i ++)
		if(pthread_create(&pstu[i], NULL, &student, &pstu[i])){
			printf("create student thread %d error\n", i);
			exit(2);
		}

	for(i = 0; i < STU_MAX; i ++)
		pthread_join(pstu[i], NULL);
	pthread_join(pta, NULL);

	return 0;
}
