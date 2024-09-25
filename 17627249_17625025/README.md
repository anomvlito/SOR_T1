# SOR_T1

Para esta tarea, asumimos que no nos están pidiendo ejecutar los procesos con respecto al timer del computador o algo parecido, nosotros simularemos el tiempo. Proponemos correr el programa en un bucle tal que itere sobre una variable entera `int tick = 0`, la cual incrementará en cada iteración.

# **Apuntes/Estrategia**

# Consideraciones importantes explícitas y no explícitas en el enunciado:

- Recordar que si un proceso consume su quantum, este pasa a la cola Low. 

- Cuando un proceso cambia de una cola a otra, el valor del quantum asociado se debe reiniciar con el valor del quantum de la nueva cola. Por ejemplo, si un proceso en la cola High está ejecutándose en la CPU y se le acaba el quantum disponible, cede la CPU y pasa a la cola Low, su quantum se reinicia al valor del quantum de la cola Low.

- Recordar que para que un proceso sea ejecutado desde la cola Low, los demás procesos en la cola High tienen que haber terminado su ejecución por completo.

- Tener en consideración que si se están ejecutando procesos en la cola Low, hay que estar atentos si es necesario pasar estos procesos a la cola High si se cumple para este proceso que, `2 * Tdeadline < Tactual − TLCPU`, este proceso sube a la cola High, y en el próximo bloque tick este proceso tendrá prioridad por sobre los otros procesos.

### - Sobre el estado `READY`:

- Si un proceso llega por primera vez al scheduler, lo hace en estado `READY`, además, por enunciado va directamente a la cola High. 

- Si existe otro proceso ocupando la **CPU** y no la cede, los procesos en estado `READY` al no ser seleccionado para ser llevados y ejecutados en CPU, se mantiendran en estado `READY`, ademas:

    - se le suma una unidad de `WAITING` time.

    - se le suma una unidad de response time (el tiempo que tardo en entrar por primera vez a la CPU), hasta que el valor de current_burst 

    - si el proceso pasa de estado `READY` a `RUNNING`, este es seleccionado para ser llevado a la cpu, pasando a estado , se le reste una unidad de `WAITING` time ya que por construccion del scheduler, antes de este analisis se actualizan todos los procesos que estan en estado ready mientras la cpu esta ocupada.

### - Sobre el estado ``WAITING``:

- Un proceso no puede estar en estado ``WAITING`` sin haber ejecutado por completo su primera ráfaga de CPU.

- Si un proceso ya ejecuto "quemo" su primera rafaga de cpu por completo, cuando este en estado ``WAITING``, pasara que:

    - se le suma una unidad de `WAITING` time.

    - se le suma 1 y al current_io_wait_time y al igualarse con io_wait_time se pasa a estado `READY`.

    - Si justo cuando pasa a estado `READY` y este es seleccionado para ser llevado a la cpu, pasando a estado `RUNNING`, el mismo scheduler es capas de controlar las unidades de `WAITING` time sumadas en cierta parte del codigo, esto esta referenciado en el tercer elemento listado, del segundo elemento listados de "sobre el estado ready". 


################
################
################

CONTROLAR LOS CAMBIO DE ESTADO DE LOS PROCESOS
BUSCAR UNA MANERA DE HACERLO Y QUE SEA INTELIGNTE 

################
################

### Sobre el tiempo de espera `IO_WAIT`:

- Entre rafagas de cpu se espera input/output `IO_WAIT`antes de poder ser elegido nuevamente para pasar a la **CPU**.

# **Construcción de structs**






## **Funciones asociadas a la CPU**
- Modifica el quantum de cada proceso que entra a la cpu, el cual está designado desde la cola de la que proviene antes de entrar a la CPU.

## Process

- **Nombre** `char nombre[10]`
- **Pid** `int pid`
- **Burst time** `int burst_time` // Tiempo de ejecucion por rafaga
- **num_bursts_solicitados_por_proceso** `int num_bursts_solicitados_por_proceso`
- **Io wait time** `int io_wait_time`
- **Deadline** `int deadline`
- **Estado** `estado_t estado`
- **Quantum** `int quantum`
- **Queue** `char queue[10]`
- **Interrupciones** `int interrupciones`
- **Turnaround time** `int turnaround_time` // termino – t inicio
- **Response time** `int response_time` // tiempo que tardo en entrar por primera vez a la CPU
- **`WAITING` time** `int waiting_time` // numero de veces que se encuentre en estado `WAITING` y ready (sin contar ready cuando pasa a running)
- **Suma deadline** `int deadline_sum`
- **num_current_complete_burst** `int num_current_complete_burst` // para saber si es su primera rafaga, y su ultima para pasar a Finished
- **Current Burst** `int current_burst` // para saber si sale de la CPU
- **Current io wait time** `int current_io_wait_time`

#################
ver si descartar:

- cada proceso deber tener el atributo q_cola_de_origen, el cual no puede decrementar a diferencia de quantum restante,

- cada proceso debe tener quantum_restante el cual dene estar asociado a cola actual

#################



## **Funciones process **

 - Create process: 
    - se guarda el nuevo proceso con sus atributos 
    - Move to high queue 
    - Estado ready por defecto cuando es creado.



## Queue 

se va a ORDENAR COLA POR PRIORIDAD, con insertion sort.



## **Funciones asociadas a cola de prioridad**
### Insert High Queue

- **Process Quantum**: `2 * quantum`
- **Action**: Agregar a la cola High Queue

### Insert Low Queue

- **Process Quantum**: `quantum`
- **Action**: Agregar a la cola Low Queue



### Ordenar procesos con estado `READY`

- Utilizar insertion sort, seleccionando solo los procesos con estado `READY` de la cola. 
- el con mayor prioridad es el que tenga mayor:
    - (Tactual − TLCPU1) − Tdeadline . 

- Si hay un empate en el deadline, se debe escoger el que tenga menor PID.  

Un proceso en estado RUNNING solo sale de la CPU si se acaba su quantum o cede la CPU.




# **Modelamiento/ flujo del main: **

## 1) Se lee la primera línea de input:

- Se carga el quantum *q* asociado al scheduler, el cual se utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de quantum que la cola Low: Quantum High = 2q, Quantum Low = q.
- Procesos restantes = N procesos 
## 2) Se leen las líneas de input posteriores a la primera, donde encontraremos la información asociada a los procesos.

- Se crean los procesos correspondientes al `struct process`, aun no se ingresan al Scheduler.
- Hay que tener en consideración que cada proceso tiene un tiempo `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por primera vez.
- Se crean las colas high.
- Se crean las colas low.
- se llama y crea la lista de procesos finalizados, con sus funciones correspondientes
- se crea  Process *cpu_process = NULL; que simula la cpu.

## 3) se crea un bucle que simula el paso del tiempo y en cada iteracion se aumenta en una unidad el tick que comienza en 0.

  - Se ejecuta el `Scheduler`.

  - Verificar si todos los procesos han terminados, si es asi se genera un archivo outuput "informe" tal como se pide en el enunciado, tal que:
    - si terminaron, verificando Procesos restantes = 0: 
        - break al while
    - si no terminaron:
        - aumentar en una unidad el tick para la nueva iteracion del bucle.


## 4) si se rompio el bucle es por que terminaron de ejecutarse los procesos, por lo tanto ejecutar:
    - walk_in_finished_processes(finished_list), aqui crea el informe correspondiente que escribira en el csv como output, esto se hace a traves de la funcion  walk_in_finished_processes, la cual recorrera todos los procesos terminados y tomara los atributos de cada proceso 


## 5) Liberar la memoria


## Scheduler

Creeamos la función Scheduler que controla todo lo que pasa en cada intervalo tick de tiempo. 

<!-- - si se libera la cpu tener atención con los procesos que terminan su espera i/o_waiting, ya que pasarian de estado `WAITING` a ready y podrian tener mayor prioridad que otros elementos en la misma cola que ya estaban en estado ready. -->

- Comparamos prioridad de procesos en esta prioridad:

    - solo comparamos elementos cuando la cpu esta libre.

    - solo comparamos si las colas high y low tienen len mayor a 0 (no estan vacias).

    - solo comparamos elementos de la cola low si la cola high tienen len igual a 0 (esta vacia).

    - si la cola a comparar no esta vacia, solo se comparan los procesos en estado ready

- Manejo del waiting_time:
    - Para procesos que ingresan al estado READY: Cuando un proceso es agregado a la cola READY (en la cola high o low) y no es seleccionado para ejecutarse en la CPU durante ese tick, se incrementa su waiting_time en 1.
    Esto ocurre cuando el proceso se mueve a READY pero no pasa a RUNNING en el mismo tick. Por ejemplo, un proceso que inicia en el tick = T_INICIO entra a READY pero su waiting_time se incrementa ya que no fue inmediatamente seleccionado.

    - Para procesos que pasan a RUNNING: Al seleccionar un proceso para ejecutarse en la CPU (cambio de READY a RUNNING), se debe restar 1 al waiting_time para compensar el hecho de que fue seleccionado en ese tick. Esto asegura que el waiting_time refleja correctamente el tiempo total que el proceso esperó antes de ser ejecutado.

## **Funcion scheduler**

Por cada tick, el scheduler realiza las siguientes tareas, en el orden indicado:  

1) Actualizar los procesos que hayan terminado su tiempo de espera de I/O de `WAITING` a READY.  

    -  Iterar sobre cola high y low y revisar io_wait_time- current io_wait_time , si esto llega a ser = 0  pasara a estado ready.
        - esto se hace como fue mencionado anteriormente "se le suma 1 y al current_io_wait_time y al igualarse con io_wait_time se pasa a estado `READY`".

2) Si hay un proceso en estado RUNNING, actualizar su estado según corresponda. Esto podría incluir sacarlo de la CPU si su quantum ha expirado o ha terminado su rafaga de ejecucio. Esto pasa cuando (*cpu_process != NULL) es true (la cpu esta ocupada) y por consiguiente:

    - Actualizar sus variables quantum, current burst, num_current_complete_burst y estado(según burst= wait o quantum=ready), por lo tanto:
        - se resta una unidad de quantum
        - se suma una unidad a current burst
    

    - Si no queda quantum: <!--if-->
        - si ({burst_time-current_burst} > 0) :  <!-- el proceso es interrumpido, hacer esto dentro de un if-->
            - se suma una unidad a interrupciones. <!-- muy importante-->

            - proceso sigue en estado `READY`.

            - reiniciar su quantum, con el valor de la cola de la que proviene.

            - se ingresa a la cola de la que proviene.

            - se libera la cpu con *cpu_process = NULL;

        - si ({burst_time-current_burst} = 0) : <!-- la rafaga en la que esta ha terminado, hacer esto dentro de un else if-->
            <!-- muy importante-->
            - aumentar en una unidad num_current_complete_burst <!-- muy importante-->
            <!-- muy importante-->
            
            - si ({num_bursts_solicitados_por_proceso -num_current_complete_burst} > 0): <!-- aun quedan rafagas por quemar -->
                - proceso cambia a estado ``WAITING``.

                - reiniciar su quantum, con el valor de la cola low

                - se incorpora a la cola low <!-- muy importante-->

                - se libera la cpu con *cpu_process = NULL;

            - si ({num_bursts_solicitados_por_procesos -num_current_complete_burst} = 0): <!-- el proceso termina su ejecución, hacer esto dentro de un else if-->
                - proceso cambia a estado `FINISHED`.

                - restar una unidad  a Procesos restantes.

                - es agregado al arreglo de procesos terminados Finished processes.

                - se libera la cpu con *cpu_process = NULL; (recordatorio para fabian, ojo no se termina el scheduler solo queda libre para prepararse para la proxima ejecución).

    - Si le queda quantum: <!--hacer esto dentro de un else if-->
        - si ({burst_time-current_burst} > 0) (la rafaga en la que esta no ha terminado): <!-- el proceso es interrumpido, hacer esto dentro de un if-->
            - mantenerlo en la cpu.

            - proceso sigue en estado `RUNNING`.

            - no liberar la cpu.

        - si ({burst_time-current_burst} = 0) (la rafaga en la que esta ha terminado): <!--  hacer esto dentro de un else if-->
            <!-- muy importante-->
            - aumentar en una unidad num_bursts_solicitados_por_proceso <!-- muy importante-->
            <!-- muy importante-->

            - si ({num_bursts_solicitados_por_proceso -num_current_complete_burst} > 0): <!-- aun quedan rafagas por quemar -->
                - proceso cambia a estado ``WAITING``.

                - reiniciar su quantum, con el valor de la cola de la que proviene.

                - se incorpora a la de la que proviene.

                - se libera la cpu con *cpu_process = NULL;

            - si ({num_bursts_solicitados_por_proceso -num_current_complete_burst} = 0)  (el proceso termina su ejecución): <!-- el proceso es interrumpido, hacer esto dentro de un else if-->
                - proceso cambia a estado `FINISHED`.
                - restar una unidad  a Procesos restantes

                - es agregado al arreglo de procesos terminados Finished processes.
                
                - se libera la cpu con *cpu_process = NULL; (recordatorio para fabian, ojo no se termina el scheduler solo queda libre para prepararse para la proxima ejecución).
            

        
3) Ingresar los procesos a las colas segun corresponda:  
    - 3.1 Si un proceso salió ́de la CPU, ingresarlo a la cola que corresponda.  <!-- atendido en el punto 2) -->
    - 3.2  Para cada proceso p, comprobar si tick = T inicio e ingresarlo a la cola High, por lo tanto:
        - se itera sobre los procesos  

    - 3.3  Para cada proceso de la cola Low, revisar si se cumple la condición para subir a la cola High y cambiarlos de cola segun corresponda. Iterar procesos cola low y ver si 2 ∗ Tdeadline < Tactual − TLC P U, pasar a high si corresponde 

4) Si no hay un proceso en estado `RUNNING`, osea que si la cpu esta libre, Seleccionar próximo proceso.

Los procesos de la cola High siempre tienen prioridad por sobre la cola Low.  

 Ingresar el proceso de mayor prioridad en estado READY a la CPU, esto implica ignorar a todos los que se encuentren en estado `WAITING`, sin moverlos de su posición ́ atual.  

 - Si *cpu_process = NULL Revisar cola high: 
    - si size = 0; revisar cola low.  
    - ORDENAR COLA POR PRIORIDAD Interar los procesos extraer el que tenga el mayor valor de :
        (Tactual − TLCPU1) − Tdeadline mayor.
    - Al seleccionar un proceso para ser llevado a la CPU, verifica si es la primera vez que ingresa a RUNNING. Si es así, calcula el response_time de la siguiente forma:
        - si(process->num_current_complete_burst == 0):
            - process->response_time = tick - process->T_INICIO;





