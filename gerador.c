#include "parque.h"


int main(int argc, char *argv[]) {

  //Wrong number of args
  if(argc != 3){
    printf("usage: %s <t_generation> <u_clock>\n",argv[0]);
    exit(1);
  }

  // Period of time in seconds to the working generator
  int t_generation = atoi(argv[1]);
  // General unit of time in ticks
  int u_clock = atoi(argv[2]);

  //Inicial Ticks
  clock_t t_inicial = clock();
  //Inicial Diference
  clock_t  t = t_inicial - clock();

  //Cicle with t_gera
  while(t <= t_geracao*CLOCKS_PER_SEC){

  }


  exit(0);
}
