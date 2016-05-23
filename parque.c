#include "parque.h"

int n_spots;
int n_spots_available;
clock_t start;

int main(int argc, char *argv[]) {
	n_spots = atoi(argv[1]);
	n_spots_available = n_spots;
	int duration = atoi(argv[2]);

	//==============================================
	// Open park

	// Create controller threads
	pthread_t tidN, tidS, tidE, tidO;
	char n[] = "N", s[] = "S", e[] = "E", o[] = "O";
	pthread_create(&tidN, NULL, controller, n);
	pthread_create(&tidS, NULL, controller, s);
	pthread_create(&tidE, NULL, controller, e);
	pthread_create(&tidO, NULL, controller, o);

	// Sleep while park is open
	sleep(duration);
	printf("\n");

	//=============================================
	// Close park

	// Open controller FIFO
	int fdwN, fdwS, fdwE, fdwO;
	fdwN = open("fifoN", O_WRONLY);
	fdwS = open("fifoS", O_WRONLY);
	fdwE = open("fifoE", O_WRONLY);
	fdwO = open("fifoO", O_WRONLY);

	// Write stop vehicle
	Vehicle *stop_vehicle = malloc(sizeof(Vehicle));
	stop_vehicle->id = -1;
	writeVehicle(fdwN, stop_vehicle);
	writeVehicle(fdwS, stop_vehicle);
	writeVehicle(fdwE, stop_vehicle);
	writeVehicle(fdwO, stop_vehicle);

	// Close FIFO

	// Wait for controller thread return
	pthread_join(tidN, NULL);
	pthread_join(tidS, NULL);
	pthread_join(tidE, NULL);
	pthread_join(tidO, NULL);

	// Destroy FIFO
		
	return 0;
}

void *controller(void *arg) {
	// Create controller ID
	char id[CONTROLLER_ID_SIZE];
	strcpy(id, (char *)arg);

	// Create FIFO name
	char fifo_name[FIFO_NAME_SIZE];
	strcpy(fifo_name, "fifo");
	strcat(fifo_name, id);
	int parque_open = TRUE;

	printf("Open %s controller\n", id);

	// Make FIFO
	mkfifo(fifo_name, FIFO_MODE);
	int fdr = open(fifo_name, O_RDONLY);

	// Read vehicle while open
	start = clock();
	while(parque_open) {
		Vehicle *v = malloc(sizeof(Vehicle));
		readVehicle(fdr, v);

		// If stop vehicle is read stop reading
		if (v->id == -1) {
			parque_open = FALSE;
		}
		// Else create car assistant thread
		else {
			pthread_t u;
			pthread_create(&u, NULL, car_assistant, v);
			pthread_detach(u);
		}

	}

	// Close controller

	// while fifo not empty
		// answer calls and create assistants

	printf("Close %s controller\n", id);
	close(fifo_name);

	return NULL;
}


void *car_assistant(void *arg) {
	Vehicle v = *(Vehicle *) arg;
	// free arg

	// Open vehicle fifo
	char vehicle_fifo[ID_SIZE];
	sprintf(vehicle_fifo, "%i", v.id);
	int fdw = open(vehicle_fifo, O_WRONLY);

	// if parque = encerrado escrever em log	

	if (n_spots_available <= 0) {
		write(fdw, "cheio", VEHICLE_STATE_SIZE);
		// escrever em log
	}
	else {
		n_spots_available--;
		write(fdw, "entrada", VEHICLE_STATE_SIZE);
		// escrever em log
		
		//sleep for v.t_parking
		//sair
		//escrever em log
	}

	
	
	
	





}


//===============================================================
// Auxiliar functions


// Write vehicle info to fifo
// param: fd -> file descriptor, v -> pointer to vehicle to write
void writeVehicle(int fd, Vehicle *v) {

	// Write ID
	char id[ID_SIZE];
	sprintf(id, "%i", v->id);
	write(fd, id, ID_SIZE);

	// Write destination (controller id N-S-E-O)	 
	write(fd, v->dest, CONTROLLER_ID_SIZE);

	// Write parking time
	char t[T_PARKING_SIZE];
	sprintf(t, "%i", v->t_parking);
	write(fd, t, T_PARKING_SIZE);
}

// Read vehicle info from fifo
// param: fd-> file descriptor, v -> pointer to vehicle to read
void readVehicle(int fd, Vehicle *v) {

	// Read ID
	char id[ID_SIZE];
	read(fd, id, ID_SIZE);
	v->id = atoi(id);
	
	// Read destination
	read(fd, v->dest, CONTROLLER_ID_SIZE);

	// Read parking time
	char t[T_PARKING_SIZE];
	read(fd, t, T_PARKING_SIZE);
	v->t_parking = atoi(t);
}






