#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

struct processo{
	char NamePrograma[20];
	int Prioridade;
	int Pid;
	int StatusProcesso;
};
typedef struct processo Processo;

int CreateProcess(int pid,char *Programa);
int CreateProgram(int numerodoPrograma);
int Interpreter(FILE *fp1, FILE *fp2, Processo v[], int *numProgramas);

int main (){
	Processo *v;
	int *nProg,*status,pid;
	int seg1, seg2, seg3;
	int i, retornoInterp,k,j;
	int ptemp, indice = 0, pid2;

	FILE *in = fopen("entrada.txt","r");
	FILE *out = fopen("saida.txt","w");

	seg1 = shmget(IPC_PRIVATE, sizeof(Processo)*8, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	v = (Processo *)shmat(seg1, 0, 0);

	seg2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	nProg = (int *)shmat(seg2, 0, 0);

	seg3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	status = (int *)shmat(seg3, 0, 0);

	*nProg = *status = 0;

	if(in == NULL){
		printf("Erro no arquivo de entrada\n");
	}

	if (out == NULL){
		printf("Erro no arquivo de saida\n");
	}

	for(i = 0; i < 9; i++){
		v[i].Pid = 1;
		v[i].StatusProcesso = 0;
	}
	pid = fork();

	if (pid != 0){							/* Escalonador  */
		while(nProg != 0){
			int retornoCriaçãoProcesso;
			while(status == 0 || i < *nProg){
				 if(indice < *nProg){
					  retornoCriaçãoProcesso = criaçãoProcesso(v[*nProg -1].NamePrograma);
					  indice = *nProg;
                
				 if ( retornoCriaçãoProcesso == -1 ){
					 fprintf(out, "Erro na criacao do processo\n");
				    return -1 ;
						}  
					}
			}
        
        ptemp = -1;    // Inicializando um valor inteiro para uma prioridade temporaria com o maior valor menor que 0
        
        while (k < *nProg ) {                         //Pega primeiro processo valido
            if (v[k].StatusProcesso == 0){
                ptemp = k;
            }
            k++;
        }
        
        for (j = 0 ; j < *nProg ; j++){           
            if (v[j].StatusProcesso == 0){                  
                if( v[j].Prioridade < v[ptemp].Prioridade){    // "Função busca" da maior prioridade
                    ptemp = j;                               // temp = posicao do vetor com processo de maior prioridade
                }
            }
        }
        
        if (ptemp != -1){                                    // Encontrou algum programa
			kill(v[ptemp].Pid, SIGCONT) ;					 //Roda programa
            sleep(0.5);
            
            pid2 = waitpid(v[ptemp].Pid, &status, WNOHANG);

            if (pid2 == 0) {                                          //Nao terminou, entao pausa.
                kill (v[ptemp].Pid, SIGSTOP) ;
            } else if (pid2 == -1) {    
				printf("Erro\n");
                return -1;
            } else {                                               
                v[ptemp].StatusProcesso = 1;                // Programa terminado 
                i++ ;
            }
        }
    }
		kill(pid,SIGKILL);
		}
	
	else{
		retornoInterp = Interpreter(in, out, v, nProg);
	}

	fclose(in);
	fclose(out);
	shmdt(v);
	shmdt(nProg);
	shmdt(status);
	shmctl(seg1, IPC_RMID, 0);
	shmctl(seg2, IPC_RMID, 0);
	shmctl(seg3, IPC_RMID, 0);

	return 0;
}

int criaçãoPrograma(int numerodoPrograma){
	int pid;
	char *nomePrograma = "prog";

	if(numerodoPrograma >= 0 && numerodoPrograma <= 7){
		sprintf(nomePrograma, "prog%d", numerodoPrograma);
	}
	pid = fork();

	if(pid < 0){
		printf("Erro ao criar processo\n");
		return -1;
	}
	else if(pid == 0){
		execl(nomePrograma, "aaaa", NULL);
	}
	else{
		kill(pid, SIGSTOP);
	}

	return 1;
}

int criaçãoProcesso(char *nomedoProcesso){
	int pid2;
	
	if(strcmp(nomedoProcesso, "prog0") == 0){
		pid2 = criaçãoPrograma(0);
	}

	else if(strcmp(nomedoProcesso, "prog1") == 0){
		pid2 = criaçãoPrograma(1);
	}

	else if(strcmp(nomedoProcesso, "prog2") == 0){
		pid2 = criaçãoPrograma(2);
	}

	else if(strcmp(nomedoProcesso, "prog3") == 0){
		pid2 = criaçãoPrograma(3);
	}

	else if(strcmp(nomedoProcesso, "prog4") == 0){
		pid2 = criaçãoPrograma(4);
	}

	else if(strcmp(nomedoProcesso, "prog5") == 0){
		pid2 = criaçãoPrograma(5);
	}

	else if(strcmp(nomedoProcesso, "prog6") == 0){
		pid2 = criaçãoPrograma(6);
	}

	else if(strcmp(nomedoProcesso, "prog7") == 0){
		pid2 = criaçãoPrograma(7);
	}
	
	else{
		printf("Programa com numero fora dos limites\n");
		return 0;
	}
	return 1;
}

int Interpreter (FILE * in,	FILE * out, Processo v[], int *numeroProgs)
{
    int	i = 0;		
    char nome[10];           
    
    numeroProgs = 0;                 
    
    while(fscanf (in, "%s", nome) > 0){ /* Enquanto o arquivo possuir algum programa com o nome certo */
        if(i != 0){
            sleep(1);
        }
        
        fscanf(in,"%s", v[i].NamePrograma);
        
        if(fscanf (in, "%s", nome) > 0){
			fscanf(in,"%d", &v[i].Prioridade);
        }
        
        if (v[i].Prioridade < 0 || v[i].Prioridade > 7){
			fprintf(out, "Numero de prioridade incorreto\n"); 
            return -1 ;
        }
        
        (*numeroProgs)++;
        i++;
        
    }

    return 0 ;
}
