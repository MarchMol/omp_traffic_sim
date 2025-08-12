#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
// Number of data entries
#define NUM_CARS 20
#define NUM_INTERSECTION 4
// #define NUM_INTERSECTION 5

// Buffer size
#define BUFFER_SIZE 128

// Traffic Light states
#define RED_LIGHT 0
#define YELLOW_LIGHT 1
#define GREEN_LIGHT 2

// Estructura para una Carretera 🚗


typedef struct{
    // La carretera contiene carros y un semaforo
    int num_cars;
    int car_idx_start;
    int car_pos[NUM_CARS];
    int traffic_light;
} Road;

// Estructura para interseccion
typedef struct{
    // La interseccion posee multiples carreteras con sus propios semaforos y carros que manejar
    int num_roads;
    Road roads[NUM_INTERSECTION];
} Intersection;

// int init_road(
//     char c_path[],
//     int car_pos[],
//     char s_path[],
//     struct Semaforo semaforos[]
// ){
//     // Load car position file
//     FILE *c_file = fopen(c_path,"r");
//     char line[BUFFER_SIZE];
//     int car_idx = 0;
//     while(fgets(line, sizeof(line),c_file)){
//         car_pos[car_idx] = atoi(line);
//         car_idx++;
//     }

//     int cars_per_sem = car_idx/NUM_TRAFFIC_LIGHT;

//     // Load traffic light initial condition
//     FILE *s_file = fopen(s_path, "r");
//     int sem_idx = 0;
//     while(fgets(line, sizeof(line),s_file)){
//         struct Semaforo s_tem;
//         s_tem.state = atoi(line);
//         s_tem.num_cars = cars_per_sem;
//         for(int i = 0; i <cars_per_sem; i++){
//             s_tem.cars[i] = (cars_per_sem*sem_idx)+i;
//         }
//         semaforos[sem_idx] = s_tem;
//         sem_idx++;
//     }

//     return 0;
// }


// void update_road(struct Semaforo semaforos[], int car_pos[]){
//     for(int i = 0; i< NUM_TRAFFIC_LIGHT; i++){

//         int cars = semaforos[i].num_cars;
//         int prev_light = semaforos[i].state;
//         int car_idx = 0;
//         if(prev_light!=RED_LIGHT){
//             for(int j = 0; j<cars; j++){
//                 car_idx = semaforos[i].cars[j];
//                 car_pos[car_idx]++;
//             }
//         }
//         if(prev_light==RED_LIGHT){
//             semaforos[i].state = GREEN_LIGHT;
//         }
//         if(prev_light==GREEN_LIGHT){
//             semaforos[i].state = YELLOW_LIGHT;
//         }
//         if(prev_light==YELLOW_LIGHT){
//             semaforos[i].state = RED_LIGHT;
//         }
//     }
// }

// void update_cars(){
// }

void print_road_state(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
    for(int r = 0; r<nr; r++){
        // Save traffic light str
        char buffer[1028] = "";
        int tf_state = traffic_sim->roads[r].traffic_light;
        if (tf_state == RED_LIGHT){
            snprintf(buffer, sizeof(buffer), "\033[0;31m [%d] \033[0m", tf_state);
        }
        if (tf_state == YELLOW_LIGHT){
            snprintf(buffer, sizeof(buffer), "\033[0;33m [%d] \033[0m", tf_state);
        }
        if (tf_state == GREEN_LIGHT){
            snprintf(buffer, sizeof(buffer), "\033[0;32m [%d] \033[0m", tf_state);
        }

        // Save Road str
        char road_str[128] = "";
        int num_cars = traffic_sim->roads[r].num_cars;
        int car_idx_start = traffic_sim->roads[r].car_idx_start;
        int car_pos = 0;
        for(int car = 0; car<num_cars; car++){
            car_pos = traffic_sim->roads[r].car_pos[car];
            size_t road_len = strlen(road_str);
            snprintf(road_str + road_len, sizeof(road_str) - road_len, "🚗 [id: %d | pos %d ]", (car+car_idx_start), car_pos);
        }

        // Concat
        strcat(buffer, road_str);
        printf("%s\n",buffer);
    }
}

void update_cars(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
    int n_cars = 5;
    #pragma omp parallel for collapse(2) num_threads(nr * n_cars)
    for(int r = 0; r<nr; r++){
        for(int i = 0; i<n_cars; i++){
            if(traffic_sim->roads[r].traffic_light!=RED_LIGHT){
                traffic_sim->roads[r].car_pos[i]++;
            }
            
        }
    }

}

void update_traffic_lights(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
    #pragma omp parallel for num_threads(nr)
    for(int i = 0; i<nr; i++){
        int pre_state = traffic_sim->roads[i].traffic_light;
        if (pre_state == RED_LIGHT) {
            traffic_sim->roads[i].traffic_light = GREEN_LIGHT;
        }
        if (pre_state == YELLOW_LIGHT){
            traffic_sim->roads[i].traffic_light = RED_LIGHT;
        } 
        if (pre_state == GREEN_LIGHT){
            traffic_sim->roads[i].traffic_light = YELLOW_LIGHT;
        } 
    }
}

void dynamic_simulation(int n_sims, Intersection traffic_sim, int do_print){
    omp_set_dynamic(1);
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    for(int n = 0; n<=n_sims; n++){
        if(do_print>0){
            printf("Tick %d\n",n);
            print_road_state(&traffic_sim);
            printf("\n");
        }
        update_cars(&traffic_sim);
        update_traffic_lights(&traffic_sim);
    }
}

int main(){
    Intersection traffic_sim;
    traffic_sim.num_roads = NUM_INTERSECTION;

    // Roads
    Road r_tem;
    for(int r = 0; r<4;r++){
        r_tem.num_cars = 5;
        r_tem.car_idx_start = r*5;
        r_tem.traffic_light = r%3;
        for(int i = 0; i<r_tem.num_cars;i++){
            r_tem.car_pos[i] = -i;
        }
        traffic_sim.roads[r] = r_tem;
    }

    // Dynamic simulation
    dynamic_simulation(
        10,
        traffic_sim,
        1
    );

    

    return 0;
}