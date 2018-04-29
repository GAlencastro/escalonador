#include <stdio.h>
#include <unistd.h>


int main(){
  int i = 2;
  for(;;){
    usleep(1000000);

    printf("Programa 2 - %d\n", i);
    i+= 2;
  }
  return 0;
}
