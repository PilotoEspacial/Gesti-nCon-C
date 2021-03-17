/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: pc.c
 * 
 * Descripcion: Calcular nota media de la clase y calcular nota minima para aprobar para cada estudiante
 * 
 * Fecha de Creacion: 27-02-2020
 * 
 * Nombre: Paulino de la Fuente Lizcano
 * 
 * Seguimiento: En github se puede ver el cronograma del proyecto, aunque tuve problemas y no
 * esta todo reflejado
**********************************************/

#define _POSIX_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include "libreria.h"


void instalarManejadorSignal();
void manejadorSignal(int sig);
void lecturaEstudiantes();
int calcularNota(char *nota);
void copiarNota(char *modelo, char *dni, int nota);
void calcularMedia(int Notas[15]);

int  main(int argc, char **argv){
    int Notas[15];
    lecturaEstudiantes(Notas);
    calcularMedia(Notas);
    return EXIT_SUCCESS;
}

void instalarManejadorSignal(){
    if(signal(SIGINT,manejadorSignal)==SIG_ERR){
        fprintf(stderr,"[PC] Error al instalar el manejador: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void manejadorSignal(int sig){
    fprintf(stderr,"[PC] Programa terminado(Ctrl+C), %s\n",strerror(errno));
    exit(EXIT_FAILURE);
}

void lecturaEstudiantes(int Notas[15]){
    FILE *archivo;
    char buffer[1024];
    char *modelo;
    char *dni;
    char *nota;
    int i=0;

    if((archivo=fopen(ESTUDIANTES,"r"))==NULL){
        fprintf(stderr,"[PC]Error leyendo el archivo estudiantes.txt, %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    while(fgets(buffer,1024,archivo)!=NULL){
        dni = strtok(buffer," ");
        modelo = strtok(NULL," ");
        nota = strtok(NULL," ");
        Notas[i]=atoi(nota);
        i++;
        /*Descomentar linea de abajo para ver si la lectura es correcta*/
        copiarNota(modelo,dni,calcularNota(nota));
    }
}

int calcularNota(char *nota){
    return 10-atoi(nota);
}

void calcularMedia(int Notas[15]){
    int i;
    double total = 0;
    
    for(i=0;i<15;i++){
        total=total+Notas[i];
    }
    printf("%f\n",total/15);
}

void copiarNota(char *modelo, char *dni, int nota){
    FILE *nuevo;
    /*Variable para guardar el directorio donde guardar el examen de cada alumno*/
    char fullPathEstudiante[1024];

    sprintf(fullPathEstudiante,"%s/%s/%s",PATH_ESTUDIANTES,dni,ARCHIVO_NOTA);

    if((nuevo = fopen(fullPathEstudiante,"w"))==NULL){
        fprintf(stderr,"[PC] Error creando el nuevo fichero %s\n",fullPathEstudiante);
        exit(EXIT_FAILURE);
    }
    fprintf(nuevo,"Tu nota necesaria para aprobar es %d",nota);
    fclose(nuevo);
}