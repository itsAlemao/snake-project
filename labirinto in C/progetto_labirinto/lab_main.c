//<> || qua ci sta il MIO labirinto 
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <memory.h> //per le alloc
#include <time.h>
#include <stdlib.h> // per srand
#include <string.h> //per strncmp
#include "lab_functions.c"


int main() {


  playground_t* labirinto;

  labirinto = lettura_labirinto(labirinto);

  stampa_labirinto(labirinto);

  return 0;
}