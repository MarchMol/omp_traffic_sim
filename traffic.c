#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CARS 20
#define NUM_TRAFFIC_LIGHT 4
#define NUM_INTERSECTION 5

#define MAX_LINE 128

// Carro
struct Car{
    int id;
    int position;
};

// Semaforo
struct TrafficLight{
    int id;
    int state;
};

//interseccion

void init_cars(char name[], struct Car cars[]){
    FILE *v_file = fopen(name,"r"); 
    char line[MAX_LINE];
    int idx = 0;
    // Save cars
    while(fgets(line, sizeof(line), v_file)){
        cars[idx].id = idx;
        cars[idx].position = atoi(line);
        idx++;
    }
    // Show carss
    // for(int i = 0; i<NUM_CARS; i++){
    //     printf("Carro %d\tPosicion %d\n",cars[i].id, cars[i].position);
    // }
}

void init_traffic_lights(char name[], struct TrafficLight semaforos[]){
    FILE *s_file = fopen(name,"r"); 
    char line[MAX_LINE];
    int idx = 0;
    while(fgets(line, sizeof(line), s_file)){
        semaforos[idx].id = idx;
        semaforos[idx].state = atoi(line);
        idx++;
    }
}

struct Intersection{
    int state;
};
int main(){

    // Carros
    struct Car cars[NUM_CARS];
    char input[] = "vehiculos.txt";
    init_cars(input, cars);

    for(int i = 0; i<NUM_CARS; i++){
        printf("Carro %d\tPosicion %d\n",cars[i].id, cars[i].position);
    }

    // Semaforos
    struct TrafficLight semaforos[NUM_TRAFFIC_LIGHT];
    char s_input[] = "semaforos.txt";
    init_traffic_lights(s_input, semaforos);
    
    for(int i = 0; i<NUM_TRAFFIC_LIGHT; i++){
        printf("Semaforo %d\tEstado %d\n",semaforos[i].id, semaforos[i].state);
    }

    return 0;
}