#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "header.h"

#define SIZE 1024

int ter=0;
int exe=0;


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


	while(1) {
    fd = open("FIFO",O_RDONLY);
    printf("merdas\n");
    while((r = read(fd,&buffer,SIZE)) > 0) {

    	switch(buffer[0]){

    		//executar tarefa
    		case('e'):
      	
      			write(1,&buffer,r);

      			printf("\n");

      			
      			char * guarda = malloc(sizeof(strlen(buffer))-1);

      			strcpy(guarda,buffer+1);




      			int z=  executa(buffer+1);

      		
      			if(z==0){

    			registo.terminadas[ter++]=guarda;


    			
}		
    				
    				//registo.execucao[exe--]

    				


    			break;


    		case('r'):

    		printf("REGISTOS DE TAREFAS CONCLUDIAS\n");

    			if(ter==0){
    				printf("Nao existem registos de tarefas concluidas\n");
    			}

    			else{

    			for(int i=0;i<ter;i++){
    				printf("%d: %s\n", i,registo.terminadas[i]);
    				}

    			}	

    			break;



    		default:

    			printf("nao percebi o comando\n");
    			break;
   		
   		}


		}

	}
	
	
	
	return 0;
}

