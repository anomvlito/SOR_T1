# Parámetros Iniciales
- **Quantum base (q)**: 2
- **Quantum de la cola High**: 2 * q = 4
- **Quantum de la cola Low**: q = 2

## Procesos

### PROCESS1
- **PID**: 1
- **T_INICIO**: 0
- **T_CPU_BURST**: 2
- **N_BURSTS**: 2
- **IO_WAIT**: 2
- **T_DEADLINE**: 3

### PROCESS2
- **PID**: 2
- **T_INICIO**: 2
- **T_CPU_BURST**: 3
- **N_BURSTS**: 3
- **IO_WAIT**: 3
- **T_DEADLINE**: 4

## Simulación Tick por Tick

### Tick 0
**Eventos**:
- Llegada: PROCESS1 llega al sistema (T_INICIO = 0).
- Cola High: PROCESS1 se encola en la cola High en estado READY.
- CPU: Vacía.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING: CPU está libre.
- Ingresar procesos a las colas: PROCESS1 ya en cola High.
- Seleccionar próximo proceso:
  - PROCESS1 es asignado a la CPU.
  - Estado cambia a RUNNING.
  - Quantum asignado: 4 (cola High).
  - Quantum restante: 4.
  - Burst restante: 2.

**Estados al final del Tick 0**:
- CPU: PROCESS1 ejecutando.
- Cola High: Vacía.
- Cola Low: Vacía.

### Tick 1
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS1 continúa ejecutando.
  - Quantum restante: 4 → 3.
  - Burst restante: 2 → 1.
- Ingresar procesos a las colas: No aplica.
- Seleccionar próximo proceso: CPU ocupada por PROCESS1.

**Estados al final del Tick 1**:
- CPU: PROCESS1 ejecutando.
- Cola High: Vacía.

### Tick 2
**Eventos**:
- Llegada: PROCESS2 llega al sistema (T_INICIO = 2).
- Cola High: PROCESS2 se encola en la cola High en estado READY.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS1 continúa ejecutando.
  - Quantum restante: 3 → 2.
  - Burst restante: 1 → 0.
  - PROCESS1 completa su ráfaga:
    - Cambia a estado WAITING (I/O).
    - IO_WAIT restante: 2.
    - Quantum restante: 2 (no se reinicia).
    - Liberamos la CPU.
- Ingresar procesos a las colas: PROCESS1 en WAITING.
- Seleccionar próximo proceso:
  - PROCESS2 es asignado a la CPU.
  - Estado cambia a RUNNING.
  - Quantum asignado: 4.
  - Quantum restante: 4.
  - Burst restante: 3.

**Estados al final del Tick 2**:
- CPU: PROCESS2 ejecutando.
- Cola High: Vacía.
- PROCESS1: En WAITING, IO_WAIT restante: 2, Quantum restante: 2.

### Tick 3
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY:
  - PROCESS1 en WAITING:
    - IO_WAIT restante: 2 → 1.
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 4 → 3.
  - Burst restante: 3 → 2.
- Ingresar procesos a las colas: No aplica.
- Seleccionar próximo proceso: CPU ocupada por PROCESS2.

**Estados al final del Tick 3**:
- CPU: PROCESS2 ejecutando.
- Cola High: Vacía.
- PROCESS1: En WAITING, IO_WAIT restante: 1, Quantum restante: 2.

### Tick 4
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY:
  - PROCESS1 en WAITING:
    - IO_WAIT restante: 1 → 0.
    - Cambia a estado READY.
    - Se encola en la cola High.
    - Quantum restante: 2 (no se reinicia).
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 3 → 2.
  - Burst restante: 2 → 1.
- Ingresar procesos a las colas: PROCESS1 ya en cola High.
- Seleccionar próximo proceso: CPU ocupada por PROCESS2.

**Estados al final del Tick 4**:
- CPU: PROCESS2 ejecutando.
- Cola High: PROCESS1 en READY, Quantum restante: 2.

### Tick 5
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 2 → 1.
  - Burst restante: 1 → 0.
  - PROCESS2 completa su ráfaga:
    - Cambia a estado WAITING (I/O).
    - IO_WAIT restante: 3.
    - Quantum restante: 1 (no se reinicia).
    - Liberamos la CPU.
- Ingresar procesos a las colas: PROCESS2 en WAITING.
- Seleccionar próximo proceso:
  - PROCESS1 es asignado a la CPU.
  - Estado cambia a RUNNING.
  - Quantum restante: 2 (continuación).
  - Burst restante: 2 (segunda ráfaga).

**Estados al final del Tick 5**:
- CPU: PROCESS1 ejecutando.
- Cola High: Vacía.
- PROCESS2: En WAITING, IO_WAIT restante: 3, Quantum restante: 1.

### Tick 6
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY:
  - PROCESS2 en WAITING:
    - IO_WAIT restante: 3 → 2.
- Actualizar proceso RUNNING:
  - PROCESS1 continúa ejecutando.
  - Quantum restante: 2 → 1.
  - Burst restante: 2 → 1.
- Ingresar procesos a las colas: No aplica.
- Seleccionar próximo proceso: CPU ocupada por PROCESS1.

**Estados al final del Tick 6**:
- CPU: PROCESS1 ejecutando.
- Cola High: Vacía.
- PROCESS2: En WAITING, IO_WAIT restante: 2, Quantum restante: 1.

### Tick 7
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY:
  - PROCESS2 en WAITING:
    - IO_WAIT restante: 2 → 1.
- Actualizar proceso RUNNING:
  - PROCESS1 continúa ejecutando.
  - Quantum restante: 1 → 0.
  - Burst restante: 1 → 0.
  - PROCESS1 completa su ráfaga y todas sus ráfagas totales:
    - Cambia a estado FINISHED.
    - Tiempo de finalización: Tick 7.
    - Liberamos la CPU.
- Ingresar procesos a las colas: PROCESS1 finalizado.
- Seleccionar próximo proceso: No hay procesos READY en colas. CPU permanece libre.

**Estados al final del Tick 7**:
- CPU: Libre.
- Cola High: Vacía.
- PROCESS2: En WAITING, IO_WAIT restante: 1, Quantum restante: 1.
- PROCESS1: En FINISHED.

### Tick 8
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY:
  - PROCESS2 en WAITING:
    - IO_WAIT restante: 1 → 0.
    - Cambia a estado READY.
    - Se encola en la cola High.
    - Quantum restante: 1 (no se reinicia).
- Actualizar proceso RUNNING: CPU está libre.
- Ingresar procesos a las colas: PROCESS2 ya en cola High.
- Seleccionar próximo proceso:
  - PROCESS2 es asignado a la CPU.
  - Estado cambia a RUNNING.
  - Quantum restante: 1 (continuación).
  - Burst restante: 3 (segunda ráfaga).

**Estados al final del Tick 8**:
- CPU: PROCESS2 ejecutando.
- Cola High: Vacía.

### Tick 9
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 1 → 0.
  - Burst restante: 3 → 2.
  - PROCESS2 agota su quantum:
    - Cambia a estado READY.
    - Se mueve a la cola Low.
    - Quantum se reiniciará la próxima vez que sea seleccionado.
    - Liberamos la CPU.
- Ingresar procesos a las colas: PROCESS2 encola en cola Low.
- Seleccionar próximo proceso:
  - No hay procesos en cola High.
  - PROCESS2 es seleccionado de la cola Low.
  - Estado cambia a RUNNING.
  - Quantum asignado: 2 (cola Low).
  - Burst restante: 2.

**Estados al final del Tick 9**:
- CPU: PROCESS2 ejecutando.
- Cola High: Vacía.
- Cola Low: Vacía.

### Tick 10
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 2 → 1.
  - Burst restante: 2 → 1.
- Ingresar procesos a las colas: No aplica.
- Seleccionar próximo proceso: CPU ocupada por PROCESS2.

**Estados al final del Tick 10**:
- CPU: PROCESS2 ejecutando.
- Cola High: Vacía.

### Tick 11
**Eventos**: Ninguno.

**Acciones del Scheduler**:
- Actualizar procesos de WAITING a READY: No aplica.
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 1 → 0.
  - Burst restante: 1 → 0.
  - PROCESS2 completa su ráfaga:
    - Cambia a estado WAITING (I/O).
    - IO_WAIT restante: 3.
    - Quantum restante: 0.
    - Liberamos la CPU.
- Ingresar procesos a las colas: PROCESS2 en WAITING.
- Seleccionar próximo proceso: No hay procesos READY. CPU permanece libre.

**Estados al final del Tick 11**:
- CPU: Libre.
- PROCESS2: En WAITING, IO_WAIT restante: 3, Quantum restante: 0.

### Tick 12 - Tick 14
**Eventos**: Ninguno.

**Acciones del Scheduler en cada tick**:
- Actualizar procesos de WAITING a READY:
  - Decrementar IO_WAIT restante de PROCESS2.
    - Tick 12: 3 → 2.
    - Tick 13: 2 → 1.
    - Tick 14: 1 → 0.
  - En Tick 14, PROCESS2 cambia a estado READY y se encola en cola Low.
- Actualizar proceso RUNNING: CPU sigue libre.
- Ingresar procesos a las colas: PROCESS2 en cola Low.
- Seleccionar próximo proceso:
  - En Tick 14, PROCESS2 es asignado a la CPU.
  - Estado cambia a RUNNING.
  - Quantum asignado: 2 (cola Low).
  - Burst restante: 3 (tercera ráfaga).

**Estados al final del Tick 14**:
- CPU: PROCESS2 ejecutando.
- Cola Low: Vacía.

### Tick 15
**Acciones del Scheduler**:
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 2 → 1.
  - Burst restante: 3 → 2.

**Estados al final del Tick 15**:
- CPU: PROCESS2 ejecutando.

### Tick 16
**Acciones del Scheduler**:
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 1 → 0.
  - Burst restante: 2 → 1.
  - PROCESS2 agota su quantum:
    - Cambia a estado READY.
    - Se mantiene en cola Low.
    - Liberamos la CPU.
- Seleccionar próximo proceso:
  - PROCESS2 es asignado nuevamente a la CPU.
  - Estado cambia a RUNNING.
  - Quantum asignado: 2 (cola Low).
  - Burst restante: 1.

**Estados al final del Tick 16**:
- CPU: PROCESS2 ejecutando.

### Tick 17
**Acciones del Scheduler**:
- Actualizar proceso RUNNING:
  - PROCESS2 continúa ejecutando.
  - Quantum restante: 2 → 1.
  - Burst restante: 1 → 0.
  - PROCESS2 completa su ráfaga y todas sus ráfagas totales:
    - Cambia a estado FINISHED.
    - Tiempo de finalización: Tick 17.
    - Liberamos la CPU.

**Estados al final del Tick 17**:
- CPU: Libre.
- PROCESS2: En FINISHED.

## Cálculo de Estadísticas

### PROCESS1
- **Turnaround Time**:
  - Tiempo de finalización: Tick 7.
  - T_INICIO: 0.
  - Turnaround Time: 7 - 0 = 7 ticks.

- **Response Time**:
  - Primera vez en CPU: Tick 0.
  - Response Time: 0 - 0 = 0 ticks.

- **Waiting Time**:
  - Tiempo en READY:
    - Tick 4: Entra a READY, es seleccionado en Tick 5.
    - Tiempo en READY: 1 tick.
  - Total Waiting Time: 1 tick.

- **Número de Interrupciones**:
  - No fue interrumpido por agotamiento de quantum.
  - Interrupciones: 0.

- **Tiempo ejecutado después del deadline**:
  - T_DEADLINE: 3.
  - Ejecutó en ticks 5, 6, 7.
  - Tiempo ejecutado después del deadline: 3 ticks.

### PROCESS2
- **Turnaround Time**:
  - Tiempo de finalización: Tick 17.
  - T_INICIO: 2.
  - Turnaround Time: 17 - 2 = 15 ticks.

- **Response Time**:
  - Primera vez en CPU: Tick 2.
  - Response Time: 2 - 2 = 0 ticks.

- **Waiting Time**:
  - Tiempo en READY:
    - Tick 9: Cambia a READY en cola Low, es seleccionado en Tick 9.
    - Tick 16: Cambia a READY, es seleccionado en el mismo tick.
    - Tiempo en READY: 0 ticks.
  - Total Waiting Time: 0 ticks (siempre fue seleccionado inmediatamente).

- **Número de Interrupciones**:
  - Fue interrumpido 2 veces por agotamiento de quantum (Ticks 9 y 16).
  - Interrupciones: 2.

- **Tiempo ejecutado después del deadline**:
  - T_DEADLINE: 4.
  - Ejecutó desde Tick 5 en adelante.
  - Tiempo ejecutado después del deadline: 17 - 5 = 12 ticks.

## Observaciones
- Quantum no se reinicia cuando el proceso cede la CPU; se mantiene el restante.
- Al agotar el quantum, el proceso se mueve a la cola Low y el quantum se reinicia al asignarse nuevamente.
- La promoción a cola High no aplicó en este caso, ya que no se cumplió la condición 2 * T_DEADLINE < T_ACTUAL - T_LCPU.