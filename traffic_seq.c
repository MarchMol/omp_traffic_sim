#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
// Number of data entries
#define NUM_CARS 2000
#define NUM_INTERSECTION 400
#define CAR_PER_INT 50
#define NUM_SIMULATIONS 10000
// #define NUM_INTERSECTION 5

// Buffer size
#define BUFFER_SIZE 1028

// Traffic Light states
#define RED_LIGHT 0
#define YELLOW_LIGHT 1
#define GREEN_LIGHT 2

// Estructura para una Carretera
typedef struct{
    // La carretera contiene carros y un semaforo
    int num_cars;
    int car_idx_start;
    int car_pos[CAR_PER_INT];
    int traffic_light;
} Road;

// Estructura para interseccion
typedef struct{
    // La interseccion posee multiples carreteras con sus propios semaforos y carros que manejar
    int num_roads;
    Road roads[NUM_INTERSECTION];
} Intersection;


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

void save_road_state(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
    for(int r = 0; r<nr; r++){
        // Save traffic light str
        char buffer[1028] = "";
        int tf_state = traffic_sim->roads[r].traffic_light;
        if (tf_state == RED_LIGHT){
            snprintf(buffer, sizeof(buffer), "Sem at: %d\n", tf_state);
        }
        if (tf_state == YELLOW_LIGHT){
            snprintf(buffer, sizeof(buffer), "Sem at: %d\n", tf_state);
        }
        if (tf_state == GREEN_LIGHT){
            snprintf(buffer, sizeof(buffer), "Sem at: %d\n", tf_state);
        }

        // Save Road str
        char road_str[1028] ="Cars";
        int num_cars = traffic_sim->roads[r].num_cars;
        int car_idx_start = traffic_sim->roads[r].car_idx_start;
        int car_pos = 0;
        for(int car = 0; car<num_cars; car++){
            car_pos = traffic_sim->roads[r].car_pos[car];
            size_t road_len = strlen(road_str);
            snprintf(road_str + road_len, sizeof(road_str) - road_len, "id: %d, pos %d \n", (car+car_idx_start), car_pos);
        }
        // Concat
        strcat(buffer, road_str);
        printf("%s\n",buffer);
    }
}

void update_cars(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
    // #pragma omp parallel for collapse(2) schedule(dynamic)
    for(int r = 0; r<nr; r++){
        for(int i = 0; i<CAR_PER_INT; i++){
            if(traffic_sim->roads[r].traffic_light!=RED_LIGHT){
                traffic_sim->roads[r].car_pos[i]++;
            }
            
        }
    }

}

void update_traffic_lights(Intersection *traffic_sim){
    int nr = traffic_sim->num_roads;
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
    for(int n = 0; n<=n_sims; n++){
        update_cars(&traffic_sim);
        update_traffic_lights(&traffic_sim);
    }
}

int main(){
    Intersection traffic_sim;
    traffic_sim.num_roads = NUM_INTERSECTION;
    freopen("output_seq.txt", "w", stdout); 
    // Roads
    Road r_tem;
    for(int r = 0; r<NUM_INTERSECTION;r++){
        r_tem.num_cars = CAR_PER_INT;
        r_tem.car_idx_start = r*CAR_PER_INT;
        r_tem.traffic_light = r%3;
        for(int i = 0; i<r_tem.num_cars;i++){
            r_tem.car_pos[i] = -i;
        }
        traffic_sim.roads[r] = r_tem;
    }
    // Dynamic simulation
    double total_start = omp_get_wtime();
    dynamic_simulation(
        NUM_SIMULATIONS,
        traffic_sim,
        0
    );
    double total_end = omp_get_wtime();
    printf("Total time: %f", total_end - total_start)
    return 0;
}