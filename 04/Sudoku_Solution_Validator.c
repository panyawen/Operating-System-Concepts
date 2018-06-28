/*
 * Because the concurrent thread could modify array valid, 
 * so the later answer will overwrite the former answer, 
 * which could result in a error! Here we just use sleep
 * system call to stragger these concurrent threads. 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
//#include <thread>
#include <sys/types.h>

#define SCALE 9
#define SCALE_SQRT 3
#define THREADS 27

typedef struct{
    int row;
    int col;
} parameters;

int digits[SCALE][SCALE], valid[THREADS] = {0};

pthread_spinlock_t spin_lock;

void * validate(void * args){
    parameters * para = (parameters *)args;
    int flag[SCALE] = {0};
    int row, col;
    row = para->row;
    col = para->col;
    if(row != -1 && col == -1){ //validate row
        // printf("\n");
        for(int j = 0; j < SCALE; j ++){
            // printf("%d ", digits[row][j]);
            flag[digits[row][j] - 1] = 1;
        }
        int k;
        for(k = 0; k < SCALE; k ++){
            // printf("%d ", flag[k]);
            if(flag[k] != 1){
                break;
            }
        }

        //printf("k = %d\n", k);
        if(k >= SCALE){
			pthread_spin_lock(&spin_lock);
            valid[row] = 1;
            printf("row:%d  %d\n", row, valid[row]);
			pthread_spin_unlock(&spin_lock);
        }

    }else if(row == -1 && col != -1){ //validate col
        for(int i = 0; i < SCALE; i ++)
            flag[digits[i][col] - 1] = 1;
        int k;
        for(k = 0; k < SCALE; k ++)
            if(flag[k] != 1)
                break;
        
        if(k >= SCALE){
			pthread_spin_lock(&spin_lock);
            valid[SCALE + col] = 1;
            printf("col:%d  %d\n", SCALE + col, valid[SCALE + col]);
			pthread_spin_unlock(&spin_lock);
        }

    }else if(row != -1 && col != -1){ //grids
        for(int i = row; i < row + SCALE_SQRT; i ++)
            for(int j = col; j < col + SCALE_SQRT; j ++)
                flag[digits[i][j] - 1] = 1;
        int k;
        for(k = 0; k < SCALE; k ++)
            if(flag[k] != 1)
                break;
        
        if(k >= SCALE){
			pthread_spin_lock(&spin_lock);
            valid[SCALE*2 + row + col / SCALE_SQRT] = 1;
            printf("row col:%d  %d\n", SCALE*2 + row + col / SCALE_SQRT, \
                                       valid[SCALE*2 + row + col / SCALE_SQRT]);
			pthread_spin_unlock(&spin_lock);
		}

    }else{
        printf("Not validate parameters\n");
        exit(0);
    }

    return NULL;
}

int main(){
    int i, j;
    pthread_t tids[THREADS];
	pthread_spin_init(&spin_lock, PTHREAD_PROCESS_PRIVATE);

    for (i = 0; i < SCALE; i ++){
        for (j = 0; j < SCALE; j ++){
            scanf("%d", &digits[i][j]);
            if(digits[i][j] <= 0 || digits[i][j] > SCALE){
                printf("Not validate input\n");
                return 0;
            }
        }
    }
    
    for(i = 0; i < SCALE; i ++){
        parameters * para = (parameters *)malloc(sizeof(parameters));
        // pthread_t tid;
        para->row = i;
        para->col = -1;
        pthread_create(&tids[i], NULL, &validate, para);
     //   sleep(1);
    }

    for(j = 0; j < SCALE; j ++){
        parameters * para = (parameters *)malloc(sizeof(parameters));
        // pthread_t tid;
        para->row = -1;
        para->col = j;
        pthread_create(&tids[SCALE + j], NULL, &validate, para);
     //   sleep(1);
    }

    for(i = 0; i < SCALE; i += SCALE_SQRT){
        for(j = 0; j < SCALE; j += SCALE_SQRT){
            parameters * para = (parameters *)malloc(sizeof(parameters));
            // pthread_t tid;
            para->row = i;
            para->col = j;
            pthread_create(&tids[SCALE * 2 + i + j / SCALE_SQRT], NULL, &validate, para);
       //     sleep(1);
        }
    }

    for(i = 0; i < THREADS; i ++){
        pthread_join(tids[i], NULL);
        if(valid[i] != 1){
            printf("false %d\n", i);
            break;
        }
    }

    if(i < THREADS)
        printf("NO\n");
    else
        printf("YES\n");

    return 0;
}
