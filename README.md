SSOO2

PRACTICA 1:

    Esta practica consta de un programa manager, y subprocesos pa, pb, pc, pd

    -Manager: Se encarga de gestionar, lanzar y controlar dichos subprocesos, tambien 
              tiene la tarea de lanzar un proceso "demonio" el cual se ejecuta en 2º plano
              para hacer una copia de seguridad de la carpeta estudiantes en el directorio 
              backup. Cada vez que uno de dichos procesos completa su tarea, se registra en
              un log.txt

    -pa: Este proceso crea el directorio estudiante, en el caso que no exista, y rellenarlo con
         otros subdirectorios con el nombre de los DNI contenido en el txt "estudiantes"
    
    -pb: Proceso que copia el modelo de examen asignado a dicho alumno

    -pc: Proceso que crea un txt con la nota minima que necesita ese alumno para aprobar, 
         además este proceso comunica la nota media de la clase al padre mediante una tuberia.

    -pd: Este proceso se encarga de eliminar el directorio estudiantes en caso de que el usuario
         finalice el proceso manager con la convinación de teclas "Ctrl+C"

    
   EJECUTAR EL PROGRAMA:

        -Para ejecutar el programa contamos con un MakeFile para agilizar el proceso:
        Tenemos que estar en la carpeta raiz, ./Practica1
       
   -make all
   
         Con este comando, creamos los directorios necesarios y se generan los binarios necesarios.
        
   -make run

        Para finalizar el proceso demon.c hay que mandar una señal kill al PID mostrado por terminal. Después solo hay que mandar una señal kill para "matarlo".
    	
   -make cleanAll      
         
        Para limpiar los directorios.
    
