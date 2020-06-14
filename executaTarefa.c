#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "header.h"

int * pids;
int pids_count=0;
int timeout=0;
int segundos=0;
int completa = 0;
int ret=0;
int zat=0;
int servidor;

void sigalarm2_handler(int signum){
	
	for(int i=0; i<pids_count; i++){	
		if(pids[i]>0){
			//printf("killing grep %d\n", pids[i]);
			kill(pids[i],SIGKILL);
		}
		kill(getpid(),SIGKILL);
		_exit(3);
	}

}

void sigalarm_handler(int signum){	

	for(int i=0; i<pids_count; i++){
		if(pids[i]>0){
			//printf("killing grep %d\n", pids[i]);
			kill(pids[i],SIGKILL);
		}
		write(1,"processo terminado\n",19);
	}
	kill(servidor,SIGUSR1);
	//printf("processo pai terminado %d\n",getpid());
	kill(getpid(),SIGKILL);	
} 

void parsecomand(char * comando, char * resultado[]){

	char * ptr = strtok(comando, " ");
	int i = 0;
	while(ptr != NULL)
	{		
		resultado[i++] = strdup(ptr);
		ptr = strtok(NULL, " ");
	}
	resultado[i]=NULL;
}



int executa (char * args,int tempoExec, int tempoInat, int servpid){

	servidor=servpid;
			if(signal(SIGALRM, sigalarm_handler)==SIG_ERR){
						perror("signal sigalarm");	
						exit(3);
			}

			if(signal(SIGUSR1, sigalarm2_handler)==SIG_ERR){
						perror("signal sigint");	
						exit(4);
			}
	alarm(tempoExec);
	int quantos2=1;
	for(int z =0; args[z]!='\0';z++){
		if(args[z]=='|') quantos2++;
	}
	char * ptr = strtok(args,"|");
	int quantos=0;
	char **pedidos=malloc(quantos2*sizeof(char*));

	while(ptr != NULL){	
		pedidos[quantos] = strdup(ptr);
		ptr = strtok(NULL, "|");
		quantos++;
	}

	int pid;
	int j=-1;
	if(quantos==1){

		pids_count=1;
		pids = (int*) malloc(sizeof(int)*quantos);
			if((pid=fork())==0){	
			
			char * respostas[10];
			int fd2=open("FIFO2",O_WRONLY);
			dup2(fd2,1);
			parsecomand(pedidos[0], respostas);
			execvp(respostas[0], respostas);
			close(fd2);
			_exit(1);
		}
		pids[0]=pid;
		
	}else{

	quantos += (quantos -1);
	pids_count=quantos;
	pids = (int*) malloc(sizeof(int)*quantos);
	int pipe_fdd[quantos-1][2];
	
	for(int i=0; i<quantos;i++ ){
		
		if(i==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}
			j++;
			if((pid=fork())==0){

			close(pipe_fdd[i][0]);
			dup2(pipe_fdd[i][1],1);
			close(pipe_fdd[i][1]);
			sleep(10);
			char * respostas[10];
			parsecomand(pedidos[j], respostas);
			execvp(respostas[0], respostas);
			_exit(1);
		}
		close(pipe_fdd[i][1]);
	}
		else if(i==quantos-1){

			close(pipe_fdd[i][1]);
			j++;
			if((pid=fork())==0){

			dup2(pipe_fdd[i-1][0],0);
			close(pipe_fdd[i-1][0]);
			int fd2=open("FIFO2",O_WRONLY);
			dup2(fd2,1);
			char * respostas[10];
			parsecomand(pedidos[j], respostas);
			execvp(respostas[0], respostas);
			close(fd2);
			_exit(2);
		}
		close(pipe_fdd[i-1][0]);
	}
		else if (i%2==0){
			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}
			j++;
			if((pid=fork())==0){

			dup2(pipe_fdd[i-1][0],0);
			dup2(pipe_fdd[i][1],1);
			close(pipe_fdd[i-1][0]);
			close(pipe_fdd[i][1]);
			char * respostas[10];
			parsecomand(pedidos[j], respostas);
			execvp(respostas[0], respostas);
			_exit(1);
		}
		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][1]);
		}
			else{
				if(pipe(pipe_fdd[i])<0){
					perror("pipe");
					exit(1);
				}
				if((pid=fork())==0){
					if(signal(SIGALRM, sigalarm2_handler)==SIG_ERR){
						perror("signal sigalarm2");	
						exit(3);
					}
					dup2(pipe_fdd[i-1][0],0);
					dup2(pipe_fdd[i][1],1);
					close(pipe_fdd[i-1][0]);
					close(pipe_fdd[i][1]);	
					int bytes_read;
					char zz[100];
					alarm(tempoInat);
					while((bytes_read=read(0,zz,100))>0){
						write(1,zz,bytes_read);
						alarm(tempoInat);
					}
					_exit(1);
				}
			close(pipe_fdd[i-1][0]);
			close(pipe_fdd[i][1]);
			}
		pids[i]=pid;
		}
	}
	int status;
	for(int i=0;i<quantos;i++){
		wait(&status);
		if(WIFEXITED(status)==0) ret=2;
	}
	if(zat==1){
		return 3;
	} 
	return ret;
}


