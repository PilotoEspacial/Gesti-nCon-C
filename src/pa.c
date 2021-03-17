/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: pa.c
 * 
 * Descripcion: Creación de directorios en base al dni contenido en el fichero estudiantes.txt
 * 
 * Fecha de Creacion: 25-02-2020
 * 
 * Nombre: Paulino de la Fuente Lizcano
 * 
 * Seguimiento: En github se puede ver el cronograma del proyecto, aunque tuve problemas y no
 * esta todo reflejado
**********************************************/


#define _POSIX_SOURCE

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "libreria.h"

void instalarManejadorSignal();
void manejadorSignal(int sig);
void crearDirectorios();
void crearEstudiantes();


int main(int argc, char *argv[]){
    instalarManejadorSignal();
    /*Comprobar si existe el directorio estudiantes*/
    crearEstudiantes();
    crearDirectorios();

return 0;    
}

void instalarManejadorSignal(){
    if(signal(SIGINT,manejadorSignal)==SIG_ERR){
        fprintf(stderr,"[PC] Error al instalar el manejador: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void manejadorSignal(int sig){
    printf("[PC] Programa terminado(Ctrl+C)\n");
    exit(EXIT_FAILURE);
    /*terminarProcesos();*/
}

void crearEstudiantes(){
    DIR *estudiantes;
    if((estudiantes=opendir("estudiantes"))==NULL){
        if(mkdir("estudiantes",0777)==-1){
            fprintf(stderr,"Error creando directorio estudiantes: %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }else{
            printf("Creando directorio estudiantes\n");
        }  
    }else{
        printf("Directorio estudiantes ya existe\n");
    }
}

void crearDirectorios(){
    char completedPath[64];
    DIR *directorio;
    FILE *archivo;
    char linea[1024];
    const char *dni;
    
    if((archivo=fopen(ESTUDIANTES,"r"))!=NULL){
        while((fgets(linea,1024,archivo))!=NULL){
            dni = strtok(linea," ");
            sprintf(completedPath,"%s/%s","estudiantes",dni);
        /*Comprobamos si existe dicho directorio*/
            if((directorio=opendir(completedPath))!=NULL){
                /*printf("Ya existe el directorio: %s\n",dni);*/
            }else{
            /*Condicional aqui -1 error si no guay :)*/
                if((mkdir(completedPath,0777))!=-1){
                    /*printf("Directorio: %s creado\n",dni);*/
                }else{
                    fprintf(stderr,"Error creando directorio %s, causa: %s\n",completedPath,strerror(errno));
                }
            
            }
        }
    }else{
        printf("Error leyendo el txt\n");
    }
    fclose(archivo);
}