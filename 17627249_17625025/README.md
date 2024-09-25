# SOR_T1

Para esta tarea, asumimos que no nos están pidiendo ejecutar los procesos con respecto al timer del computador o algo parecido, nosotros simularemos el tiempo. Proponemos correr el programa en un bucle tal que itere sobre una variable entera `int tick = 0`, la cual incrementará en cada iteración.

# **Apuntes/Estrategia**

# Consideraciones importantes explícitas y no explícitas en el enunciado:

- Recordar que si un proceso consume su quantum, este pasa a la cola Low. 

- Cuando un proceso cambia de una cola a otra, el valor del quantum asociado se debe reiniciar con el valor del quantum de la nueva cola. Por ejemplo, si un proceso en la cola High está ejecutándose en la CPU y se le acaba el quantum disponible, cede la CPU y pasa a la cola Low, su quantum se reinicia al valor del quantum de la cola Low.

- Recordar que para que un proceso sea ejecutado desde la cola Low, los demás procesos en la cola High tienen que haber terminado su ejecución por completo.

- Tener en consideración que si se están ejecutando procesos en la cola Low, hay que estar atentos si es necesario pasar estos procesos a la cola High si se cumple para este proceso que, `2 * Tdeadline < Tactual − TLCPU`, este proceso sube a la cola High, y en el próximo bloque tick este proceso tendrá prioridad por sobre los otros procesos.

### - Sobre el estado `Ready`:

- Si un proceso llega por primera vez al scheduler, lo hace en estado `Ready`, además, por enunciado va directamente a la cola High. 

- Si existe otro proceso ocupando la **CPU** y no la cede, los procesos en estado `Ready` al no ser seleccionado para ser llevados y ejecutados en CPU, se mantiendran en estado `Ready`, ademas:

    - se le suma una unidad de Waiting time.

    - se le suma una unidad de response time (el tiempo que tardo en entrar por primera vez a la CPU), hasta que el valor de current_burst 

    - si el proceso pasa de estado `Ready` a `Running`, este es seleccionado para ser llevado a la cpu, pasando a estado , se le reste una unidad de waiting time ya que por construccion del scheduler, antes de este analisis se actualizan todos los procesos que estan en estado ready mientras la cpu esta ocupada.

### - Sobre el estado `Waiting`:

- Un proceso no puede estar en estado `Waiting` sin haber ejecutado por completo su primera ráfaga de CPU.

- Si un proceso ya ejecuto "quemo" su primera rafaga de cpu por completo, cuando este en estado `Waiting`, pasara que:

    - se le suma una unidad de Waiting time.

    - se le suma 1 y al current_io_wait_time y al igualarse con io_wait_time se pasa a estado `Ready`.

    - Si justo cuando pasa a estado `Ready` y este es seleccionado para ser llevado a la cpu, pasando a estado `Running`, el mismo scheduler es capas de controlar las unidades de waiting time sumadas en cierta parte del codigo, esto esta referenciado en el tercer elemento listado, del segundo elemento listados de "sobre el estado ready". 


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
- **N bursts** `int num_bursts`
- **Io wait time** `int io_wait_time`
- **Deadline** `int deadline`
- **Estado** `estado_t estado`
- **Quantum** `int quantum`
- **Queue** `char queue[10]`
- **Interrupciones** `int interrupciones`
- **Turnaround time** `int turnaround_time` // termino – t inicio
- **Response time** `int response_time` // tiempo que tardo en entrar por primera vez a la CPU
- **Waiting time** `int waiting_time` // numero de veces que se encuentre en estado waiting y ready (sin contar ready cuando pasa a running)
- **Suma deadline** `int deadline_sum`
- **N Bursts restantes** `int n_burst_restante` // para saber si es su primera rafaga, y su ultima para pasar a finished
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

#  Rosario, favor en esta parte explicar haces uso de heap para el manejo de la prioridad, en el sentido más practico 

## **Funciones asociadas a cola de prioridad**

### ORDENAR COLA POR PRIORIDAD: 
   
    - insertion sort, seleccionando solo los procesos con estado `Ready`. Y contador cada vez q se selecciona un proceso. Si contador = 0, no hay ready, si es 0<, current process in cpu = process, process in cpu = true.



# **Modelamiento/ flujo del main: **

## 1) Se lee la primera línea de input:

- Se carga el quantum *q* asociado al scheduler, el cual se utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de quantum que la cola Low: Quantum High = 2q, Quantum Low = q.

## 2) Se leen las líneas de input posteriores a la primera, donde encontraremos la información asociada a los procesos.

- Se crean los procesos correspondientes al `struct process`, aun no se ingresan al Scheduler.
- Hay que tener en consideración que cada proceso tiene un tiempo `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por primera vez.

- se llama y crea la lista de procesos finalizados, con sus funciones correspondientes

## 3) Se inicializa el `Scheduler`.




## 4) Procesos empiezan a "competir" por el uso de la cpu


## 5) Se completan de ejecutar todos los procesos y se genera un archivo outuput "informe" tal como se pide en el enunciado




############################################### completar
############################################### completar
############################################### completar
############################################### completar
############################################### completar


## x) Al no quedar procesos en las colas se asume que estos estan en la lista de procesos finalizados.

    - se crea el informe correspondiente que escribira en el csv como output, esto se hace a traves de la funcion  walk_in_finished_processes, la cual recorrera todos los procesos terminados y tomara los atributos de cada proceso 


## Scheduler

Creeamos la función Scheduler que controla todo lo que pasa en cada intervalo tick de tiempo. 

<!-- - si se libera la cpu tener atención con los procesos que terminan su espera i/o_waiting, ya que pasarian de estado waiting a ready y podrian tener mayor prioridad que otros elementos en la misma cola que ya estaban en estado ready. -->

- Comparamos prioridad de procesos en esta prioridad:

    - solo comparamos elementos cuando la cpu esta libre.

    - solo comparamos si las colas high y low tienen len mayor a 0 (no estan vacias).

    - solo comparamos elementos de la cola low si la cola high tienen len igual a 0 (esta vacia).

    - si la cola a comparar no esta vacia, solo se comparan los procesos en estado ready


## **Flujo del cheduler**

Por cada tick, el scheduler realiza las siguientes tareas, en el orden indicado:  

1) Actualizar los procesos que hayan terminado su tiempo de espera de I/O de WAITING a READY.  

    -  Iterar sobre cola high y low y revisar io_wait_time- current io_wait_time , si esto llega a ser = 0  pasara a estado ready.
        - esto se hace como fue mencionado anteriormente "se le suma 1 y al current_io_wait_time y al igualarse con io_wait_time se pasa a estado `Ready`".

2) Si hay un proceso en estado RUNNING, actualizar su estado según corresponda. Esto podría incluir sacarlo de la CPU si su quantum ha expirado o ha terminado su rafaga de ejecucio. Esto pasa cuando (*cpu_process != NULL) es true (la cpu esta ocupada) y por consiguiente:

    - Actualizar sus variables quantum, current burst, num_bursts y estado(según burst= wait o quantum=ready).

    - se resta una unidad de quantum
    - se suma una unidad a current burst
    

    - Si no queda quantum: <!--if-->
        - si ({burst_time-current_burst} > 0) (la rafaga en la que esta no ha terminado): <!--if-->
            - sigue en estado `Running`.
            - reiniciar su quantum, con el valor del quantum asociado a la cola low.
            - llevarlo a la cola low
        - si ({burst_time-current_burst} = 0) (la rafaga en la que esta ha terminado): <!--else if-->

    - Si le queda quantum: <!--else if-->
        - si ({burst_time-current_burst} > 0) (la rafaga en la que esta no ha terminado): <!--if-->
            - mantenerlo en la cpu.
            - sigue en estado `Running`.
        - si ({burst_time-current_burst} = 0) (la rafaga en la que esta ha terminado): <!--else if-->
            - si ({num_bursts -n_burst} > 0)  (el proceso es interrumpido):  <!--if-->
                <!-- ############### -->
                - se suma una unidad a interrupciones.
                - sigue en estado `Ready`.
                - se ingresa a la cola de la que proviene.
                - podriamos agregar el atributo ultima cola visitada.
                <!-- ############### -->
            - si ({num_bursts -n_burst} = 0)  (el proceso termina su ejecución): <!--else if-->
                - cambia a estado `FINISHED`.
                - es agregado al arreglo de procesos terminados finished processes.
                - se libera la cpu con *cpu_process = NULL; (recordatorio para fabian, ojo no se termina el scheduler solo queda libre para prepararse para la proxima ejecución).
            



    - si no le queda quantum 
    - si la rafaga en la que esta no ha terminado y se acabo su quantum, el proceso es interrumpido, por lo tanto:
        - sumar una unidad a interrupciones. 
        - mover a cola low, por haber consumido su quantum.
            
    - Ver si ha quemando todas sus rafagas, osea que si n_burst_restante = 0 hay que actualizar `*cpu_process = NULL` y ademas llevar el proceso al arreglo de procesos finalizados finished_processes.
        
3)

4)


