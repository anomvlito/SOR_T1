#include "main.h"
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

void imprime_inicio(char *file_name, InputFile *input_file) {
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
}

void scheduler(struct Queue *queueHigh, struct Queue *queueLow) {
  int tick = 0;
  struct Process *running_en_cpu = NULL;
  struct Queue *cabeza_colaHigh = queueHigh;
  struct Queue *cabeza_colaLow = queueLow;

  if (*running_en_cpu == NULL) {
    // Si no hay un proceso en estado RUNNING, ingresar el proceso de mayor
    // prioridad en estado READY a la CPU, esto implica ignorar a todos los que
    if (*cabeza_colaHigh->estado == READY) {
      *running_en_cpu = *cabeza_colaHigh;
    }

    else {
      // Si hay un proceso en estado RUNNING, actualizar su estado segun
      // corresponda. Esto podrıa incluir sacarlo de la CPU si su quantum ha
      // expirado o ha terminado su rafaga de ejecucion.
    }
  }

  int main(int argc, char const *argv[]) {
    /*Lectura del input*/
    char *file_name = (char *)argv[1];
    InputFile *input_file = read_file(file_name);

    imprime_inicio(file_name, input_file);

    int Numero_de_procesos = input_file->len;

    // ## 1) Se lee la primera línea de input:

    // - Se carga el quantum *q* asociado al scheduler, el cual se utilizará en
    // ambas colas. Por enunciado, la cola High tendrá el doble de quantum que
    // la cola Low: Quantum High = 2q, Quantum Low = q.

    // - Se carga el tiempo de inicio *T inicio* asociado al scheduler, el cual
    // se utilizará para determinar si un proceso debe ingresar a la cola High.

    int quantum = atoi(input_file->lines[0][0]);

    // se crean las colas para pasarlas al scheduler, cada cola recibe el
    // quantum, high recibe 2*quantum y low recibe quantum

    struct Queue *queueHigh = queue_init(*quantum * 2);
    // ver el nombre que le pone rosario
    // ################################################################################################

    struct Queue *queueLow = queue_init(*quantum);
    // ver el nombre que le pone rosario
    // ###########################################################################################

    // 	## 2) Se leen las líneas de input posteriores a la primera, donde
    // encontraremos la información asociada a los procesos.

    // - Se crean los procesos correspondientes al `struct process`, aun no se
    // ingresan al Scheduler.
    // - Hay que tener en consideración que cada proceso tiene un tiempo
    // `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por
    // primera vez.

    for (int i = 0; i < Numero_de_procesos; ++i) {
    };

    // 	## 3) Se crea el Scheduler, el cual recibe las colas High y Low, el

    input_file_destroy(input_file);
  }
