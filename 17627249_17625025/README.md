# SOR_T1

Considerar que para esta tarea, no nos están pidiendo ejecutar los procesos con respecto al timer del computador, nosotros simularemos el tiempo. Proponemos correr el programa en un bucle tal que itere sobre una variable entera `int tick = 0`, la cual incrementará en cada iteración.

# **Apuntes/Estrategia**

# Consideraciones importantes no explícitas en el enunciado:

- Recordar que si un proceso consume todo su quantum, este pasa a la cola Low. Cuando un proceso cambia de una cola a otra, el valor del quantum asociado se debe reiniciar con el valor del quantum de la nueva cola. Por ejemplo, si un proceso en la cola High está ejecutándose en la CPU y se le acaba el quantum disponible, cede la CPU y pasa a la cola Low, su quantum se reinicia al valor del quantum de la cola Low.

- Recordar que para que un proceso sea ejecutado desde la cola Low, los demás procesos en la cola High tienen que haber terminado su ejecución por completo.

- Tener en consideración que si se están ejecutando procesos en la cola Low, hay que estar atentos si es necesario pasar estos procesos a la cola High si se cumple para este proceso que, `2 * Tdeadline < Tactual − TLCPU`, este proceso sube a la cola High, y en el próximo bloque tick este proceso tendrá prioridad por sobre los otros procesos.

### - Sobre el estado `Waiting`:

- Si un proceso llega por primera vez al scheduler, lo hace en estado `Ready`, además, por enunciado va directamente a la cola High. Teniendo lo anterior en consideración, si existe otro proceso ocupando la **CPU**, el proceso del cual estábamos hablando, al no ser ejecutado en el intervalo correspondiente a su tick de llegada, finaliza en estado `Waiting`.

- Un proceso en estado `Waiting` sin haber ejecutado por completo su primera ráfaga de CPU, no "gasta" su tiempo de espera ***IO_WAIT***.

### Sobre el tiempo de espera `IO_WAIT`:

- Una vez ejecutada por completo su primera ráfaga de **CPU**, al entrar a otro periodo de tiempo tick estando en estado `Waiting`, este empieza a consumir su tiempo de espera input/output `IO_WAIT`, y lo hace por completo antes de poder ser elegido nuevamente para pasar a la **CPU**.

# **Construcción de structs**


## Scheduler

Crear una función que controle lo que pasa en cada intervalo tick de tiempo. En esta función se van actualizando cada uno de los structs, haciendo uso de punteros de forma inteligente que apunte a los elementos de mayor prioridad de los queues y a cada uno de sus elementos, dependiendo de su estado.

- si el proceso esta en estado {READY}

- En cada intervalo tick, revisar todos los procesos y actualizar sus parametros si es necesario:
    - si el proceso esta en estado finished, saltarlo.
    - Si el proceso esta en estado waiting y su atributo primera_rafaga_de_cpu = {True}, entonces cuando se le apunte en cada ciclo de tick para actualizar sus parametros, restar una unidad de tiempo de espera i/o_waiting.
    - si el proceso esta en estado waiting y su atributo primera_rafaga_de_cpu = {False}, entonces dejar sus atributos tal y como estan, probablemente el proceso fue interrumpido antes de poder terminar su primera rafaga de cpu



## **Funciones asociadas a la CPU**
- Modifica el quantum de cada proceso que entra a la cpu, el cual está designado desde la cola de la que proviene antes de entrar a la CPU.
- Modifica el `cpu_burst` asociado al ciclo, tener en cuenta que un proceso puede tener más de una ráfaga de burst.


## Process 

- cada proceso deber tener el atributo primera_rafaga_de_cpu = true or false
- cada proceso deber tener el atributo q_cola_de_origen, el cual no puede decrementar a diferencia de quantum restante,
- cada proceso debe tener quantum_restante el cual dene estar asociado a cola actual
- los procesos dependiendo de su estado, si ya quemaron su primera rafaga de cpu, ir actualizando cada unos de sus atributos.
- si el proceso cambia de cola, cambiar q_cola_de_origen y reiniciar quantum

## **Funciones asociadas al Scheduler**

- Crear una función que controla la prioridad entre procesos para su ejecución.

    - Esta sera ejecutada por el scheduler al principio de cada intervalo de tiempo Tick.

    - El elemento de mayor prioridad en el heap siempre que pueda estara en estado {READY}, ahora si en la cola high la raiz del heap no esta en estado {READY} se asumira que los demas procesos de la cola no estan en estado ready. 

    - Comparar prioridad de procesos en la misma cola solo si estan en estado ready, si tienen la misma desempatar como dice en el enunciado.

    - Tener atención con los procesos que terminan su espera i/o_waiting, ya que pasarian de estado waiting a ready y podrian tener mayor prioridad que otros elementos en la misma cola que ya estaban en estado ready. Para esto actualizar prioridades en cada nuevo intervalo Tick de tiempo


- Crear una función que controla si un proceso en la cola low tiene que pasar a la cola high.

- Crear una funcion que controle el tiempo de espera i/o para cada uno de los procesos 


## Queue 

#  Rosario, favor en esta parte explicar haces uso de heap para el manejo de la prioridad, en el sentido más practico 

## **Funciones asociadas a heaps-queltehues**



# **Ejecución/Estrategia**

## 1) Se lee la primera línea de input:

- Se carga el quantum *q* asociado al scheduler, el cual se utilizará en ambas colas. Por enunciado, la cola High tendrá el doble de quantum que la cola Low: Quantum High = 2q, Quantum Low = q.

## 2) Se leen las líneas de input posteriores a la primera, donde encontraremos la información asociada a los procesos.

- Se crean los procesos correspondientes al `struct process`, aun no se ingresan al Scheduler.
- Hay que tener en consideración que cada proceso tiene un tiempo `T_INICIO`, el cual es el tiempo en el que el proceso entra a la cola por primera vez.

## 3) Se inicializa el `Scheduler`.

- se llama y crea la lista de procesos finalizados, con sus funciones correspondientes

- De los 5 tests proporcionados, el primer proceso que llega siempre es en el tiempo 0. Según las issues, lo que se da a entender y lo que asumiremos para esta tarea es que el primer proceso entra en estado `Ready` a la cola `High`. Como la CPU está libre, este pasará directamente a la CPU, cambiando de estado a `Running`.

    - Este proceso ocupará una unidad de `cpu_burst`, la que corresponde a su primera ráfaga de ejecución en CPU.
    - Este proceso ocupará una unidad del quantum asociado a la cola de la que proviene, la cual es la cola `High`.
    - ***Todo lo anterior ocurre en el primer bloque tick, o sea, pasa entre el intervalo 0 y 1 de tiempo tick.***
    - ***Por lo mismo, es importante tener atención a cómo van pasando los bloques tick de ejecución.***

    .

## 4) Procesos empiezan a "competir" por el uso de la cpu


## 5) Se completan de ejecutar todos los procesos y se genera un archivo outuput "informe" tal como se pide en el enunciado




############################################### completar
############################################### completar
############################################### completar
############################################### completar
############################################### completar


## x) Al no quedar procesos en las colas se asume que estos estan en la lista de procesos finalizados.

    - se crea el informe correspondiente que escribira en el csv como output, esto se hace a traves de la funcion  walk_in_finished_processes, la cual recorrera todos los procesos terminados y tomara los atributos de cada proceso 



