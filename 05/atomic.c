#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdatomic.h>


atomic_int b;


//int b = 0;

void * add(void * args){
	for(int i = 0; i < 1000000; i ++){
	//	b ++;
	//	atomic_inc(&b);
	atomic_fetch_add(&b, 1);
	}

	return NULL;
}

int main(){
	pthread_t pid1, pid2;
	if(pthread_create(&pid1, NULL, &add, NULL) != 0 \
			|| pthread_create(&pid2, NULL, &add, NULL) != 0){
		printf("pthread create error!\n");
		exit(1);
	}

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	printf("%d\n", b);

	return 0;
}
