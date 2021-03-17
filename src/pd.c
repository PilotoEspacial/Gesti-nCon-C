/*********************************************
 * 
 * Proyecto: Practica 1 de SSOO2
 * 
 * Programa: pd.c
 * 
 * Descripcion: Eliminar carpeta estudiantes en caso de interrupcion del programa manager.c
 * 
 * Fecha de Creacion: 03-03-2020
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

    if(system("rm -rf estudiantes/")==-1){
        fprintf(stderr,"[PD] Error al eliminar estudiantes\n");
    }
    printf("[PD] El directorio estudiantes ha sido borrado.\n");
    
    return EXIT_SUCCESS;       
}
