#include <stdio.h>
#include <unistd.h>

int main(){

  int i = 1;
  for(;;){
    usleep(1000000);
    printf("Programa 1 - %d -- PID: %d\n", i, getpid());
    i++;
  }

  return 0;
}
