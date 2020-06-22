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
	int fd3;
	char *buf=NULL;	
	char b;
	char input[100];
	char buffer[1024];

	if(argc == 1){
		write(1,"Bem vindo\n",10);
		write(1,"Ira introduzir os pedidos por aqui (q para sair)\n",49);

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
				command = strtok(input, " ");

				if(strcmp(command,"tempo-inactividade")==0){ 

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
					
				 else if(strcmp(command,"executar")==0){

					buf= malloc(1+strlen(input));
					if((fd=open("FIFO",O_WRONLY))<0){
						perror("open");
					} 
					strcpy(buf,"e");
					command=strtok(NULL,"\n");
					strcpy(buf+1, command);
					write(fd,buf,strlen(buf)+1);
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

					write(1,"AJUDAS\n",7);
					write(1,"Comandos:\n",10);
					write(1,"tempo-inactividade n (n=tempo maximo de inatividade num pipe anonimo)\n",70);
					write(1,"tempo-execucao n (n=tempo maximo de execucao de uma tarefa)\n",60);	
					write(1,"executar p1 | p2 .. | pn (executar tarefa)\n",43);
					write(1,"listar (listar tarefas em execução)\n",36);
					write(1,"terminar n (terminar tarefa n)\n",31);
					write(1,"historico (registo historico de tarefas terminadas\n",51);	
				}
				else{
					write(1,"Comando desconhecido\n",21);
				}	
		}while (strcmp(command, QUIT_STRING) != 0);

		if (NULL != command)
    {
      write(1,"User quit.\n",11);
    }	

	}else{

	int r=0;
	char * comando = argv[1]; 	
	char c = comando[1];
	int fifo;

	switch(c){

		case('e'):
			
			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"executar tarefas\n",17);
			buf = malloc(1+strlen(argv[2]));
			strcpy(buf,"e");
			strcpy(buf+1,argv[2]);
			int bytes_read=0;	
     	 	write(fd,buf,strlen(buf)+1);
     	 	fd2 = open("FIFO2",O_RDONLY);
				while((r = read(fd2,buffer,SIZE)) > 0) {
					write(1,buffer,r);	
					}
			break;

		case('h'):
			
			write(1,"AJUDAS\n",7);
			write(1,"Comandos:\n",10);
			write(1,"tempo-inactividade n (n=tempo maximo de inatividade num pipe anonimo)\n",70);
			write(1,"tempo-execucao n (n=tempo maximo de execucao de uma tarefa)\n",60);	
			write(1,"executar p1 | p2 .. | pn (executar tarefa)\n",43);
			write(1,"listar (listar tarefas em execução)\n",36);
			write(1,"terminar n (terminar tarefa n)\n",31);
			write(1,"historico (registo historico de tarefas terminadas\n",51);
			break;

		case('r'):

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"registos\n",9);
			buf = malloc(1);
			buf = "r";
     	 	write(fd,buf,strlen(buf)+1);
     	 	fd2 = open("FIFO2",O_RDONLY);
				while((r = read(fd2,buffer,SIZE)) > 0) {
					write(1,buffer,r);	
					}
			break;

		case('m'):

			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"tempo maximo definido\n",22);
			buf = malloc(1+strlen(argv[2]));
			strcpy(buf,"m");
			strcpy(buf+1,argv[2]);
     	 	write(fd,buf,strlen(buf)+1);
			break;

		case('i'):
			
			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"tempo de inatividade entre pipes anonimos definido\n",51);
			buf = malloc(1+strlen(argv[2]));
			strcpy(buf,"i");
			strcpy(buf+1,argv[2]);	
     	 		write(fd,buf,strlen(buf)+1);
			break;

		case('l'):
			
			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"Tarefas em execucao\n",20);
			buf = malloc(1);
			buf = "l";
     	 	write(fd,buf,strlen(buf)+1);
     	 	fd2 = open("FIFO2",O_RDONLY);
				while((r = read(fd2,buffer,SIZE)) > 0) {
					write(1,buffer,r);	
					}
			break;

		case('t'):
			
			if((fd=open("FIFO",O_WRONLY))<0){
				perror("open");
			}
			write(1,"Terminando Tarefa\n",18);
			buf = malloc(1+strlen(argv[2]));
			strcpy(buf,"t");
			strcat(buf,argv[2]);
     	 	write(fd,buf,strlen(buf)+1);
			break;
		}
	}
	return 0;
}

