/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: demon.c
 * 
 * Descripcion: Hacer una copia de seguridad dentro de ./backup con el contenido de ./estudiantes
 * 
 * Fecha de Creacion: 05-03-2020
 * 
 * Nombre: Paulino de la Fuente Lizcano
 * 
 * Seguimiento: En github se puede ver el cronograma del proyecto, aunque tuve problemas y no
 * esta todo reflejado
**********************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>



int main(int argc, char *argv[]){

    while(1){
        printf("[DEMON] MI PID ES : %ld\n",(long)getpid());
        sleep(60);
        
        if(system("cp -r estudiantes ./backup")==-1){
            fprintf(stderr,"Error al ejecutar proceso demonio\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return EXIT_SUCCESS;       
}
