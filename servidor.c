#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "header.h"

#define SIZE 1024
#define DEFAULT 100


int ter=0;
int exe=0;
int tempoExec=DEFAULT;
int tempoInat=DEFAULT;
int minhaVez=0;
int bytesEscritos=0;


//registo.terminadas= malloc(20*sizeof(char*));
//registo.execucao=malloc(20*sizeof(char*));





int main(int argc, char * argv[]){

	
	memoria registo;

	registo.terminadas= malloc(20*sizeof(char*));
	registo.execucao=malloc(20*sizeof(char*));


	int fifo_fd=-1;

	int fd;
	mkfifo("FIFO",0666);
  	// keeps reading from the named pipe
  	
	int r;

	printf("open esta a andar da perna\n");

	char buffer[1024];
	int bytes=0;

	int servpid;

	printf("palhacada\n");

	int fw=open("logs.txt",O_WRONLY|O_CREAT|O_TRUNC,0600);
	int fr=open("logs.txt",O_RDONLY,0600);

	while(1) {



    fd = open("FIFO",O_RDONLY);
    printf("merdas\n");
    while((r = read(fd,buffer,SIZE)) > 0) {

    			char * guarda = malloc(sizeof(strlen(buffer))-1);
    			int bytesaff = strlen(buffer);

    			if(buffer[0]=='m'){
    				strcpy(guarda,buffer+1);

    				tempoExec=atoi(guarda);

    				printf("tempo de exec= %d\n", tempoExec);

    			}
    			else if(buffer[0]=='i'){
    				strcpy(guarda,buffer+1);

    				tempoInat=atoi(guarda);

    				printf("tempo de Inatividade entre pipes anonimos= %d\n", tempoInat);
    			}

		    	if(fork()==0){

    			

    			int pid;

    			int status;
    			
					    			

    			

    	switch(buffer[0]){

    		//executar tarefa
    		case('e'):

    			

    			strcpy(guarda,buffer+1);
    			strcat(guarda, "\n");

    			if((pid=fork())==0){
      	
      			write(1,buffer,r);

            write(1,"para de printar merda crl\n", 30);
      			//printf("pid filho= %d\n", pid );

      			//printf("\n");
		
					//char ** pedidos = malloc(4*sizeof(char*));

      			//printf("tempoExec novo =%d\n", tempoExec );
      			
      			
      			int z=  executa(buffer+1, tempoExec, tempoInat);
      			//sleep(10);

      			_exit(0);

      		}

      			
      			else{
      			

      			//printf("pid = %d\n", pid );
				
      			wait(NULL);

      			
				printf("pidzao = %d\n", pid );

				while(minhaVez==1){
					sleep(1);
				}

				minhaVez=1;
				bytesEscritos=write(fw,guarda,strlen(guarda));
				printf("bytesEscritos filho = %d\n", bytesEscritos);
				minhaVez=0;

		}

    			break;


    		case('r'):

    		printf("REGISTOS DE TAREFAS CONCLUDIAS\n");

    			if(ter==0){
    				printf("Nao existem registos de tarefas concluidas\n");
    				
    			}

    			else{

    			while(minhaVez==1){
    				sleep(1);
    			}
    				minhaVez=1;
    				char * res = (char*) malloc(sizeof(100));

    				while(read(fr,res,100)>0){
    					write(1,res,strlen(res));
    				}

    				lseek(fr,0,SEEK_SET);


    			}	

    			break;


    		case('m'):



    			

    			
      	
      			


    			break;


    		case('i'):



    			

    			
      	

    			break;
      		

      			



    		default:

    			printf("nao percebi o comando\n");
    			break;
   		
   			}

   		
   				_exit(0);
   		}


   			ter++;
   			
   			
		}
		


	}
	
	
	
	return 0;
}

