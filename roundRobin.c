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



void roundRobin(Exec * execs, int n) {

  Exec * aux = execs;
    // preparando o vetor de processos
  int i;
  int pid = 1;
  int *pids;
  int segmento;

  segmento = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

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
    // pai de todos
    // comandará o tempo para cada programa
  if(pid != 0){
    for(i = 0; i< n; i++){
        kill(pids[i], SIGSTOP);
    }
    while(1){
			for(i = 0; i < n; i++){

					kill(pids[i], SIGCONT);
					usleep(500000);
					kill(pids[i], SIGSTOP);

			}
    }
  }

    // filhos
    // executarão os programas
  if(pid == 0){

		int auxPid = getpid();
		for(i = 0; i < n; i++){
			if (auxPid == pids[i]){

        char *args[]={aux->name,NULL};
				execv(args[0], args);
			}
      aux = aux->next;
		}
	}




}

//
// int main () {
//
//   int numberOfExecs = 3;
//   int times[3][2] = {
//     {10, 10},
//     {25, 5},
//     {31, 6},
//   };
//   char names[3][30] = { "prog1", "prog2", "prog3" };
//   int i;
//
//   Exec * realTimeExec ;
//
//   Exec * prev = NULL;
//
//   Exec * aux ;
//
//   // CRIANDO LISTA CIRCULAR DE PROCESSOS.
//
//   for(i = 0; i < numberOfExecs; i++){
//
//       aux = (Exec *)malloc(numberOfExecs*sizeof(Exec));
//
//       aux->name = (char *)malloc(MAX_NAME*sizeof(char));
//       strcpy(aux->name, names[i]);
//       aux->initTime = times[i][0];
//       aux->duration = times[i][1];
//
//       if(prev != NULL){
//         prev->next = aux;
//       }
//       else{
//         realTimeExec = aux;
//       }
//
//       prev = aux;
//       aux = aux->next;
//   }
//   prev->next = realTimeExec;
//
//
//
//   roundRobin(realTimeExec, i);
//   //
//   // while(1){
//   //   time_t nowTime = time(0);
//   //   // if(initialTime){
//   //   //   break;
//   //   // }
//   // }
//   return 0;
// }
