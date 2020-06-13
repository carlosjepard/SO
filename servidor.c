#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "header.h"

#define SIZE 1024
#define DEFAULT 100


int ter=0;
int exe=0;
int tempoExec=DEFAULT;
int tempoInat=DEFAULT;
int minhaVez=0;
int bytesEscritos=0;
char** execucao=NULL;
//memoria registo=NULL;

void hdl(int sig, siginfo_t * siginfo, void *context){


    int j=0;

    char * pidString = malloc(sizeof(char*));
    int value = siginfo->si_pid;
    snprintf(pidString, 10, "%d", value);


  for(j;j<exe;j++){

    if(strstr(execucao[j],pidString) != NULL) {
      execucao[j]='\0';
      break;
  }
    
}

  for(int i=j;i<exe;i++){
    execucao[i]=execucao[i+1];
  }

  exe--;
  

  

  
}



//registo.terminadas= malloc(20*sizeof(char*));
//registo.execucao=malloc(20*sizeof(char*));





int main(int argc, char * argv[]){

  

  struct sigaction act;
 
  memset (&act, '\0', sizeof(act));
 
  /* Use the sa_sigaction field because the handles has two additional parameters */
  act.sa_sigaction = &hdl;
 
  /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
  act.sa_flags = SA_SIGINFO;
 

  if(sigaction(SIGUSR1, &act,NULL)<0){

            perror("signal error");  

            exit(3);

      }

	
	

	execucao=malloc(24*sizeof(char*));

  //char * teste[100];

	int fifo_fd=-1;



	int fd;
	mkfifo("FIFO",0666);
  

  //ESTE E O PIPE PARA ESCREVER PARA O CLIENTE
  mkfifo("FIFO2",0666);


  	// keeps reading from the named pipe
  	
	int r;

	printf("open esta a andar da perna\n");

	char buffer[1024];
	int bytes=0;

	int servpid = getpid();

	printf("palhacada\n");

	int fw=open("logs.txt",O_WRONLY|O_CREAT|O_TRUNC,0600);
	int fr=open("logs.txt",O_RDONLY,0600);



	while(1) {



    fd = open("FIFO",O_RDONLY);
    printf("merdas\n");
    while((r = read(fd,buffer,SIZE)) > 0) {

    			char * guarda = malloc(sizeof(strlen(buffer))-1);
    			int bytesaff = strlen(buffer);

          //strcpy(teste[0],buffer);

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

          int pipe_paiFilho[2];

          if(pipe(pipe_paiFilho)<0){
                perror("pipe");
                exit(1);
            }


		    	if(fork()==0){

        close(pipe_paiFilho[0]);

    			int pid;

    			int status;
    			
					    			

    			

    	switch(buffer[0]){

    		//executar tarefa
    		case('e'):

    			

    			strcpy(guarda,buffer+1);

          char  TAREFAPID[1024];


          strcpy(TAREFAPID,guarda);


          strcat(TAREFAPID, " " );

          strcat(TAREFAPID, "PID-");
          
          strcat(guarda, "\n");




          

    			if((pid=fork())==0){

            char * bufferAA = malloc(sizeof(char*));
            int value = getpid();
            snprintf(bufferAA, 10, "%d", value);

            strcat(TAREFAPID,bufferAA);

            
      	
      			write(pipe_paiFilho[1],TAREFAPID,strlen(TAREFAPID));

           
           
            close(pipe_paiFilho[1]);


            
      			int z=  executa(buffer+1, tempoExec, tempoInat);

            kill(servpid,SIGUSR1);

      			if(z==0){ write(1,"acabou com 0\n",13); _exit(1);} //normal
            if(z==2){ write(1,"acabou com 2\n",13); _exit(2);} //pipes anonumos tempo
            if(z==3){ write(1,"acabou com 3\n",13); _exit(3);}

            
      			_exit(0); //tempo de exec acabou

      		}

      			
      			else{
      			
            close(pipe_paiFilho[1]);

      			//printf("pid = %d\n", pid );
				  int status;
          wait(&status);

          

          //printf("WEXITSTATUS = %d\n", WEXITSTATUS(status));
          int escrever = WEXITSTATUS(status);

      			
				    printf("pidzao = %d\n", pid );

				    while(minhaVez==1){
				    	sleep(1);
			     	}

				
        if(escrever==2){
          char esc[bytesaff + 22];
          strcpy(esc,"TEMPO DE INATIVIDADE: ");
          strcat(esc,guarda);
          bytesEscritos=write(fw,esc,strlen(esc));          
        }
        if(escrever==0){
          char esc[bytesaff + 19];
          strcpy(esc,"TEMPO DE EXECUCAO: ");
          strcat(esc,guarda);
          bytesEscritos=write(fw,esc,strlen(esc)); 
        }
        if(escrever==1){
          char esc[bytesaff + 23];
          strcpy(esc,"CONCLUIDO COM SUCESSO: ");
          strcat(esc,guarda);
          bytesEscritos=write(fw,esc,strlen(esc));
        }

				minhaVez=0;

		}

    			break;


    		case('r'):



        
        close(pipe_paiFilho[1]);

      

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
            int ler;
    				while((ler=read(fr,res,100))>0){
              //AQUI TENS DE ESCREVER PARA O CLIENTE TMB
    					write(1,res,ler);
    				}

    				lseek(fr,0,SEEK_SET);


    			}	

    			break;


    		case('m'):

          close(pipe_paiFilho[1]);

    			

    			
      	
      			


    			break;


    		case('i'):


        close(pipe_paiFilho[1]);

    			

    			
      	

    			break;


          case('l'):



        
        close(pipe_paiFilho[1]);

      

        printf("Tarefas em execucao\n");

          if(exe==0){
            printf("Nao existem tarefas em execucao\n");
            
          }

          else{

          while(minhaVez==1){
            sleep(1);
          }
            
            for(int i=0;i<exe;i++){
              //AQUI TENS DE ESCREVER PARA O CLIENTE TMB
              printf("%d: %s\n",i+1,execucao[i]);
            }

          } 

          break;



          case('t'):



        
          close(pipe_paiFilho[1]);

          
        

          kill(atoi(guarda),SIGUSR1); 

          break;
      		

      			



    		default:

          close(pipe_paiFilho[1]);

          //AQUI TENS DE ESCREVER PARA O CLIENTE TMB MAS E CAGATIVO. 
    			printf("nao percebi o comando\n");
    			break;
   		
   			}

   		
   				_exit(0);
   		}

      close(pipe_paiFilho[1]);

      if(buffer[0]=='e'){

          char * tarefa = (char*) malloc(100) ;
          char b;
          int i=0;
         while((r = read(pipe_paiFilho[0],&b,1)) > 0){

            tarefa[i]=b;
            i++;
      } 


          char final[i];

          strcpy(final,tarefa);


         
         

          execucao[exe]=strdup(final);

          exe++;
         

        close(pipe_paiFilho[0]);

        

    }
    else{
      close(pipe_paiFilho[0]);
    }

     ter++;
		}
		


	}
	
	
	
	return 0;
}

