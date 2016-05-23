#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/file.h>
#include <string.h>
#include <time.h>

#define FIFO_MODE 0660
#define ID_SIZE 7
#define T_PARKING_SIZE 7
#define FIFO_NAME_SIZE 5
#define CONTROLLER_ID_SIZE 2
#define VEHICLE_STATE_SIZE 9
#define TRUE 1
#define FALSE 0

typedef struct {
	int id;
	char dest[CONTROLLER_ID_SIZE];
	int t_parking;
	int t_life;
	char state[VEHICLE_STATE_SIZE];
} Vehicle;

void *controller(void *arg);
void *car_assistant(void *arg);

void writeVehicle(int fd, Vehicle *v);
void readVehicle(int fd, Vehicle *v);
