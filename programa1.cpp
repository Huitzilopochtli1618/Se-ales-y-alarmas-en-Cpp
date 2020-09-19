

#include <fstream>
#include <stdlib.h>

#include <stdio.h>     /* funciones estándar de I/O                      */
#include <unistd.h>    /* funciones unix, como getpid()                  */
#include <signal.h>    /* signal name macros, y el prototipo signal()    */


using namespace std;

/* Esta rutina es el manejador de la señal */
void cacha_int(int sig_num)
{
    /* reinstala el manejador de la señal de nuevo para cacharla la siguiente vez */
    signal(SIGINT, cacha_int);
    printf("\nBorrando archivos temporales...\n");
    fflush(stdout);
    system("rm temp1.txt");
    system("rm temp2.txt");

    printf("Archivos borrados\n");
    fflush(stdout);
    _exit(0);

}


int main(int argc, char const *argv[])

{
	 /* poner la runtina cacha_int como manejador de la señal INT (ctrl-c)*/
	signal(SIGINT, cacha_int);

	//crea archivos temporales
	ofstream archivo1;
	archivo1.open("temp1.txt",ios::out);// crea o remplaza el archivo
	archivo1.close();

	ofstream archivo2;
	archivo2.open("temp2.txt",ios::out);// crea o remplaza el archivo
	archivo2.close();

	for ( ;; )
	{
		pause();
	}


	return 0;
}
