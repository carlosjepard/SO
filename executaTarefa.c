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

void sigalarm2_handler(int signum){

	
	for(int i=0; i<pids_count; i++){
		
		

		if(pids[i]>0){
			printf("killing grep %d\n", pids[i]);
			kill(pids[i],SIGKILL);
		}
		_exit(3);
		
		
	}



}

void sigalarm_handler(int signum){

	

	for(int i=0; i<pids_count; i++){
		


		if(pids[i]>0){
			printf("killing grep %d\n", pids[i]);
			kill(pids[i],SIGKILL);
		}
		
		write(1,"processo terminado\n",19);
	}

	printf("processo pai terminado %d\n",getpid());
	ret=1;
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



int executa (char * args,int tempoExec, int tempoInat){

	//pai
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

	//printf("quantos2 = %d\n", quantos2 );
	
	char * ptr = strtok(args,"|");

	int quantos=0;

	//write(1,"pisa\n", 5);

	//DA MERDA AQUI

	char **pedidos=malloc(quantos2*sizeof(char*));





	printf("merda 2\n");

	while(ptr != NULL)
	{	
		//printf("merda\n");
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

			//sleep(20);
			
			execvp(respostas[0], respostas);

			close(fd2);

			_exit(1);

		}
		

		pids[0]=pid;

		

		}


	else{


	quantos += (quantos -1);
	pids_count=quantos;
	pids = (int*) malloc(sizeof(int)*quantos);
	int pipe_fdd[quantos-1][2];
	
	for(int i=0; i<quantos;i++ ){
		//write(1,"passou aqui\n", 12);
		
		if(i==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			
			j++;
			//close(pipe_fdd[i][0]);
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
		//pids[i]=pid;
		//write(1,"i==0\n",5);	
	
			}

		else if(i==quantos-1){

			/*if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}
			*/

			close(pipe_fdd[i][1]);
			j++;
			if((pid=fork())==0){

			
			//close(pipe_fdd[i][1]);

			
			dup2(pipe_fdd[i-1][0],0);
			
			close(pipe_fdd[i-1][0]);
			
			//int fd2=open("FIFO2",O_WRONLY);

			//dup2(fd2,1);
			
			//sleep(15);

			char * respostas[10];

			
			parsecomand(pedidos[j], respostas);

			//sleep(10);


			//ESTE E O ULTIMO EXEC DE QUALQUER TAREFA COM PIPES. TENS DE REDIRECIONAR O STDOUT(1) PARA O EXTREMO DE ESCRITA DO PIPE COM NOME ->FIFO2
			//TENS DE FAZER UM DUP IGUAL AO QUE FIZ NA LINHA 264 MAS DESTE GENERO( dup2(extremo de escrita,1) )
			

			execvp(respostas[0], respostas);

//			close(fd2);

			//printf("gil\n");

			_exit(2);

		}
		
		
		close(pipe_fdd[i-1][0]);
		//close(pipe_fdd[i][0]);
		//pids[i]=pid;
		//write(1,"i==i-1\n",7);

		}
		else if (i%2==0){

			if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

			//if(i%2==0){
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
		//write(1,"i==i\n",5);
		//pids[i]=pid;
		

		}

			else{

				if(pipe(pipe_fdd[i])<0){
				perror("pipe");
				exit(1);
			}

				if((pid=fork())==0){



				/*if(signal(SIGALRM, sigalarm2_handler)==SIG_ERR){

						perror("signal sigalarm2");	

						exit(3);

					}
*/
					//alarm(tempoInat);
					//printf("tempo inat = %d\n", tempoInat);

			dup2(pipe_fdd[i-1][0],0);
			
			dup2(pipe_fdd[i][1],1);

			close(pipe_fdd[i-1][0]);
			
			close(pipe_fdd[i][1]);	
			
			int bytes_read;
			char zz[100];

			
			alarm(tempoInat);

			//sleep(10);

			while((bytes_read=read(0,zz,100))>0){
				write(1,zz,bytes_read);
				alarm(tempoInat);
			
			}

				//write(1,zz,bytes_read);


			_exit(1);

		}

		close(pipe_fdd[i-1][0]);
		close(pipe_fdd[i][1]);
		//write(1,"verificacao\n",12);
		
		//pids[i]=pid;
		





			}
			
		

		pids[i]=pid;

		}

	}


	printf("j = %d\n", j);

	int status;

	for(int i=0;i<quantos;i++){
		wait(&status);
		if(WIFEXITED(status)==0) ret=2;
		printf("PID acabou%d\n", pids[i]);
	}

	write(1,"caralhoooo\n",11);


	if(zat==1){
		write(1,"zat=1\n",6);
		return 3;

	} 

	return ret;

}


