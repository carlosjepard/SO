#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"



int * pids;
int pids_count=0;
int timeout=0;
int segundos=0;
int completa = 0;

void sigalarm2_handler(int signum){

	write(1, "pipe anon nao anda da perna\n",28);
	printf("VOU MATAR ESTE PID %d\n",getpid() );
	kill(getpid(),SIGKILL);

}

void sigalarm_handler(int signum){


	for(int i=0; i<pids_count; i++){
		


		if(pids[i]>0){
			printf("killing grep %d\n", pids[i]);
			kill(pids[i],SIGKILL);
		}
		
		//write(1,"processo terminado\n",19);
	}

	printf("processo pai terminado %d\n",getpid());
		kill(getpid(),SIGKILL);


	
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



int executa (char args[],int tempoExec, int tempoInat){

	
			if(signal(SIGALRM, sigalarm_handler)==SIG_ERR){

						perror("signal sigalarm");	

						exit(3);

			}
	
	//printf("tempoExec= %d\n", tempoExec );

			alarm(tempoExec);

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

	pids = (int*) malloc(sizeof(int)*quantos);
	
	int pid;

	//alarm(tempoExec);
	//int i;
	//while((segundos<tempoExec) && (completa == 0)){

	//alarm(tempoExec);


	if(quantos==1){


		

			
			if((pid=fork())==0){

				
			
			char * respostas[10];


			parsecomand(pedidos[0], respostas);

			//sleep(20);
			
			execvp(respostas[0], respostas);


			_exit(1);

		}
		

		pids[0]=pid;

		

		}


	else{


	
	int pipe_fdd[quantos][2];

	for(int i=0; i<quantos; i++){
		write(1,"passou aqui\n", 12);
		
		if(i==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			

			if((pid=fork())==0){


			if(signal(SIGALRM, sigalarm2_handler)==SIG_ERR){

						perror("signal sigalarm2");	

						exit(3);

			}
			alarm(tempoInat);
				
			close(pipe_fdd[i][0]);
			
			dup2(pipe_fdd[i][1],1);
			
			close(pipe_fdd[i][1]);
			

			//sleep(10);
		

			char * respostas[10];

			
			parsecomand(pedidos[i], respostas);
	
			

			execvp(respostas[0], respostas);

			

			_exit(1);

		}
		
		close(pipe_fdd[i][1]);
		//pids[i]=pid;
		write(1,"i==0\n",5);	
	
			}

		else if(i==quantos-1){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			close(pipe_fdd[i][1]);
			if((pid=fork())==0){

			if(signal(SIGALRM, sigalarm2_handler)==SIG_ERR){

						perror("signal sigalarm2");	

						exit(3);

			}
			alarm(tempoInat);	
				
			//close(pipe_fdd[i][1]);

			
			dup2(pipe_fdd[i-1][0],0);
			
			close(pipe_fdd[i][0]);
			
			
			sleep(15);

			char * respostas[10];

			
			parsecomand(pedidos[i], respostas);

			//sleep(10);
			execvp(respostas[0], respostas);

			printf("gil\n");

			_exit(2);

		}
		
		
		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][0]);
		//pids[i]=pid;
		write(1,"i==i-1\n",7);

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
			alarm(tempoInat);
			
			
			dup2(pipe_fdd[i-1][0],0);
			
			dup2(pipe_fdd[i][1],1);

			close(pipe_fdd[i][0]);
			
			close(pipe_fdd[i][1]);
			


			char * respostas[10];

			//sleep(30);
			parsecomand(pedidos[i], respostas);

			

			execvp(respostas[0], respostas);


			_exit(1);

		}

			

		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][1]);
		write(1,"i==i\n",5);
		//pids[i]=pid;
		

		}

		pids[i]=pid;

		}

	}


	


	for(int i=0;i<quantos;i++){
		wait(NULL);
		printf("PID acabou%d\n", pids[i]);
	}

	write(1,"caralhoooo\n",11);

	return 0;

}



