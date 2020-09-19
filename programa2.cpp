#include <time.h>
#include <stdlib.h>

#include <stdio.h>     /* funciones estándar de I/O                      */
#include <unistd.h>    /* funciones unix, como getpid()                  */
#include <signal.h>    /* signal name macros, y el prototipo signal()    */

#include <sys/wait.h>

const int M=5;
pid_t enProceso[M]={0,0,0,0,0};
int contP=0; // contador de procesos hijos
int tiempo;

/* Esta rutina es el manejador de la señal */
void procesarArchivo(int);

void pintarMenu();
void pintarDivicion();

void cacha_usr1(int sig_num)
{
	 /* reinstala el manejador de la señal de nuevo para cachar la señal SIGUSR1 */
	signal(SIGUSR1, cacha_usr1);
    contP--;

    int resultado;
    pid_t pidHijo= wait(&resultado);

    bool borrado=false;
    int i=0;
		do
		{	
			if(enProceso[i]==pidHijo)
			{
				enProceso[i]=0;
				borrado=true;
			}
			i++;

		}while(!borrado);
  
    return;
}


int main(int argc, char const *argv[])
{
	
	/* poner la runtina cacha_int como manejador de la señal SIGUSR1 */
	signal(SIGUSR1, cacha_usr1);



	system("clear");
	int eleccion;
	
	srand(time(NULL));
	
	do{	
		pintarMenu();

		fflush(stdin);
		scanf("%d",&eleccion);

		int pos=5;

		switch (eleccion)
		{
			case 1:
				system("clear");
				if (contP<M)
				{
					tiempo=10+rand()%(21-10);
					fflush(stdout);
					procesarArchivo(tiempo);
					contP++;	
					printf("\033[5;50H \033[42;30m#### Se ha agregado un proceso ###\n\033[0m");
				}else
				{
					
					printf("\033[5;50H \033[41;37m#### El servidor está saturado ###\n\033[6;50H### por favor intentelo más tarde ###\033[0m \033[0H\n");

				}
				break;

			case 2:

				system("clear");
				printf("\033[3;50H \033[47;30mEn proceso:\033[0m ");
				
				for(int i=0;i<M;i++)
				{
					if (enProceso[i]!=0)
					{
						printf("\033[%d;50H \033[44;40mtemp%d.txt \033[0m ",pos,enProceso[i]);
						pos++;
					}
					
				}
				
				fflush(stdout);
				break;

			case 3:
				system("clear");

				if(contP>0){
					pintarDivicion();
					printf("\033[5;5H\033[43;30m   El servidor se apagara en cuanto   ");
					printf(           "\033[6;5H      se terminen de procesar los     ");
					printf(           "\033[7;5H         archivos pendientes          \033[0m ");

					fflush(stdout);

					int totalP=contP;
					printf("\033[4;50H \033[44;40m     Archivos procesados: %d de %d    \033[0m", totalP-contP,totalP);
					fflush(stdout);
					while(contP>0)
					{
						pause();	
						printf("\033[4;50H \033[44;40m     Archivos procesados: %d de %d    \033[0m", totalP-contP,totalP);
						for (int i = 0; i <=((totalP-contP)*35)/totalP;i++)
						{
							printf("\033[6;%dH\033[46m \033[0m ",i+51);
						}
						fflush(stdout);
					}

					printf("\033[8;59H\033[41;37m  Servidor apagado  \033[0m\n\n\n\n\n");
					fflush(stdout);
				}else{
					printf("\033[2;14H\033[41;37m                          ");
					printf(           "\033[3;14H                          ");
					printf(           "\033[4;14H     Servidor apagado     ");
					printf(           "\033[5;14H                          ");
					printf(           "\033[6;14H                          \033[0m\n\n\n");
					fflush(stdout);
				}

				break;

		}

	}while(eleccion!=3);

	return 0;
}

void procesarArchivo(int tiempo)
{
	pid_t p = fork();
	if (p==0)  //si eres el hijo
	{
		sleep(tiempo);
		kill(getppid(),SIGUSR1);
		_exit(0);
	}
	if (p>0)
	{
		bool guardado=false;
		int i=0;
		do
		{
			if(enProceso[i]==0)
			{
				enProceso[i]=p;
				guardado=true;
			}
			i++;

		}while(!guardado);
	}

	return;
}

void pintarMenu()
{
	pintarDivicion();
	printf("\033[3;1HMENU: ");
	printf("\033[5;1H1. Iniciar el procesamiento de un archivo");
	printf("\033[6;1H2. Desplegar lista de archivos en proceso");
	printf("\033[7;1H3. Apagar servidor:");
	printf("\033[9;1H -> ");
	fflush(stdout);
}

void pintarDivicion(){
	for(int i=3;i<10;i++){
		printf("\033[%d;47H|",i);
	}
}


