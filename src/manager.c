/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: manager.c
 * 
 * Descripcion: Gestiona la creacion, control y finalización de los procesos creados
 * 
 * Fecha de Creacion: 25-02-2020
 * 
 * Nombre: Paulino de la Fuente Lizcano
 * 
 * Seguimiento: En github se puede ver el cronograma del proyecto, aunque tuve problemas y no
 * esta todo reflejado
**********************************************/

#define _POSIX_SOURCE
#define _BDS_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include "libreria.h"

#define NUM_HIJOS 3

/*Tabla para guardar los procesos creados*/
pid_t MISPROCESOS[NUM_HIJOS];
FILE *LOGFD;
/****METODOS CLASE MANAGER****/
void lanzarDemonio();
void calcularHora();
void instalarManejadorSignal();
void manejadorSignal(int sig);

void crearProcesoPA();
void crearProcesosB_C(int tuberia[2]);
void esperarProcesosB_C();
void terminarProcesos();
FILE *iniciarPunteroLog();

/****************MAIN PROGRAM****************/
int main(int argc, char *argv[]){
    int tuberia[2];
    char media[1];
    LOGFD = iniciarPunteroLog();
    /*Puntero definido en la libreria*/
    lanzarDemonio();
    
    instalarManejadorSignal();
    fprintf(LOGFD,"******************************************************\n");
    calcularHora();
    printf("Archivo log.txt creado\n");

    crearProcesoPA();
    fprintf(LOGFD,"*Creada la carpeta estudiantes y sus subdirectorios\t*\n");
    crearProcesosB_C(tuberia);
    sleep(5);
    esperarProcesosB_C();

    read(tuberia[0],media, sizeof(char));
    fprintf(LOGFD,"*La nota media de la clase es : %s\t\t*\n",media);
    fprintf(LOGFD,"******************************************************\n\n");
    fclose(LOGFD);
    return EXIT_SUCCESS;
}

/*******Funcion que obtener la fecha y l*********/
void lanzarDemonio(){
    pid_t pid;
    pid=fork();

    if(pid==-1){
        fprintf(stderr,"Error al crear demon.c \n");
        exit(EXIT_FAILURE);
    }else if(pid==0){
        if(execl(DM,"&",NULL)==-1){
            fprintf(stderr,"Error al ejecutar demon \n");
            exit(EXIT_FAILURE);
        }
    }
}

void calcularHora(){
    time_t tiempo=time(0);
    struct tm *tlocal = localtime(&tiempo);
    char salida[128];
    strftime(salida,128,"*%d/%m/%y %H:%M*\n",tlocal);
    fprintf(LOGFD,salida);
}


void iniciarTablaProcesos(){
    int i;
    for(i=0;i<NUM_HIJOS;i++){
        MISPROCESOS[i]=0;
    }
}

void instalarManejadorSignal(){
    if(signal(SIGINT,manejadorSignal)==SIG_ERR){
        fprintf(stderr,"[MANAGER] Error al instalar el manejador: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void manejadorSignal(int sig){
    pid_t pidPD;
    int status;
    
    terminarProcesos();

    pidPD=fork();
    if(pidPD==-1){
        fprintf(stderr,"Error al crear el proceso PD, %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pidPD==0){
        if(execl(PD,PD,NULL)==-1){
            fprintf(stderr,"Error al ejecutar pa %d\n",errno);
            exit(EXIT_FAILURE);
        }
    }else{
        waitpid(pidPD,&status,0);
        fprintf(LOGFD,"*PROGRAMA ACABADO POR Ctrl+C\n*");
        fprintf(LOGFD,"******************************************************\n\n");
        printf("[MANAGER] Programa terminado(Ctrl+C)\n");
        exit(EXIT_FAILURE);
    }
}

FILE *iniciarPunteroLog(){
    FILE *log;
    if((log=fopen(LOG,"a"))==NULL){
        fprintf(stderr,"Error creando archivo log.txt, %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    return log;
}

void crearProcesoPA(){
    pid_t pidPA;
    pidPA=fork();
    MISPROCESOS[0]=pidPA;

    if(pidPA==-1){
        fprintf(stderr,"Error al crear hijo PA, %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }else if(pidPA==0){
        if(execl(PA,PA,NULL)==-1){
            fprintf(stderr,"Error al ejecutar pa %d\n",errno);
            exit(EXIT_FAILURE);
        }
    }else{
        pidPA=wait(NULL);
        printf("[MANAGER] Proceso PA acabado\n");
    }
}


void crearProcesosB_C(int tuberia[2]){
    int i;
    
    pipe(tuberia);
    for(i=1;i<NUM_HIJOS;i++){
        MISPROCESOS[i] = fork();

        switch(MISPROCESOS[i]){
            case -1:
                fprintf(stderr,"Error al crear un proceso %s\n",strerror(errno));
                exit(EXIT_FAILURE);
            case 0:
                if(i==1){
                    if(execl(PB,PB,NULL)==-1){
                        fprintf(stderr,"Error ejecutando PB, %s\n",strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                }else if(i==2){
                    dup2(tuberia[1],STDOUT_FILENO);
                    if(execl(PC,PC,NULL)==-1){
                        fprintf(stderr,"Error ejecutando PC, %s\n",strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                }
        }
    }
}

void esperarProcesosB_C(){
    pid_t pidWait;
    int status,i;

    for(i=1;i<NUM_HIJOS;i++){
        pidWait=wait(&status);
        if(pidWait==MISPROCESOS[1]){
            printf("[MANAGER] Proceso PB acabado\n");
            fprintf(LOGFD,"*Copiado el modelo de examen a cada alumno\t\t*\n");

        }else if(pidWait==MISPROCESOS[2]){
            printf("[MANAGER] Proceso PC acabado\n");
            fprintf(LOGFD,"*Asignada la nota minima para aprobar a cada alumno\t*\n");
        }
    }

}

void terminarProcesos(){
    int i;
    printf("\n------- [MANAGER] Terminating running child processes ----\n");
    
    for(i=0;i<NUM_HIJOS;i++){
        if(MISPROCESOS[i] != 0){
            printf("[MANAGER] Terminating process [%d]...\n",MISPROCESOS[i]);
            if(kill(MISPROCESOS[i],SIGINT)==-1){
                fprintf(stderr,"[MANAGER] Error using kill on process %d: %s\n",MISPROCESOS[i], strerror(errno));
            }
        }
    }
}
