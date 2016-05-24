#include "parque.h"

int id =0;
int fd_g_log;
pthred_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

//Function that the thread with tid executes when is created
void* func_vehicle(void* arg){
  Vehicle v= *(Vehicle*) arg;
  //int fd_read, fd_write;
  //int state=0;


  sem_t *sem = sem_open("/sem", O_CREAT ,FIFO_MODE,1);

  //Creat FIFO
  mkfifo(v->fifo_name,FIFO_MODE);

  pthread_exit(0);
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
      v->dest = "N";
      break;
    case 1:
      v->dest = "S";
      break;
    case 2:
      v->dest = "E";
      break;
    case 3:
      v->dest = "O";
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

  if(next_v <= 5){
    next_v = 0;
  }
  if(next_v >5 && next_v <=8){
    next_v = u_clock;
  }
  if(next_v > 8){
    next_v = 2* u_clock;
  }

  sprintf(v->fifo_name,"%s%d","fifo",v->id);

  //Call thread
  if(pthread_create(&tid,NULL,func_vehicle,v) != OK){
    perror("Gerador::Error creating thread\n");
  }

  return next_v;
}

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
  write (fd_g_log,name_colums,strlen(name_colums));

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
  while(t <= t_geration*CLOCKS_PER_SEC){
    //Generate Vehicle
    clock_t wait_time = generate_vehicle(u_clock,clock());

    //Sleep wait_time seconds
    sleep(wait_time/CLOCKS_PER_SEC);

    t = clock()-t_inicial;
  }

  pthread_exit(NULL);

  //exit(0);
}
