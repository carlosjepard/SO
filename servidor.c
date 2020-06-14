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


void signalHandler(int signal)
{
  printf("Cought signal %d!\n",signal);
  wait(NULL);
  
}

void signalIGNORA(int signal)
{
  write(1,"nada\n",5);
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

  if(signal(SIGCHLD, signalHandler)==SIG_ERR){
    perror("signal SIGCHLDERRO");  
    exit(3);
    }

	execucao=malloc(24*sizeof(char*));
	int fifo_fd=-1;
  int fd2;
	int fd;
	mkfifo("FIFO",0666);
  mkfifo("FIFO2",0666);
	int r;
	char buffer[1024];
	int bytes=0;
	int servpid = getpid();
	int fw=open("logs.txt",O_WRONLY|O_CREAT|O_TRUNC,0600);
	int fr=open("logs.txt",O_RDONLY,0600);
  char *aux;
  char *str1;

	while(1) {
    fd = open("FIFO",O_RDONLY);
    while((r = read(fd,buffer,SIZE)) > 0) {
    			char * guarda = malloc(sizeof(strlen(buffer))-1);
    			int bytesaff = strlen(buffer);

    			if(buffer[0]=='m'){
    				strcpy(guarda,buffer+1);
    				tempoExec=atoi(guarda);
    				write(1,"tempo de execucao definido\n",28);
    			}

    			else if(buffer[0]=='i'){
    				strcpy(guarda,buffer+1);
    				tempoInat=atoi(guarda);
    				write(1,"tempo de Inatividade entre pipes anonimos definido\n",51);
    			}

          int pipe_paiFilho[2];
          if(pipe(pipe_paiFilho)<0){
              perror("pipe");
              exit(1);
            }

		    	if(fork()==0){
            if(signal(SIGCHLD, signalIGNORA)==SIG_ERR){
              perror("signal sigalarm");  
              exit(3);
            }

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
      			int z=  executa(buffer+1, tempoExec, tempoInat,servpid);
            kill(servpid,SIGUSR1);
      			if(z==0){ write(1,"saiu com 0\n",11); _exit(1);} //normal
            if(z==2){ write(1,"saiu com 2\n",11); _exit(2);} //pipes anonumos tempo
      			_exit(0); //tempo de exec acabou
      		  }

      			else{
            close(pipe_paiFilho[1]);
				    int status;
            wait(&status);
            int escrever = WEXITSTATUS(status);
				    while(minhaVez==1){
				    	sleep(1);
			     	}

				      minhaVez=1;

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
              write(1,"chegou\n",7);
              bytesEscritos=write(fw,esc,strlen(esc));
            }

				    minhaVez=0;
		    }
    		break;

    		case('r'):
        
        close(pipe_paiFilho[1]);
    			if(ter==0){

            if((fd2=open("FIFO2",O_WRONLY))<0){
                perror("open");
              }
              aux="Nao existem registos de tarefas concluidas\n";
              write(fd2,aux,strlen(aux)+1);
    			}
    			else{
    				
    				char * res = (char*) malloc(sizeof(100));
            int ler;
            if((fd2=open("FIFO2",O_WRONLY))<0){
                perror("open");
              }
    				while((ler=read(fr,res,1000))>0){
              write(fd2,res,ler);
            }  
            close(fd2);
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

          if(exe==0){
             aux="Nao existem tarefas em execucao\n";
            if((fd2=open("FIFO2",O_WRONLY))<0){
                perror("open");
              }
              write(fd2,aux,strlen(aux)+1);
          }
          else{

          while(minhaVez==1){
            sleep(1);
          }
            
          for(int i=0;i<exe;i++){
            int length = snprintf( NULL, 0, "%d", i+1 );
            str1 = malloc( length + 1 );
            snprintf(str1, length + 1, "%d", i+1 );
            strcat(str1,": ");
            strcat(str1,execucao[i]);
            strcat(str1,"\n");

            if((fd2=open("FIFO2",O_WRONLY))<0){
                perror("open");
              }

            write(fd2,str1,strlen(str1)+1);
            close(fd2);
            } 
          } 
          break;

        case('t'):

          close(pipe_paiFilho[1]);
          kill(atoi(guarda),SIGUSR1); 
          break;
      		
    		default:

          close(pipe_paiFilho[1]);
    			aux="nao percebi o comando\n";
           if((fd2=open("FIFO2",O_WRONLY))<0){
                perror("open");
              }
          write(fd2,aux,strlen(aux)+1);
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

