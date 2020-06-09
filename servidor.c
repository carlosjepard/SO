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


	while(1) {



    fd = open("FIFO",O_RDONLY);
    printf("merdas\n");
    while((r = read(fd,&buffer,SIZE)) > 0) {

		    	if(fork()==0){

    			char * guarda = malloc(sizeof(strlen(buffer))-1);

    			int pid;

    			int status;
    			
					    			

    			

    	switch(buffer[0]){

    		//executar tarefa
    		case('e'):

    			

    			strcpy(guarda,buffer+1);

    			if((pid=fork())==0){
      	
      			write(1,&buffer,r);

      			//printf("pid filho= %d\n", pid );

      			printf("\n");

      		

      			int z=  executa(buffer+1, tempoInat, tempoExec);
      			//sleep(10);

      			_exit(0);

      		}

      			
      			else{
      			

      			//printf("pid = %d\n", pid );
				
      			wait(NULL);

      			
				printf("pidzao = %d\n", pid );
				registo.terminadas[ter++]=guarda;
      			

		}

    			break;


    		case('r'):

    		printf("REGISTOS DE TAREFAS CONCLUDIAS\n");

    			if(ter==0){
    				printf("Nao existem registos de tarefas concluidas\n");
    			}

    			else{

    			for(int i=0;i<ter;i++){
    				printf("%d: %s\n", i+1,registo.terminadas[i]);
    				}

    			}	

    			break;


    		case('m'):



    			strcpy(guarda,buffer+1);

    			tempoExec=atoi(guarda);

    			printf("tempo de exec= %d\n", tempoExec);
      	
      			


    			break;


    		case('i'):



    			strcpy(guarda,buffer+1);

    			tempoInat=atoi(guarda);

    			printf("tempo de Inatividade entre pipes anonimos= %d\n", tempoInat);
      	

    			break;
      		

      			



    		default:

    			printf("nao percebi o comando\n");
    			break;
   		
   			}

   				//_exit(0);
   			//}

   			//else{
   				//wait(NULL);
   				//printf("pedido efetuado\n");
   			//}
   				_exit(0);
   		}
   			else{
			printf("pisa\n");

		}
   			
		}
		


	}
	
	
	
	return 0;
}

