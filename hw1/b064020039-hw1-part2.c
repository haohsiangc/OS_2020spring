#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#define FALSE 0
#define TRUE 1
#define STD_INPUT 0
#define STD_OUTPUT 1

static char *args1[100], *args2[100];

int read_cmd(char *cmd1, char *cmd2, char c){
	int i = 0;
	char blank_t = ' ';
	char *temp, *blank = &blank_t;
	if(cmd2 == NULL){
		temp = strtok(cmd1, blank);
		args1[i] = temp; i++;
		while(temp != NULL){
			temp = strtok(NULL, blank);
			args1[i] = temp; i++;
		}
		if(!strcmp(args1[0], "cd\0")){
			chdir(args1[1]); return -1;
		}
	}
	else{
		char sep_t = '|', in_t = '<', out_t = '>';
		char *temp2, *sep = &sep_t, *in = &in_t, *out = &out_t, *stop;
		if(c == '<') stop = in;
		else if(c == '>') stop = out;
		else stop = sep;
		temp2 = strtok(cmd1, stop);
		temp = strtok(temp2, blank);
		args1[i] = temp; i++;
		while(temp != NULL){
			temp = strtok(NULL, blank);
			args1[i] = temp, i++;
		}
		i = 1;
		temp2 = strtok(cmd2, blank);
		temp = strtok(temp2, blank);
		args2[i] = temp; i++;
		while(temp != NULL){
			temp = strtok(NULL, blank);
			args2[i] = temp, i++;
		}
	}
	return 0;
}

int main(void){
	int judge, fin, fout, fd[2];
	char input[200];
	char *process1, *process2;
	char sep = '|', back = '&', in = '<', out = '>', stop;
	pid_t pid, parent, child;
	while(TRUE){
		printf("[b064020039@shell]$ ");
		fgets(input, 200, stdin);
		input[strlen(input)-1] = '\0';
		process1 = input;
		if(!strcmp(process1, "exit")) break;
		if(strchr(input, back) != NULL){
			process2 = strchr(input, back); process2++;
			stop = back;
		}
		else if(strchr(input, sep) != NULL){
			process2 = strchr(input, sep); process2++;
			stop = sep;
		}
		else if(strchr(input, in) != NULL){
			process2 = strchr(input, in); process2++;
			stop = in;
		}
		else if(strchr(input, out) != NULL){
			process2 = strchr(input, out); process2++;
			stop = out;
		}
		else process2 = NULL;

		pipe(&fd[0]);
		if((pid = fork()) != 0){
			if(stop == back) continue;
			close(fd[1]);
			close(fd[0]);
			waitpid(pid, NULL, 0);
		}
		else{
			if(process2 == NULL){ //single process
				judge = read_cmd(process1, process2, stop);
				if(judge == -1) continue;
				if(fork() != 0) wait(0);
				else execvp(args1[0], args1);
			}
			else if(stop == sep){ //pipe
				read_cmd(process1, process2, stop);
				if((parent = fork()) == 0){
					close(fd[0]);
					close(STD_OUTPUT);
					dup(fd[1]);
					close(fd[1]);
					execvp(args1[0], args1);
					exit(0);
				}
				if((child = fork()) == 0){
					close(fd[1]);
					close(STD_INPUT);
					dup(fd[0]);
					close(fd[0]);
					execvp(args2[0], args2);
					exit(0);
				}
				close(fd[0]);
				close(fd[1]);
				waitpid(child, NULL, 0);
			}
			else if(stop == in){ //input
				read_cmd(process1, process2, stop);
				fin = open(args2[0], O_RDONLY);
				close(STD_INPUT);
				dup2(fin, STD_INPUT);
				close(fin);
				execvp(args1[0], args1);
			}
			else if(stop == out){ //output
				read_cmd(process1, process2, stop);
				fout = open(args2[0], O_WRONLY | O_CREAT | O_TRUNC);
				close(STD_OUTPUT);
				dup2(fout, STD_OUTPUT);
				close(fout);
				execvp(args1[0], args1);
			}
		}
	}
	return 0;
}
