# SOR_T1
# Tarea: Simulación de Procesos

Para esta tarea, asumimos que no nos están pidiendo ejecutar los procesos con respecto al timer del computador o algo parecido, nosotros simularemos el tiempo. Proponemos correr el programa en un bucle tal que itere sobre una variable entera `int tick = 0`, la cual incrementará en cada iteración.

## **Apuntes/Estrategia**

### Consideraciones importantes explícitas y no explícitas en el enunciado:

- Recordar que si un proceso consume su quantum, este pasa a la cola Low.
- Cuando un proceso cambia de una cola a otra, el valor del quantum asociado se debe reiniciar con el valor del quantum de la nueva cola. Por ejemplo, si un proceso en la cola High está ejecutándose en la CPU y se le acaba el quantum disponible, cede la CPU y pasa a la cola Low, su quantum se reinicia al valor del quantum de la cola Low.
- Recordar que para que un proceso sea ejecutado desde la cola Low, los demás procesos en la cola High tienen que haber terminado su ejecución por completo.
- Tener en consideración que si se están ejecutando procesos en la cola Low, hay que estar atentos si es necesario pasar estos procesos a la cola High si se cumple para este proceso que, `2 * Tdeadline < Tactual − TLCPU`, este proceso sube a la cola High, y en el próximo bloque tick este proceso tendrá prioridad por sobre los otros procesos.

### Estado `READY`:

- Si un proceso llega por primera vez al scheduler, lo hace en estado `READY`, además, por enunciado va directamente a la cola High.
- Si existe otro proceso ocupando la **CPU** y no la cede, los procesos en estado `READY` al no ser seleccionados para ser llevados y ejecutados en CPU, se mantendrán en estado `READY`, además:
    - Se le suma una unidad de `WAITING` time.
    
    - Se le suma una unidad de response time (el tiempo que tardó en entrar por primera vez a la CPU), hasta que el valor de current_burst.
    - Si el proceso pasa de estado `READY` a `RUNNING`, este es seleccionado para ser llevado a la CPU, pasando a estado `RUNNING`, se le resta una unidad de `WAITING` time ya que por construcción del scheduler, antes de este análisis se actualizan todos los procesos que están en estado ready mientras la CPU está ocupada.

### Estado `WAITING`:

- Un proceso no puede estar en estado `WAITING` sin haber ejecutado por completo su primera ráfaga de CPU.
- Si un proceso ya ejecutó "quemó" su primera ráfaga de CPU por completo, cuando esté en estado `WAITING`, pasará que:
    - Se le suma una unidad de `WAITING` time.
    - Se le suma 1 al current_io_wait_time y al igualarse con io_wait_time se pasa a estado `READY`.
    - Si justo cuando pasa a estado `READY` y este es seleccionado para ser llevado a la CPU, pasando a estado `RUNNING`, el mismo scheduler es capaz de controlar las unidades de `WAITING` time sumadas en cierta parte del código, esto está referenciado en el tercer elemento listado, del segundo elemento listados de "sobre el estado ready".

### Tiempo de espera `IO_WAIT`:

- Entre ráfagas de CPU se espera input/output `IO_WAIT` antes de poder ser elegido nuevamente para pasar a la **CPU**.

## **Construcción de structs**

### **Funciones asociadas a la CPU**

- Modifica el quantum de cada proceso que entra a la CPU, el cual está designado desde la cola de la que proviene antes de entrar a la CPU.

### **Process**

- **Nombre**: `char nombre[10]`
- **Pid**: `int pid`
- **Burst time**: `int burst_time` // Tiempo de ejecución por ráfaga
- **Num bursts solicitados por proceso**: `int num_bursts_solicitados_por_proceso`
- **Io wait time**: `int io_wait_time`
- **Deadline**: `int deadline`
- **Estado**: `estado_t estado`
- **Quantum**: `int quantum`
- **Queue**: `char queue[10]`
- **Interrupciones**: `int interrupciones`
- **Turnaround time**: `int turnaround_time` // término – t inicio
- **Response time**: `int response_time` // tiempo que tardó en entrar por primera vez a la CPU
- **Waiting time**: `int waiting_time` // número de veces que se encuentre en estado `WAITING` y ready (sin contar ready cuando pasa a running)
- **Suma deadline**: `int deadline_sum`
- **Num current complete burst**: `int num_current_complete_burst` // para saber si es su primera ráfaga, y su última para pasar a Finished
- **Current Burst**: `int current_burst` // para saber si sale de la CPU
- **Current io wait time**: `int current_io_wait_time`

### **Funciones process**

- **Create process**:
    - Se guarda el nuevo proceso con sus atributos.
    - Move to high queue.
    - Estado ready por defecto cuando es creado.

### **Queue**

Se va a ordenar la cola por prioridad, con insertion sort.

### **Funciones asociadas a cola de prioridad**

#### Insert High Queue

- **Process Quantum**: `2 * quantum`
- **Action**: Agregar a la cola High Queue

#### Insert Low Queue

- **Process Quantum**: `quantum`
- **Action**: Agregar a la cola Low Queue

#### Ordenar procesos con estado `READY`

- Utilizar insertion sort, seleccionando solo los procesos con estado `READY` de la cola.
- El con mayor prioridad es el que tenga mayor:
    - `(Tactual − TLCPU1) − Tdeadline`.
- Si hay un empate en el deadline, se debe escoger el que tenga menor PID.

Un proceso en estado `RUNNING` solo sale de la CPU si se acaba su quantum o cede la CPU.

## **Modelamiento/ flujo del main:**

### 1) Se lee la primera línea de input:

- Se carga el quantum *q* asociado al scheduler, el cual se utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de quantum que la cola Low: Quantum High = 2q, Quantum Low = q.
- Procesos restantes = N procesos.

### 2) Se leen las líneas de input posteriores a la primera, donde encontraremos la información asociada a los procesos.

- Se crean los procesos correspondientes al `struct process`, aún no se ingresan al Scheduler.
- Hay que tener en consideración que cada proceso tiene un tiempo `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por primera vez.
- Se crean las colas high.
- Se crean las colas low.
- Se llama y crea la lista de procesos finalizados, con sus funciones correspondientes.
- Se crea `Process *cpu_process = NULL;` que simula la CPU.

### 3) Se crea un bucle que simula el paso del tiempo y en cada iteración se aumenta en una unidad el tick que comienza en 0.

- Se ejecuta el `Scheduler`.
- Verificar si todos los procesos han terminado, si es así se genera un archivo output "informe" tal como se pide en el enunciado, tal que:
    - Si terminaron, verificando Procesos restantes = 0:
        - Break al while.
    - Si no terminaron:
        - Aumentar en una unidad el tick para la nueva iteración del bucle.

### 4) Si se rompió el bucle es porque terminaron de ejecutarse los procesos, por lo tanto ejecutar:

- `walk_in_finished_processes(finished_list)`, aquí crea el informe correspondiente que escribirá en el csv como output, esto se hace a través de la función `walk_in_finished_processes`, la cual recorrerá todos los procesos terminados y tomará los atributos de cada proceso.

### 5) Liberar la memoria

## **Scheduler**

Creamos la función Scheduler que controla todo lo que pasa en cada intervalo tick de tiempo.

- Comparamos prioridad de procesos en esta prioridad:
    - Solo comparamos elementos cuando la CPU está libre.
    - Solo comparamos si las colas high y low tienen len mayor a 0 (no están vacías).
    - Solo comparamos elementos de la cola low si la cola high tiene len igual a 0 (está vacía).
    - Si la cola a comparar no está vacía, solo se comparan los procesos en estado ready.

### Manejo del `waiting_time`:


### **Función scheduler**

Por cada tick, el scheduler realiza las siguientes tareas, en el orden indicado:

1. **Actualizar los procesos que hayan terminado su tiempo de espera de I/O de `WAITING` a `READY`**:
     - Iterar sobre cola high y low y revisar `io_wait_time - current_io_wait_time`, si esto llega a ser = 0 pasará a estado `READY`.

2. **Si hay un proceso en estado `RUNNING`, actualizar su estado según corresponda**:
     - Actualizar sus variables quantum, current burst, num_current_complete_burst y estado (según burst= wait o quantum=ready).

     - Actualización de t_LCPU: Cuando el proceso cambia de estado (RUNNING a READY, WAITING o FINISHED), actualizamos t_LCPU con el tick actual (tick) para reflejar el último momento en que el proceso estuvo en la CPU.
     - Si no queda quantum:
         - Si (`burst_time - current_burst` > 0):
             - Se suma una unidad a interrupciones.
             - Proceso sigue en estado `READY`.
             - Reiniciar su quantum, con el valor de la cola low.
             - Se ingresa a la cola low.
             - Se libera la CPU con `*cpu_process = NULL`.
         - Si (`burst_time - current_burst` = 0):
             - Aumentar en una unidad `num_current_complete_burst`.
             - Si (`num_bursts_solicitados_por_proceso - num_current_complete_burst` > 0):
                 - Proceso cambia a estado `WAITING`.
                 - Reiniciar su quantum, con el valor de la cola de la que proviene.
                 - Se incorpora a la de la que proviene.
                 - Se libera la CPU con `*cpu_process = NULL`.
             - Si (`num_bursts_solicitados_por_procesos - num_current_complete_burst` = 0):
                 - Proceso cambia a estado `FINISHED`.
                 - Restar una unidad a Procesos restantes.
                 - Es agregado al arreglo de procesos terminados Finished processes.
                 - Se libera la CPU con `*cpu_process = NULL`.
     - Si le queda quantum:
         - Si (`burst_time - current_burst` > 0):
             - Mantenerlo en la CPU.
             - Proceso sigue en estado `RUNNING`.
             - No liberar la CPU.
         - Si (`burst_time - current_burst` = 0):
             - Aumentar en una unidad `num_bursts_solicitados_por_proceso`.
             - Si (`num_bursts_solicitados_por_proceso - num_current_complete_burst` > 0):
                 - Proceso cambia a estado `WAITING`.
                 - Reiniciar su quantum, con el valor de la cola de la que proviene.
                 - Se incorpora a la de la que proviene.
                 - Se libera la CPU con `*cpu_process = NULL`.
             - Si (`num_bursts_solicitados_por_proceso - num_current_complete_burst` = 0):
                 - Proceso cambia a estado `FINISHED`.
                 - Restar una unidad a Procesos restantes.
                 - Es agregado al arreglo de procesos terminados Finished processes.
                 - Se libera la CPU con `*cpu_process = NULL`.

3. **Ingresar los procesos a las colas según corresponda**:
     - Si un proceso salió de la CPU, ingresarlo a la cola que corresponda.
     - Para cada proceso p, comprobar si `tick = T inicio` e ingresarlo a la cola High.
     - Para cada proceso de la cola Low, revisar si se cumple la condición para subir a la cola High y cambiarlos de cola según corresponda.

4. **Si no hay un proceso en estado `RUNNING`, seleccionar próximo proceso**:
     - Los procesos de la cola High siempre tienen prioridad por sobre la cola Low.
     - Ingresar el proceso de mayor prioridad en estado `READY` a la CPU, esto implica ignorar a todos los que se encuentren en estado `WAITING`, sin moverlos de su posición actual.
     - Si `*cpu_process = NULL`, revisar cola high:
         - Si `size = 0`, revisar cola low.
         - Ordenar cola por prioridad, iterar los procesos y extraer el que tenga el mayor valor de `(Tactual − TLCPU1) − Tdeadline`.
         - Al seleccionar un proceso para ser llevado a la CPU, verifica si es la primera vez que ingresa a `RUNNING`. Si es así, calcula el `response_time` de la siguiente forma:
             - Si (`process->num_current_complete_burst == 0`):
                 - `process->response_time = tick - process->T_INICIO`.

