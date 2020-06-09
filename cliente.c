#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define SIZE 1024

int main(int argc, char * argv[]){


	
	if(argc == 1){
		printf("Bem vindo\n");
		printf("Ira introduzir os pedidos por aqui\n");

		//do{
		//	switch()
		//}
	}

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