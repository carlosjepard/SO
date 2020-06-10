#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define SIZE 1024
#define INPUT_BUFFER_SIZE (64)
#define QUIT_STRING "q"

int main(int argc, char * argv[]){

int fd;
char *buf=NULL;
char b;
char input[100];
	
	if(argc == 1){
		printf("Bem vindo\n");
		printf("Ira introduzir os pedidos por aqui\n");

		
    		char *command;
    		int i;

			do{

			while(read(0,&b,1)!=0 && b!='\n'){
				input[i]=b;
				i++;
			}
			input[i]='\0';
				
				command = strtok(input, " ");
							

			/*	if(strcmp(command,"tempo-inactividade")==0){ 

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					}

					strcpy(buf,"i");
					command=strtok(NULL,"\n");
					strcpy(buf+1, command);
					
					write(fd,buf,strlen(buf)+1);

					}
			

				
				else if(strcmp(command,"tempo-execucao")==0){

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					}

					strcpy(buf,"m");
					command=strtok(NULL,"\n");
					strcpy(buf+1, command);

					write(fd,buf,strlen(buf)+1);

					
					}
					*/
				
				 if(strcmp(command,"executar")==0){

					buf= malloc(1+strlen(input));



					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					} 

					strcpy(buf,"e");

					
					command=strtok(NULL,"\n");
					
					printf("222=%s\n", command );
					strcpy(buf+1, command);


					write(fd,buf,strlen(buf)+1);

					}

/*
				else if(strcmp(command,"listar\n")==0){

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					} 

					strcpy(buf,"l");
					

					write(fd,buf,strlen(buf)+1);

				}


				else if(strcmp(command,"terminar")==0){

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					} 

					strcpy(buf,"t");
					command=strtok(NULL,"\n");
					strcpy(buf+1, command);

					write(fd,buf,strlen(buf)+1);

				}
*/

				else if(strcmp(command,"historico")==0){

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					} 

					strcpy(buf,"r");
					

					write(fd,buf,strlen(buf)+1);

					}				

				else if(strcmp(command,"ajuda")==0){


					printf("AJUDAS\n");
					printf("Comandos:\n");
					printf("tempo-inactividade n (n=tempo maximo de inatividade num pipe anonimo)\n");
					printf("tempo-execucao n (n=tempo maximo de execucao de uma tarefa)\n");	
					printf("executar ´´p1 | p2 .. | pn´´ (executar tarefa)\n");
					printf("listar (listar tarefas em execução)\n");
					printf("terminar n (terminar tarefa n)\n");
					printf("historico (registo historico de tarefas terminadas\n");
					
				}

				

		}while ((command == NULL) || strcmp(command, QUIT_STRING) != 0);

		if (NULL != command)
    {
      printf("User quit.\n");
    }

  
	} else{

	int fd;

	int r=0;

	char * comando = argv[1]; 	
	
	char c = comando[1];

	char *buf=NULL;	
	
	int fifo;

	//mkfifo("FIFO",0666);

	switch(c){

		case('e'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("executar tarefas\n");
			
			buf = malloc(1+strlen(argv[2]));

			strcpy(buf,"e");
			strcpy(buf+1,argv[2]);

			//buf = argv[2];

			int bytes_read=0;



			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);
            
   			 //}

		break;

		case('h'):
			

			printf("AJUDAS\n");
			printf("Comandos:\n");
			printf("-i n (n=tempo maximo de inatividade num pipe anonimo)\n");
			printf("-m n (n=tempo maximo de execucao de uma tarefa)\n");	
			printf("-e ´´p1 | p2 .. | pn´´ (executar tarefa)\n");
			printf("-l (listar tarefas em execução)\n");
			printf("-t n (terminar tarefa n)\n");
			printf("-r (registo historico de tarefas terminadas\n");
			printf("-h (ajuda)\n");

			
		break;


		case('r'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("registos\n");
			
			buf = malloc(1);

			buf = "r";

			



			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);
            
   			 //}

		break;


		case('m'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("tempo maximo definido = %s \n", argv[2]);
			

			buf = malloc(1+strlen(argv[2]));

			strcpy(buf,"m");
			strcpy(buf+1,argv[2]);

			


			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);
            
   			 //}

		break;


		case('i'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("tempo de inatividade entre pipes anonimos = %s \n", argv[2]);
			

			buf = malloc(1+strlen(argv[2]));

			strcpy(buf,"i");
			strcpy(buf+1,argv[2]);

			


			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);
            
   			 //}

		break;

	
	
	
	}

}
	return 0;
}

/*
	while((r = read(fd,&buffer,SIZE)) > 0) {
      write(1,&buffer,r);
      sscanf(buffer,"%d\t%[^\n]s",&pid,command); // PID and command are OK
      // server handles at maximun 5 request simultaneously
      if(running < FORKS) {
        running++;
        if(fork() == 0) {
          // creates a child process to process request
          processRequest(pid,command);
          _exit(1);
        } else if(running >= FORKS) {
          wait(0);
          running--;
        }
      }

    }
    close(fd);
  }
  return 0;
}
*/