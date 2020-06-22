CC = gcc
CFLAGS = -Wall -g -Itemp


all: argus argusd
argus: ./argus.c 
	cc ./argus.c ./argus.h -o argus
argusd: ./executaTarefa.c ./argusd.c
	cc ./executaTarefa.c ./argusd.c ./argus.h -o argusd
clean:
	rm -rf *.o argus argusd FIFO FIFO2 *.txt