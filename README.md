## Mini-Mini Proyecto - Programacion Paralela

### Descripcion
El objetivo de este mini proyecto es desarrollar una solución para la simulacion de trafico usando agentes que representan vehículos, semáforos e intersecciones. Esta debe ser paralelizada y justificada con OpenMP de manera que la actualización de los datos se ejecute de manera simultanea y que provea una ventaja de tiempo con respecto a una versión secuencial del mismo algoritmo. Cada agente también, debera poseer acciones como cambiar de estado para un semáforo, o avanzar en una intersección para automóviles. Se incentiva la evaluación de diferentes abordajes de paralelización para poder encontrar una topología que optimice el uso de recursos. 
### Correrlo
#### Simulacion Paralela:<br>
El parametro verbose para correr la simulacion se usa para determinar que tanto se imprimiran los resultados de la simulacion
- 0 -> Solo tiempo de ejecucion
- 1 -> se almacenan los resultados en la carpeta ./logs ya sea para paralela o secuencial
- 2 -> imprime en consola
```
// Compilar
gcc -fopenmp simulacion_paralela.c -o sim 

// Correr
./sim <verbose>
```

#### Simulacion Paralela:<br>
Verbose es igual
```
// Compilar
gcc -fopenmp simulacion_secuencial.c -o seq 

// Correr
./seq <verbose>
```