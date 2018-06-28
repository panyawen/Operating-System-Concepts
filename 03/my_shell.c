#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_LEN 100

void wait_child(int signo){
	int stat;
//	pid_t pid;
	if(waitpid(-1, &stat, WNOHANG) < 0){
		printf("wait child process error\n");
		exit(1);
	}
}
//							  char *args[]  //error
int parse_commond(char *line, char args[][MAX_LEN]){
	int i, j, k, len, ok;
	len = strlen(line);
	i = 0;
	j = 0;
	k = 0;
	while(i < len){
		if(line[i] == ' ' || line[i] == '\t' || line[i] == '\n'){
			if(k){
				args[j][k] = '\0';
				j ++;
				k = 0;
			}
			i ++;
		}else{
			args[j][k++] = line[i++];
		}
	}
	if(k > 0){
		args[j][k] = '\0';
		j ++;
	}
	return j;
}

void exe_commond(char *para[]){
		if(strcmp("ls", para[0]) == 0){
			execvp(para[0], para);
		}
		else if(strcmp("cat", para[0]) == 0){
			execvp(para[0], para);
		}else if(strcmp("ps", para[0]) == 0){
			execvp(para[0], para);
		}
		else{
			printf("Invalid commond\n");
		}
}

int main(){
	char args[MAX_LEN][MAX_LEN], line[MAX_LEN];
	char *para[MAX_LEN];
	int bg, should_run = 1, i, words_cnt, len;
//	signal(SIGCHLD, wait_child);
	while(should_run){
		printf("ywpan>");
		fflush(stdout);
	
		if((len = read(STDIN_FILENO, line, MAX_LEN)) < 0){
			printf("Get line error\n");
			exit(1);
		}
	    if(line[0] == '\n')
			continue;
		line[len] = '\0';

		words_cnt = parse_commond(line, args);
		if(words_cnt == 0)
			continue;
		
		for(int i = 0; i < words_cnt; i ++)
			para[i] = args[i];
		para[words_cnt] = NULL;
		
		bg = 0;
		if(strcmp(para[words_cnt - 1], "&") == 0){
			bg = 1;
			para[words_cnt - 1] = NULL;
		}	

		pid_t pid = fork();
		if(pid == 0){
			exe_commond(para);
		}else if(pid > 0){
			if(bg){
				if(waitpid(pid, NULL, WNOHANG) < 0){
					printf("wait child process error\n");
					exit(0);
				}
			}else{
				waitpid(pid, NULL, 0);			
			}
		}else{
			printf("Create process error\n");
			should_run = 0;
		}
	}

	return 0;
}
