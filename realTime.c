#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>

#define MAX_NAME 30

struct exec {
  char * name;
  int initTime;
  int duration;
  struct exec * next;
};
typedef struct exec Exec;

void realTime(Exec * execs, int n) {

  Exec * aux = execs;
    // preparando o vetor de processos
  int i;
  int pid = 1;
  int *pids;
  int segmento;

  segmento = shmget(IPC_PRIVATE, n*(sizeof(int)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  if(segmento == -1){
      printf("Erro segmento = %d\n", segmento);
      exit(0);
  }

  pids = (int *) shmat(segmento, 0, 0);

  for(i = 0; i < n; i++) {
    if(pid != 0){
			pid = fork();
			if(pid == 0){
				break;
			}
			if(pid != 0){
				pids[i] = pid;
			}
		}
  }
  //   // pai de todos
  //   // comandará o tempo para cada programa
  // if(pid != 0){
  //   for(i = 0; i< n; i++){
  //     	kill(pids[i], SIGSTOP);
  //   }
  //
  //   while(1){
  //     // pegando o tempo real
  //     sleep(1);
  //     time_t initialTime = time(0); // Get the system time
  //     struct tm * timeinfo;
  //
  //     timeinfo = localtime ( &initialTime );
  //
  //     int nowSeconds = timeinfo->tm_sec;
  //   // REMOVE FOR TEST  if(nowSeconds == 0){
  //     //  while(1){
  //         // after the first run, it will always start at second 0 of every minute
  //         int seconds = 0;
  //
  //         for(i = 0; i < n; i++){
  //           sleep(aux->initTime - seconds);
  //
  //           seconds = aux->initTime;
  //           kill(pids[i], SIGCONT);
  //
  //           sleep(aux->duration);
  //
  //
  //           seconds += aux->duration;
  //           kill(pids[i], SIGSTOP);
  //           aux = aux->next;
  //
  //           sleep(60 - seconds);
  //
  //         }
  //     //  }
  //   //  }
  //
  //   }
  //
  // }
  //
  //   // filhos
  //   // executarão os programas
  // if(pid == 0){
  //
	// 	int auxPid = getpid();
	// 	for(i = 0; i < n; i++){
	// 		if (auxPid == pids[i]){
  //
  //       char *args[]={aux->name,NULL};
	// 			execv(args[0], args);
	// 		}
  //     aux = aux->next;
	// 	}
	// }


  shmdt(pids);
  

}


int main () {

  int numberOfExecs = 3;
  int times[3][2] = {
    {10, 10},
    {25, 5},
    {31, 6},
  };
  char names[3][30] = { "prog1", "prog2", "prog3" };
  int i;

  Exec * realTimeExec ;

  Exec * prev = NULL;

  Exec * aux ;

  // CRIANDO LISTA CIRCULAR DE PROCESSOS.

  for(i = 0; i < numberOfExecs; i++){

      aux = (Exec *)malloc(numberOfExecs*sizeof(Exec));

      aux->name = (char *)malloc(MAX_NAME*sizeof(char));
      strcpy(aux->name, names[i]);
      aux->initTime = times[i][0];
      aux->duration = times[i][1];

      if(prev != NULL){
        prev->next = aux;
      }
      else{
        realTimeExec = aux;
      }

      prev = aux;
      aux = aux->next;
  }
  prev->next = realTimeExec;


  realTime(realTimeExec, i);

  while(realTimeExec->next != NULL){
    Exec *aux = realTimeExec;
    realTimeExec = realTimeExec->next;

    //free(aux->name);
  //  free(aux);
  }
  free(realTimeExec);

  return 0;
}
