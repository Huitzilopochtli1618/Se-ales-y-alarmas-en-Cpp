#include <stdio.h>     /* funciones de I/O standard                      */
#include <unistd.h>    /* funciones standard de unix como alarm()        */
#include <signal.h>    /* nombres de macros de señales, y el prototipo de signal() */

#include <fstream>
#include <iostream>

#include <string>
using namespace std;


int ultimaFilaProcesada;
int ultimaColumnaProcesada;
int Max;
int m1[20][20];
int m2[20][20];
int m3[20][20];
/* define el manejador de la alarma */
void cacha_alarma(int sig_num)
{   
    //system("clear");
    /* reinstala el manejador para la señal ALRM */
    signal(SIGALRM, cacha_alarma);
    printf("\033[10;2H \033[47;30m Realizando respaldo: \033[0m ");
    ofstream archivo3;
    archivo3.open("matrizResultado.txt",ios::out);//modo escritura
    bool romper=false;

    //Realizo el respaldo solo hasta donde se hizo la ultima operacion
    for (int i = 0; i < Max; i++)
    {
        for (int j = 0; j <Max; j++)
        {
            if(i==ultimaFilaProcesada+1 && j==ultimaColumnaProcesada+1){
                romper=true;
                break;
            }else{
                archivo3<<m3[i][j]<<" ";

                fflush(stdout);
            }
        }
        if(romper){
            break;
        }else{
            archivo3<<"\n";

        }
    }

    int avance=((1+(((ultimaFilaProcesada+1)*Max)-(Max-ultimaColumnaProcesada)))*100)/(Max*Max);


    for (int i = 0; i <= avance/3 ;i++)
    {
        printf("\033[10;%dH\033[46;30m %d%%\033[0m",i+25, avance);
    }

    fflush(stdout);
    alarm(5);
}

int main(int argc, char* argv[])
{
    system("clear");
    /* Poner el manejador para la señal ALRM */
    signal(SIGALRM, cacha_alarma);
    
    srand(time(NULL));//para el generador aleatorio,
    int estado;//0=incompleto, 1=completo


    ifstream archivo;

    //detecto si el archivo matrices existe y sino lo creo
    archivo.open("matrices.txt",ios::in);//modo lectura

    if(archivo.fail()){//Si el archivo matrices.txt no existe entonces
        ofstream archivo2;
        archivo2.open("matrices.txt",ios::out);//modo escritura
        //creo el registro de las matrices cuadradas de tamaño y valores aleatorios

        int tam;
        int elemento;
        tam=5;//4+rand()%(7-4);

        archivo2<<"0 "<<tam<<"\n";//el estado inicial sera incompleto

        //Guardo los valores aleatorios en forma de matriz en matrices.txt-----
        for (int i = 0; i < tam; i++)                                        //
        {                                                                    //
            for (int j = 0; j < tam; j++)                                    //
            {                                                                //
                elemento=1+rand()%(11-1);                                    //
                if(j!=tam-1){                                                //
                    archivo2<<elemento<<" ";                                 //
                }else{                                                       //
                    archivo2<<elemento<<endl;                                //
                }                                                            //
            }                                                                //
        }                                                                    //
        archivo2<<"\n";                                                      //
        for (int i = 0; i < tam; i++)                                        //
        {                                                                    //
            for (int j = 0; j < tam; j++)                                    //
            {                                                                //
                elemento=1+rand()%(11-1);                                    //
                if(j!=tam-1){                                                //
                    archivo2<<elemento<<" ";                                 //
                }else{                                                       //
                    archivo2<<elemento<<endl;                                //
                }                                                            //
            }                                                                //
        }                                                                    //
        //--------------------------------------------------------------------
      
        printf("\033[2;13H\033[42;30m                                     ");
        printf(           "\033[3;13H       No existe ningun archivo      ");
        printf(           "\033[4;13H         por lo tanto se creo        ");
        printf(           "\033[5;13H          aleatoriamente uno         ");
        printf(           "\033[6;13H                                     \033[0m\n\n\n");
        fflush(stdout);
        fflush(stdout);
        archivo2.close();
    }else{
        int estaTerminado;
        archivo>>estaTerminado;
        if(estaTerminado==0){  
            printf(        "\033[2;13H\033[41;37m                                    ");
                    printf(           "\033[3;13H        Se detecto un archivo       ");
                    printf(           "\033[4;13H   que no se guardo completamente   ");
                    printf(           "\033[5;13H                                    \033[0m\n\n\n");
                    fflush(stdout);
  
            fflush(stdout);
        }
    }

    archivo.close();

    //ahora no existe forma de que el archivo matrices no exista, a menos que ocurra un error al abrir el archivo
    archivo.open("matrices.txt",ios::in);//modo lectura
    if(archivo.fail()){
        cout<<"\n\nError al abrir el archivo\n\n";
        exit(0);
    }else{

        archivo>>estado>>Max;   //el estado me dira si la matriz se termino de procesar
    }

    if(estado==0){//si la matriz resultante no se a terminado de generar cargo los valore en 2 matrices
        for (int i = 0; i < Max;i++)
        {
            for (int j = 0; j < Max; j++)
            {
                archivo>>m1[i][j];
            }
        }

        for (int i = 0; i < Max;i++)
        {
            for (int j = 0; j < Max; j++)
            {
                archivo>>m2[i][j];
            }
        }

        archivo.close();
    

        //realizo las operaciones para obtener una matriz resultado
        alarm(5);//cada 5seg tendre un respaldo
        for (int i = 0; i < Max; i++)
        {
            for (int j = 0; j < Max; j++)
            {
                m3[i][j]=0;
                for (int k = 0; k < Max; k++)
                {
                    m3[i][j]+=m1[i][k]*m2[k][j]; 
                }
                ultimaFilaProcesada=i;      
                ultimaColumnaProcesada=j;
                sleep(1);   //para simular procesamiento
            }
        }

        archivo.close();

        //vuelvo a guardar el archivo matriz, pero ahora indicando que ya se termino de procesar las matrices
        fstream archivo4;
        archivo4.open("matrices.txt",ios::out);//modo escritura
        archivo4<<"1 "<<Max<<"\n\n"; //indico que ya esta completado el proceso de las multiplicaciones

        for (int i = 0; i < Max; i++)//Guardo matriz 1
        {
            for (int j = 0; j < Max; j++)
            {
                archivo4<<m1[i][j]<<" ";
            }
            archivo4<<"\n";
        }

        archivo4<<"\n";

        for (int i = 0; i < Max; i++)//Guardo la matriz 2
        {
            for (int j = 0; j < Max; j++)
            {
                archivo4<<m2[i][j]<<" ";
            }
            archivo4<<"\n";
        }
        sleep(6);//retrazo el progrma a proposito para hacegurarme de hacer un ultimo respaldo, irrefutablemente completo
        system("clear");
        printf("\033[6;14H \033[42;30m#### --------Matriz Guardada------ ###\n\n\n\n\n\n\033[0m");
   
    }else{
        printf("\033[6;14H \033[42;30m#### --------La matriz ya fue procesada------ ###\n\n\n\n\n\n\033[0m");
    }
    
    printf("\n\n\n\n\n\n\n\n\n\n");
    
    fflush(stdout);



    return 0;
}