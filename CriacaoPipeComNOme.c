#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char * argv[]){

	mkfifo("FIFO",0666);
  // keeps reading from the named pipe
  	while(1) {
    fd = open(fifo,O_RDONLY);
    while((r = read(fd,&buffer,SIZE)) > 0) {
      write(1,&buffer,r);

	return 0;
}