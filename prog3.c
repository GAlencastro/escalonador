#include <stdio.h>
#include <unistd.h>


int main(){
  int i = 3;
  for(;;){
    usleep(1000000);
    printf("Programa 3 - %d\n", i);
    i+= 3;
  }
  return 0;
}
