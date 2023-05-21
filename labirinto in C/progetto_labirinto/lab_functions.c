#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h> // per srand()
#include "lab_functions.h"

 // il tipo del piano di gioco
  typedef struct playground{
    int w;
    int h;
    char* map;
  }playground_t;

playground_t* lettura_labirinto(playground_t* lab){
  scanf("%d %d", &lab->w, &lab->h);
  lab->map = (char*) malloc (lab->h * lab->w);
  scanf("%[^\n]s", lab->map);
  return lab;
};

void stampa_labirinto(playground_t* lab){
  for (int i = 0; i < lab->w; i++){
    for (int j = 0; j < lab->h; j++)
      printf("-%c-", lab->map[i * lab->w + j]);
    printf("\n");
  }
}
/*
void trova_giocatore(struct playground_t lab){
    for (int i = 0; i < n_righe; i++)
        for (int y = 0; y < n_colonne; y++)
            if (xy[i][y] == '.'){
                riga = i;
                colonna = y;
            }
}
/*
void move_up(char* labirinto int i, int j)
{
    labirinto[i-1][j] = labirinto[i][j];
    labirinto[i][j] = "\0";
}

void move_down(char* labirinto int i, int j)
{
    labirinto[i+1][j] = labirinto[i][j];
    labirinto[i][j] = "\0";
}

void move_left(char* labirinto int i, int j)
{
    labirinto[i][j-1] = labirinto[i][j];
    labirinto[i][j] = "\0";
}

void move_right(char* labirinto int i, int j)
{
    labirinto[i][j+1] = labirinto[i][j];
    labirinto[i][j] = "\0";
}

void mossa(int riga_player, int colonna_player) //questo dentro a un while(True)
{
    char direzione;
    scanf("%c" &direzione);
    int* i;
    int* j;
    check_player(riga_player, colonna_player, i, j);
    if(direzione == "w")
        if (check_space())
            move_up(labirinto, i, j);
    else if(direzione == "s")
        if (check_space())
            move_down(labirinto, i, j);
    else if(direzione == "a")
        if (check_space())
            move_left(labirinto, i, j);
    else if(direzione == "d")
        if (check_space())
            move_right(labirinto, i, j);
}
*/

