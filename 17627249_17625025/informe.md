# 🖥️ SOR_T1
## 📝 Tarea: Simulación de Procesos

Para esta tarea, asumimos que no nos están pidiendo ejecutar los procesos con respecto al timer del computador 🖱️. Simularemos el tiempo ⏱️ corriendo el programa en un bucle que iterará sobre una variable entera `int tick = 0`, la cual incrementará en cada iteración.

---

## 📌 Apuntes/Estrategia

### 🧠 Consideraciones importantes explícitas y no explícitas en el enunciado:

- 🔄 **Recordar** que si un proceso consume su quantum, este pasa a la cola Low ⬇️.
- Cuando un proceso cambia de una cola a otra, el valor del quantum asociado se debe reiniciar con el valor del quantum de la nueva cola 🔄.
- 🟢 **High Priority Rule**: Un proceso debe haber terminado su ejecución completa para que un proceso de la cola Low sea ejecutado.
- 🟠 Si un proceso cumple con la condición `2 * Tdeadline < Tactual − TLCPU`, este sube a la cola High 🚀 y tiene prioridad sobre otros procesos.

---

## 🎯 Estado `READY`

- Al entrar por primera vez, un proceso está en estado `READY` 🟢 y va a la cola High.
- Si la CPU está ocupada y un proceso no es seleccionado para ejecutarse:
  - ➕ Suma una unidad de `WAITING` time 🕰️.
  - ➕ Suma una unidad de `response time` 🕰️.

---

## 💻 Construcción de structs

### 🛠️ Funciones asociadas a la CPU
- **Modifica el quantum** de cada proceso según la cola de origen antes de entrar a la CPU.

### 🗃️ Process
Atributos del struct `Process` 📋:

- **Nombre**: `char nombre[10]`
- **Pid**: `int pid`
- **Burst time**: `int burst_time`
- **Num bursts solicitados por proceso**: `int num_bursts_solicitados_por_proceso`
- **Io wait time**: `int io_wait_time`
- **Deadline**: `int deadline`
- **Estado**: `estado_t estado`
- **Quantum**: `int quantum`
- **Queue**: `char queue[10]`
- **Interrupciones**: `int interrupciones`
- **Turnaround time**: `int turnaround_time`
- **Response time**: `int response_time`
- **Waiting time**: `int waiting_time`
- **Suma deadline**: `int deadline_sum`
- **Num current complete burst**: `int num_current_complete_burst`
- **Current Burst**: `int current_burst`
- **Current io wait time**: `int current_io_wait_time`

---

## 🔧 Modelamiento/flujo del main

### 1️⃣ Se lee la primera línea de input:
- Se carga el quantum asociado al scheduler 🗓️.
- Procesos restantes = N procesos.

### 2️⃣ Se leen las líneas de input posteriores:
- Se crean los procesos y se ingresan al scheduler.

### 3️⃣ Bucle de simulación del tiempo 🕰️:
- Cada iteración incrementa el tick. Verifica si los procesos han terminado 🛑.

### 4️⃣ Generación del informe 📝:
- Ejecuta `walk_in_finished_processes(finished_list)` y genera el CSV.

### 5️⃣ Liberación de memoria 🧹.
- se libera la memoria como corresponde, verificar con valgrind

---



## 🔄 Función Scheduler

1. **Actualiza procesos `WAITING` a `READY`** 📈.
2. **Gestiona el proceso en `RUNNING`** 🏃‍♂️.
3. **Gestiona el ingreso de procesos a las colas** 📤.
4. **Selecciona el próximo proceso `RUNNING`** 🎯.



## Aspectos a mejorar:
1. El codigo necesita más ajustes para su correcta implementacion en algunos aspectos, hay parte de la logica que no manejamos bien, sobre todo por una presunta compensación del tiempo de espera, la cual se puede ver comentada en algunas partes del codigo. 
2. Es necesario hacer un buen calculo de las variables que incolucran el ultimo tiempo en que la cpu fue ocupada, al parecer ese es uno de nuestros mayores problemas.


---

## ⚡ Ejecución del código

Para ejecutar el código, utiliza el siguiente comando:

```bash
valgrind --leak-check=full --track-origins=yes ./lrscheduler t1.txt output.csv
```