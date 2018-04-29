#include <stdio.h>
#include <unistd.h>


int main(){
  int i = 2;
  for(;;){
    usleep(1000000);

    printf("Programa 2 - %d -- PID: %d\n", i, getpid());
    i+= 2;
  }
  return 0;
}
