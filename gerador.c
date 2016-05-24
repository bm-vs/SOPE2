#include "parque.h"

//Global variables
int id =0;
int fd_g_log;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

//Funcion declarations
void* func_vehicle(void* arg);
clock_t generate_vehicle (clock_t u_clock,clock_t i_ticks);

//Main
int main(int argc, char *argv[]) {

  //Wrong number of args
  if(argc != 3){
    printf("usage: %s <t_generation> <u_clock>\n",argv[0]);
    exit(1);
  }

  //Creating log file
  FILE * gfile = fopen("gerador.log","w");
  fclose(gfile);
  // Open and write name of the colums
  fd_g_log = open("gerador.log",O_WRONLY,O_CREAT, 0600 );
  char name_colums[] = "t(ticks) ; id_viat ; destin ; t_estacion ; t_vida ; obser\n";
  write(fd_g_log,&name_colums,strlen(name_colums));

  // Period of time in seconds to the working generator
  int t_generation = atoi(argv[1]);
  // General unit of time in ticks
  clock_t u_clock = atoi(argv[2]);
  //Random seed generator;
  srand(time(NULL));
  //Inicial Ticks
  clock_t t_inicial = clock();
  //Inicial Diference
  clock_t  t = clock()-t_inicial ;

  //Cicle with t_geration condicion
  while(t <= t_generation*CLOCKS_PER_SEC){
    //Generate Vehicle
    clock_t wait_time = generate_vehicle(u_clock,clock());

    //Sleep wait_time seconds(WRONG CHANGE)
    sleep(wait_time/CLOCKS_PER_SEC);

    //New Diference
    t = clock()-t_inicial;
  }

  pthread_exit(NULL);
}

clock_t generate_vehicle (clock_t u_clock,clock_t i_ticks){
  Vehicle *v = (Vehicle *) malloc(sizeof(Vehicle));
  pthread_t tid;


  //ID
  v->id = id++;

  //Random controller entrance
  int controller = rand() % 4;

  switch (controller) {
    case 0:
      strcpy(v->dest,"N");
      break;
    case 1:
      strcpy(v->dest,"S");
      break;
    case 2:
      strcpy(v->dest,"E");
      break;
    case 3:
      strcpy(v->dest,"O");
      break;
    default:
      break;
  }
  //Inicial Tick
  v->ini_tick = i_ticks;

  //Time of parking
  v->t_parking= (rand() % 10 +1)* u_clock;

  //Next vehicle time
  clock_t next_v = rand () % 10+1;

  if(next_v <= 5){ //50% chance
    next_v = 0;
  }
  if(next_v >5 && next_v <=8){ //30% chance
    next_v = u_clock;
  }
  if(next_v > 8){ //20 % chance
    next_v = 2* u_clock;
  }

  //Vehicle FIFO name
  sprintf(v->fifo_name,"%s%d","fifo",v->id);

  //Call thread
  if(pthread_create(&tid,NULL,func_vehicle,v) != 0){
    perror("Gerador::Error creating thread\n");
  }

  //Return time in ticks that generator needs to wait
  return next_v;
}


//Function that the thread with tid executes when is created
void* func_vehicle(void* arg){
  Vehicle v= *(Vehicle*) arg;

  //Creat FIFO
  mkfifo(v.fifo_name,FIFO_MODE);

  //Open controller FIFO
  int fifoc;
  if(strcmp(v.dest, "N") == 0){
    fifoc = open("fifoN",O_WRONLY);
  }else if(strcmp(v.dest, "S") == 0){
    fifoc = open("fifoS",O_WRONLY);
  }else if(strcmp(v.dest, "E") == 0){
    fifoc = open("fifoE",O_WRONLY);
  }else{
    fifoc = open("fifoO",O_WRONLY);
  }

  writeVehicle(fifoc,&v);
  //while()
  pthread_exit(0);
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
	sprintf(t, "%i", (int) v->t_parking);
	write(fd, t, T_PARKING_SIZE);
}

// Read vehicle info from fifo
// param: fd-> file descriptor, v -> pointer to vehicle to read
int readVehicle(int fd, Vehicle *v) {

	int n_bytes_read = 0;

	// Read ID
	char id[ID_SIZE];
	n_bytes_read += read(fd, id, ID_SIZE);
	v->id = atoi(id);

	// Read destination
	n_bytes_read += read(fd, v->dest, CONTROLLER_ID_SIZE);

	// Read parking time
	char t[T_PARKING_SIZE];
	n_bytes_read += read(fd, t, T_PARKING_SIZE);
	v->t_parking = atoi(t);

	return n_bytes_read;
}
