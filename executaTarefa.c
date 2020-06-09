#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


int * pids;
int pids_count;
int timeout=0;
int segundos=0;
int completa = 0;

void sigalarm_handler(int signum){


	timeout=1;  

		
}

int parsecomand(char * comando, char * resultado[]){


	


	char * ptr = strtok(comando, " ");


	
	int i = 0;
	while(ptr != NULL)
	{	
		
		resultado[i++] = strdup(ptr);
		ptr = strtok(NULL, " ");
	}

	
	resultado[i]=NULL;


}



int executa (char args[], int tempoInat, int tempoExec){

	if(signal(SIGALRM, sigalarm_handler)==SIG_ERR){

		perror("signal sigalarm");

		exit(3);
	}

	
	//printf("tempoExec= %d\n", tempoExec );

	char * comando = args; 

	

	char * ptr = strtok(comando,"|");

	int quantos=0;


	char ** pedidos = malloc(4*sizeof(char*));

	while(ptr != NULL)
	{	
		
		pedidos[quantos] = strdup(ptr);
		ptr = strtok(NULL, "|");
		quantos++;
	}
	
	

	pids_count=quantos;

	int * pids = (int*) malloc(sizeof(int)*quantos);
	
	int pid;

	//alarm(tempoExec);
	//int i;
	//while((segundos<tempoExec) && (completa == 0)){


	if(quantos==1){


		

			
			if((pid=fork())==0){

				
			
			char * respostas[10];


			parsecomand(pedidos[0], respostas);

			sleep(10);
			
			execvp(respostas[0], respostas);



			

			_exit(1);

		}

		pids[0]=pid;

		

		}


	else{


	
	int pipe_fdd[quantos][2];

	for(int i=0; i<quantos; i++){
		
		if(i==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			

			if((pid=fork())==0){

				
			close(pipe_fdd[i][0]);
			
			dup2(pipe_fdd[i][1],1);
			
			close(pipe_fdd[i][1]);
			

		

			char * respostas[10];

			
			parsecomand(pedidos[i], respostas);
	
			

			execvp(respostas[0], respostas);

			printf("gil\n");

			_exit(1);

		}
		close(pipe_fdd[i][1]);
	

		}

		else if(i==quantos-1){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			close(pipe_fdd[i][1]);
			if((pid=fork())==0){

				
			//close(pipe_fdd[i][1]);

			
			dup2(pipe_fdd[i-1][0],0);
			
			close(pipe_fdd[i][0]);
			
			


			char * respostas[10];

			
			parsecomand(pedidos[i], respostas);

			
			execvp(respostas[0], respostas);


			_exit(2);

		}

		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][0]);
		

		}
		else{

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			if((pid=fork())==0){

				
			
			
			dup2(pipe_fdd[i-1][0],0);
			
			dup2(pipe_fdd[i][1],1);

			close(pipe_fdd[i][0]);
			
			close(pipe_fdd[i][1]);
			


			char * respostas[10];

			
			parsecomand(pedidos[i], respostas);

			

			execvp(respostas[0], respostas);


			_exit(1);

		}	

		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][1]);

		

		}

		pids[i]=pid;

		}
	}

	/*if(timeout){

			
		//while(segundos<tempoExec){
 
		
		//}
	
		for(int i=0; i<pids_count; i++){
		printf("killing grep %d\n", pids[i]);


		if(pids[i]>0){
			kill(pids[i],SIGKILL);
		}
	}

	printf("acabou o tempo de execucao\n");
		free(pids);
		return -1;	
}
*/

	for(int i=0;i<quantos;i++){
		wait(NULL);
	}

	return 0;

}



