#include "../file_manager/manager.h"
#include "../process/process.h"
#include "../queue/queue.h"
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc

// 1 crear funcion que revisa si  hay Actualizar los procesos que hayan
// terminado su tiempo de espera de I/O de WAITING a READY.

// 2 crear funcion que revisa Si hay un proceso en estado RUNNING, actualizar su
// estado segun corresponda. Esto podrıa incluir sacarlo de la CPU si su quantum
// ha expirado o ha terminado su rafaga de ejecucion.

// 3 crear funcion que ingresa los procesos a las colas del scheduler

//  3.1) Si un proceso salió de la CPU, ingresarlo a la cola que corresponda.
//  3.2) Para cada proceso p, comprobar si tick = T inicio e ingresarlo a la
//  cola High. 3.3) Para cada proceso de la cola Low, revisar si se cumple la
//  condición para subir a la cola High y cambiarlos de cola según corresponda.

// Si no hay un proceso en estado RUNNING, ingresar el proceso de mayor
// prioridad en estado READY a la CPU, esto implica ignorar a todos los que se
// encuentren en estado WAITING, sin moverlos de su posición actual.

int main(int argc, char const *argv[]) {
  /*Lectura del input*/
  char *file_name = (char *)argv[1];
  InputFile *input_file = read_file(file_name);

  /*Mostramos el archivo de input en consola*/
  printf("Nombre archivo: %s\n", file_name);
  printf("Cantidad de procesos: %d\n", input_file->len);
  printf("Procesos:\n");
  for (int i = 0; i < input_file->len; ++i) {
    for (int j = 0; j < 7; ++j) {
      printf("%s ", input_file->lines[i][j]);
    }
    printf("\n");
  }

  input_file_destroy(input_file);
}