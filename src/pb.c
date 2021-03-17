/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: pb.c
 * 
 * Descripcion: Copiar el modelo de examen en el directorio de un estudiante
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


/*Lectura del txt donde estan almacenados
los recursos necesarios de los estudiantes*/
void lecturaEstudiantes();
void instalarManejadorSignal();
void manejadorSignal(int sig);
/*Comparamos el modelo con los tipos de examenes
Una vez asignado el examen, copiamos dicho examen en su directorio*/
void copiarExamen(char *modelo, char *dni);
char *asginarExamen(char *modelo);

/*********PROGRAMA MAIN*********/
int  main(int argc, char **argv){
    instalarManejadorSignal();
    lecturaEstudiantes();
    return EXIT_SUCCESS;
    
}

/******FUNCIONES AUXILIARES*******/
void instalarManejadorSignal(){
    if(signal(SIGINT,manejadorSignal)==SIG_ERR){
        fprintf(stderr,"[PB] Error al instalar el manejador: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void manejadorSignal(int sig){
    printf("[PB] Programa terminado(Ctrl+C)\n");
    exit(EXIT_FAILURE);
}

/**********FUNCIONES PROGRAMAS*********/
void lecturaEstudiantes(){
    FILE *archivo;
    char buffer[1024];
    char *modelo;
    char *dni;

    if((archivo=fopen(ESTUDIANTES,"r"))==NULL){
        fprintf(stderr,"[PB] Error leyendo el archivo estudiantes.txt, %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    while(fgets(buffer,1024,archivo)!=NULL){
        dni = strtok(buffer," ");
        modelo = strtok(NULL," ");
        /*Descomentar linea de abajo para ver si la lectura es correcta*/
        /*printf("DNI: %s\t Modelo:%s\n",dni,modelo);*/
        copiarExamen(asginarExamen(modelo),dni);
    }
}

char *asginarExamen(char *modelo){
    char *Examen;

    if(strcmp("A",modelo)==0){
        Examen = A;
    }else if(strcmp("B",modelo)==0){
        Examen = B;
    }else if(strcmp("C",modelo)==0){
        Examen = C;
    }
    return Examen;
}
void copiarExamen(char *modelo, char *dni){
    FILE *nuevo, *mi_examen;
    int data = 0;
    /*Variable para almacenar el path enterod e un examen*/
    char fullPathEx[1024];
    /*Variable para guardar el directorio donde guardar el examen de cada alumno*/
    char fullPathEs[1024];

    sprintf(fullPathEx,"%s/%s",PATH_EXAMENES,modelo);
    sprintf(fullPathEs,"%s/%s/%s",PATH_ESTUDIANTES,dni,modelo);

    if((mi_examen = fopen(fullPathEx,"r"))==NULL){
        fprintf(stderr,"[PB] Error abriendo el examen %s\n",fullPathEx);
        exit(EXIT_FAILURE);
    }
    if((nuevo = fopen(fullPathEs,"w"))==NULL){
        fprintf(stderr,"[PB] Error creando el nuevo fichero %s\n",fullPathEs);
        exit(EXIT_FAILURE);
    }

    while((data = fgetc(mi_examen))!=EOF){
        fputc(data,nuevo);
    }

    fclose(nuevo);
    fclose(mi_examen);
}