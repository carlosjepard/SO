#include <string.h>
#include <stdio.h>

int parsecomand(char * comando, char * resultado[]){


	//char parvalheira[] = "parvalheira pessoal caralho";


	char * ptr = strtok(comando, " ");


	
	int i = 0;
	while(ptr != NULL)
	{	
		//printf("%s\n", ptr );	
		resultado[i++] = strdup(ptr);
		ptr = strtok(NULL, " ");
	}

	/*
	for(int j=0; j<i; j++){
		printf("resposta: %s\n", resultado[j] );
	}
*/
	resultado[i]=NULL;


}