#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>



#define SIZE 1024
#define INPUT_BUFFER_SIZE (64)
#define QUIT_STRING "q"


int main(int argc, char * argv[]){

	int fd;
	int fd2;
	int r;

	char *buf=NULL;	
	char b;
	char input[100];
	char buffer[1024];

//O SERVIDOR QUANDO COMECA ABRE UM PIPE PARA OUVIR E OUTRO PARA ESCREVER. TENS DE METER O CLIENTE A OUVIR O SERVIDOOR. PODES TER DE USAR FORKS, MAS N SEI
// executar cut -f7 -d: /etc/passwd | uniq | wc -listar
//o zat?	

	
		

	if(argc == 1){
		printf("Bem vindo\n");
		printf("Ira introduzir os pedidos por aqui (q para sair)\n");


  		
    		char *command = (char*) malloc(1024) ;
    		int i;



    		if(fork()==0){

    			fd2 = open("FIFO2",O_RDONLY);

    			char * resp = (char*) malloc(sizeof(1024));

    			while(1){


					

						while((r = read(fd2,resp,1024)) > 0) {

							write(1,resp,r);
						
					}


    			}

    		}

			do{
				i=0;

			while((read(0,&b,1)>0) && b!='\n'){
				input[i]=b;
				i++;
			}
			input[i]='\0';
				
				command = NULL;

				write(1,"passou aqui\n",12);
				command = strtok(input, " ");
				write(1,command,strlen(input));
				write(1,"\n",1);
							

				if(strcmp(command,"tempo-inactividade")==0){ 

					buf= malloc(1+strlen(input));

					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					}

					strcpy(buf,"i");
					command=strtok(NULL,"\n");
					
					strcpy(buf+1, command);

					write(1,"pedrinha\n",9);
					
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
					
				
				 else if(strcmp(command,"executar")==0){

				 	

					buf= malloc(1+strlen(input));



					if((fd=open("FIFO",O_WRONLY))<0){
					perror("open");
					} 

					strcpy(buf,"e");

					
					command=strtok(NULL,"\n");
					
					
					strcpy(buf+1, command);


					write(fd,buf,strlen(buf)+1);

					
					

					
					

					//wait(NULL);

				}



				else if(strcmp(command,"listar")==0){

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
				else{
					printf("comando desconhecido\n");
				}

				

		}while (strcmp(command, QUIT_STRING) != 0);

		if (NULL != command)
    {
      printf("User quit.\n");
    }

  
	

}else{


	int r=0;

	char * comando = argv[1]; 	
	
	char c = comando[1];

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



     	 		fd2 = open("FIFO2",O_RDONLY);
						while((r = read(fd2,buffer,SIZE)) > 0) {

							write(1,buffer,strlen(buffer)+1);
						
					}
            
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

     	 		fd2 = open("FIFO2",O_RDONLY);
				
						while((r = read(fd2,buffer,SIZE)) > 0) {

							write(1,buffer,strlen(buffer)+1);
						
					}
            
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

		case('l'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("Tarefas em exec\n");
			
			buf = malloc(1);

			buf = "l";

			



			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);

     	 		fd2 = open("FIFO2",O_RDONLY);
						while((r = read(fd2,buffer,SIZE)) > 0) {

							write(1,buffer,strlen(buffer)+1);
						
					}
            
   			 //}

		break;

		case('t'):
			

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}

			printf("Terminar Tarefa %s\n", argv[2]);
			
			buf = malloc(1+strlen(argv[2]));

			strcpy(buf,"t");
			strcat(buf,argv[2]);

			printf("%s\n", buf);



			//while((r = read(0,buf,SIZE)) > 0) {
     	 		
     	 		write(fd,buf,strlen(buf)+1);
            
   			 //}

		break;

		

	

		}
	}
	return 0;
}

