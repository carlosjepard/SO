#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"



int parsecomand(char * comando, char * resultado[]){


	//char parvalheira[] = "parvalheira pessoal caralho";


	char * ptr = strtok(comando, " ");


	
	int i = 0;
	while(ptr != NULL)
	{	
		//printf("%s\n", ptr );	
		resultado[i++] = strdup(ptr);
		ptr = strtok(NULL, " ");
	}

	/*
	for(int j=0; j<i; j++){
		printf("resposta: %s\n", resultado[j] );
	}
*/
	resultado[i]=NULL;


}



int executa (char args[]){

	
	

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
	
	//printf("pedidos = %s\n", pedidos[0] );
	
	pid_t pid;

	
	
	if(quantos==1){

			
			if(fork()==0){

				
			
			char * respostas[10];

			
			parsecomand(pedidos[0], respostas);

			

			execvp(respostas[0], respostas);



			

			_exit(1);

		}

		wait(NULL);
		return 0;

		}


	
	int pipe_fdd[quantos][2];

	for(int i=0; i<quantos; i++){
		
		if(i==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}


			if(fork()==0){

				
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
			if(fork()==0){

				
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

			if(fork()==0){

				
			
			
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

		}

		
		
 
		for(int i=1; i<quantos;i++){

			wait(NULL);


			
		}

		

	return 0;
}


