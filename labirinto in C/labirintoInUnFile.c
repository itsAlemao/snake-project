#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

typedef struct playground{
    int w;
    int h;
    char* map;
}playground_t;

typedef struct l_snake{
    char character;
    int position;
    int next_position;
    struct l_snake* next;
}snake_t;

playground_t* l_create(playground_t* lab); //crea il labirinto, deve essere scritto dall'utente
playground_t* l_create_random(playground_t* lab); //crea in automatico il labirinto, l'utente deve inserire solo altezza e larghezza
    int random_edge_position(playground_t* lab); //estrae una posizione random di un lato, serve per impostare dove inizia il player e dove è l'uscita
    bool space_check(playground_t* lab, int position, char direction); //controlla se in una casella è possibile costruire un muro senza bloccare l'uscita del labirinto
    void wall_spawner(playground_t* lab, int position, char direction); //spawna una fila di muri in numero random
    int next_wall(playground_t* lab, char* outDirection); //trova una posizione e una direzione in cui è possibile costruire un muro

//funzioni che gestiscono la creazione del serpente
snake_t* head_create(snake_t* head, playground_t* lab);
snake_t* body_create(snake_t* snake);
void snake_update(playground_t* lab, snake_t* snake, char direction); //controlla ad ogni mossa che il serpente non si sia mangiato la coda o se abbia mangiato un bonus/malus

void l_print(playground_t* lab); //printa il labirinto

void start_game_rec(playground_t* lab, snake_t* snake, int score, int drillCharges, bool tail); //usa diverse funzioni (qua sotto) per muovere il player in base all'input, è ricorsiva.
    int player_location(playground_t* lab); //trova la posizione del player
    char take_input(); //prende in input w a s d
    int next_location(playground_t* lab, char direction); //in base alla direzione (w a s d) individua la casella su cui si deve spostare il player
    bool verify_move(playground_t* lab, char direction); //verifica se la mossa è possibile, ovvero se non stai andando contro un muro # o fuori dal labirinto
    int score_update(playground_t* lab, char direction, int score); //aggiorna lo score
    int powerup_update (playground_t* lab, char direction, int drillCharges); //aggiorna lo stato dei powerup
    bool check_victory(playground_t* lab, char direction); //controlla se sei arrivato all'uscita

void l_free(playground_t* lab); //ad ogni malloc corrisponde una free!
void s_free(snake_t* snake);





//...............................................................................................................................................
int main(){

    bool lab_randomizer = true; //scegli se generare un labirinto automaticamente, altrimenti va inserito manualmente
    bool tail = true; //scegli se giocare con la coda oppure no

    srand(time(NULL));
    playground_t* labyrinth; 
    snake_t* snake; 

    if (lab_randomizer) labyrinth = l_create_random(labyrinth);
    else labyrinth = l_create(labyrinth);

    l_print(labyrinth);
    snake = head_create(snake, labyrinth);
    int score = 1000;
    int drillCharges = 0;

    start_game_rec(labyrinth, snake, score, drillCharges, tail);
    l_free(labyrinth);
    s_free(snake);

    printf("\ndaje!");
    return 0;
}
//...............................................................................................................................................

playground_t* l_create(playground_t* lab)
{
    lab = (playground_t*)malloc(sizeof(playground_t));
    printf("inserire numero righe, colonne e la mappa \n");
    scanf("%d %d\n", &lab->h, &lab->w);
    lab->map = (char*)malloc(lab->h * lab->w);
    scanf("%[^\n]s", lab->map);
    return lab;
}

snake_t* head_create(snake_t* head, playground_t* lab)
{
    head = (snake_t*)malloc(sizeof(snake_t));
    head->character = 'o';
    head->position = player_location(lab);
    return head;
}

snake_t* body_create(snake_t* snake)
{
    snake_t* cursor = snake;
    while(cursor->next != NULL)
    {
        cursor = cursor->next;
    }
    cursor->next = (snake_t*)malloc(sizeof(snake_t));
    cursor->next->character = 'c';

    return snake;
}

void l_print(playground_t* lab)
{
    //aggiungere clear screen
    for(int i = 0; i < lab->h; i++)
    {
      printf("\n");  
      for(int j = 0; j < lab->w; j++)
        {
            printf("%c", lab->map[j + i * lab->w]);
        }
    }
    printf("\n");
}

int player_location(playground_t* lab)
{
    for(int i = 0; i < lab->h; i++)
    { 
      for(int j = 0; j < lab->w; j++)
        {
            if (lab->map[j + i * lab->w] == 'o')
            {
                return j + i * lab->w;
            }
        }
    }
}

char take_input()
{
    char direction = 'z';
    while (direction != 'w' && direction != 'a' && direction != 's' && direction != 'd' && direction != 'e')
    {
    scanf("%c", &direction);
    }
    return direction;
}

int next_location(playground_t* lab, char direction)
{
    int x = player_location(lab); 
    if (direction == 'w') return x - lab->w;
    if (direction == 'a') return x - 1;
    if (direction == 's') return x + lab->w;
    if (direction == 'd') return x + 1;
}

bool verify_move(playground_t* lab, char direction)
{
    if (lab->map[next_location(lab, direction)] == '#') //se troviamo un muro ritorniamo false perche la mossa è impossibile
    {
        return false;
    }
    return true;
}

//aggiorna lo score se il player incontra delle monete, degli imprevisti o fa una mossa
int score_update(playground_t* lab, char direction, int score)
{
    if (lab->map[next_location(lab, direction)] == '$') return score + 10;
    if (lab->map[next_location(lab, direction)] == '!') return score/2; 
    else return score - 1;
}

//se il player incontra un trapano i suoi usi possibili aumentano di 3
int powerup_update (playground_t* lab, char direction, int drillCharges)
{
  if (lab->map[next_location(lab, direction)] == 'T') return drillCharges + 3;
  else return drillCharges;
}

bool check_victory(playground_t* lab, char direction)
{
    if (lab->map[next_location(lab, direction)] == '_') return true;
    else return false;
}

//richiama se stessa finche non si vince il gioco
void start_game_rec(playground_t* lab,snake_t* snake, int score, int drillCharges, bool hasTail)
{
    l_print(lab);
    printf("score: %d   Trapano usi disponbibili: %d\n", score, drillCharges);
    char direction = take_input();
    if (direction == 'e') return;

    if (check_victory(lab, direction))
    {
        return;
    }
    else
    {
        if (!verify_move(lab, direction) && drillCharges == 0 || //caso in cui prendi il muro senza trapano
            next_location(lab, direction) < 0 || next_location(lab, direction) > lab->w * lab->h || //caso in cui vuoi andare sopra la prima riga o sotto all'ultima
            player_location(lab) % lab->w == lab->w - 1 && next_location(lab, direction) % lab->w == 0 || //caso in cui sei nella fila piu a dx e vuoi andare a dx
            player_location(lab) % lab->w == 0 && next_location(lab, direction) % lab->w == lab->w - 1) //caso in cui sei nella fila piu a sx e vuoi andare a sx
        {
            printf("hai preso il muro fratellì, ritenta\n");
            start_game_rec(lab, snake, score, drillCharges, hasTail);
        }
        else
        {
            if (verify_move(lab, direction) == false)
            {
                drillCharges--;
            }
            //controllare se prende un $ o la sua coda per gestire la lunghezza, aggiornare la posizione di tutta la struct, aggiornare il labirinto di conseguenza


            score = score_update(lab, direction, score);
            drillCharges = powerup_update(lab, direction, drillCharges);

            if (hasTail)
            {
                snake_update(lab, snake, direction);                
            }
            else
            {
                int scambio = player_location(lab);
                lab->map[next_location(lab, direction)] = 'o';
                lab->map[scambio] = ' ';                
            }
          

            start_game_rec(lab, snake, score, drillCharges, hasTail);
        }
    }
}

void snake_update(playground_t* lab, snake_t* snake, char direction)
{
    snake_t* cursor = snake;
    snake->next_position = next_location(lab, direction);

  cursor = snake; //pulisce le caselle che erano occupate prima della mossa
    while(cursor != NULL)
    {
        lab->map[cursor->position] = ' ';
        cursor = cursor->next;
    }
  
    if (lab->map[snake->next_position] == '$')//allunga il serpente se prendi $
    {
        snake = body_create(snake);
    }
  

  
    cursor = snake;
    while(!(cursor == NULL || cursor->next == NULL)) //aggiorna la struct dando direction ogni elemento la casella che dovrà raggiungere basandosi sull'elemento ad essa precedente
    {
        cursor->next->next_position = cursor->position;
        cursor = cursor->next;
    }

    cursor = snake;
    while(cursor != NULL) //aggiorna la posizione degli elementi della struct
    {
        cursor->position = cursor->next_position;
        cursor = cursor->next;
    }

    if (snake->next != NULL) //controlla se ti sei mangiato la coda e in tal caso, viene liberata la memoria da quella struct in poi
    {
    cursor = snake;
    while (cursor->next != NULL)
        {
            if  (snake->next_position == cursor->next->next_position)
            {
                cursor->next = NULL;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
  
    cursor = snake; //aggiorna lo snake sulla mappa
    while (cursor != NULL)
    {
        lab->map[cursor->next_position] = cursor->character;
        cursor = cursor->next;
    }

}

void l_free(playground_t* lab)
{
    free(lab->map);
    free(lab);
}

void s_free(snake_t* snake)
{
    free(snake);
}

//WORK IN PROGRESS

playground_t* l_create_random(playground_t* lab)
{
    lab = (playground_t*)malloc(sizeof(playground_t));
    printf("inserire numero righe, colonne, la mappa verrà generata automaticamente\n");
    scanf("%d %d\n", &lab->h, &lab->w);
    printf("\n");
    lab->map = (char*)malloc(lab->h * lab->w);
 
    //mette tutte le posizioni a carattere '#'
    for(int i = 0; i < lab->h; i++)
    {
        for(int j = 0; j < lab->w; j++)
        {
            lab->map[i * lab->w + j] = '#';
        }
    }
    //mette gli spazi che non sono i bordi a carattere ' '
    for(int i = 1; i < lab->h -1; i++)
    {
        for(int j = 1; j < lab->w -1; j++)
        {
            lab->map[i * lab->w + j] = ' ';
        }
    }


    int x = 0, y = 0;
    while (x == y)
    {
        x = random_edge_position(lab); //casella da cui partirà il player
        y = random_edge_position(lab); //casella dell'uscita
    }
    lab->map[x] = 'o';
    lab->map[y] = '_';

    int wall_position = 0;
    char wall_direction;
    while (wall_position != -1)
    {
        wall_position = next_wall(lab, &wall_direction); //estrae una positione valida per mettere un muro
        if (wall_position != -1) wall_spawner(lab, wall_position, wall_direction); //stampa una riga di muri dalla posizione data e in base alla direzione
    }

    // <  > -> ||
    //ha una possibilità di stampare nei caratteri vuoti un oggetto20
    for (int i = 0; i < lab->w * lab->h; i++)
    {
        if(lab->map[i] == ' ')
        {
            int x = rand() % 100 +1; //estrae un numero da 1 a 100

            if (x < 10) lab->map[i] = '$'; //10% possibilità di spawnare una moneta
            else if (x < 14) lab->map[i] = '!'; //4% possibilità di spawnare un imprevisto
            else if (x < 15) lab->map[i] = 'T'; //1% possibilità di spawnare un trapano
        }
    }

    return lab;
}
  

int random_edge_position(playground_t* lab)
{
    int x = 0;
    while(lab->map[x] != '#' || x == 0 || x == lab->h || x == lab->w * lab->h - lab->h || x == lab->w * lab->h -1) //se non è stato estratto un muro oppure è stato estratto un angolo
    {
        x = rand() % (lab->w * lab->h);
    }
    return x;
}


bool space_check(playground_t *lab, int position, char direction)
{
    if (lab->map[position] != ' ') return false;
    //condizione muro dietro, 5 spazi liberi nella direzione detta
    if (direction == 'w') 
    {
        if(lab->map[position - lab->w] == '#')
        {
            if (lab->map[position -1] == ' ' && lab->map[position +1] == ' ' && lab->map[position - lab->w -1] == ' ' && lab->map[position - lab->w] == ' ' && lab->map[position - lab->w +1] == ' ')
            return true;
        }
        return false;
    }
    if (direction == 'a')
    {
        if(lab->map[position +1] == '#')
        {
            if (lab->map[position - lab->w -1] == ' ' && lab->map[position - lab->w] == ' ' && lab->map[position -1] == ' ' && lab->map[position + lab->w -1] == ' ' && lab->map[position + lab->w] == ' ')
            return true;
        }
        return false;        
    }
    if (direction == 's')
    {
        if(lab->map[position - lab->w] == '#')
        {
            if (lab->map[position -1] == ' ' && lab->map[position +1] == ' ' && lab->map[position + lab->w -1] == ' ' && lab->map[position + lab->w] == ' ' && lab->map[position + lab->w +1] == ' ')
            return true;
        }
        return false;
    }
    if (direction == 'd')
    {
        if(lab->map[position -1] == '#')
        {
            if (lab->map[position - lab->w] == ' ' && lab->map[position - lab->w +1] == ' ' && lab->map[position +1] == ' ' && lab->map[position + lab->w] == ' ' && lab->map[position + lab->w +1] == ' ')
            return true;
        }
        return false;
    }
    return false;
}


//trova nel labirinto un punto in cui si possa costruire un muro, ritorna la direzione tramite il secondo return
int next_wall(playground_t* lab, char* outDirection)
{
    char directions[4] = {'w', 'a', 's', 'd'};
    int x = rand(); //casella random del labirinto
    int y = rand(); //direzione random

    for(int c = 0; c < 4; c++)
    {
        *outDirection = directions[y % 4];
        for(int i = 0; i < lab->w * lab->h; i++)
        {
            if(space_check(lab, x % (lab->w * lab->h), *outDirection)) 
            {
                return x % (lab->w * lab->h);
            }
            x++;
        }
        y++;
    }
    return -1;
}

// <  > -> ||

void wall_spawner(playground_t* lab, int position, char direction)
{
    int next_space;
    int starting_position = position;
    if (direction == 'w') next_space = -lab->w;
    else if (direction == 'a') next_space = -1;
    else if (direction == 's') next_space = lab->w;
    else if (direction == 'd') next_space = +1;
    else (EXIT_FAILURE);

    int count = 0; //controlla in quanti spazi è possibile costruire un muro


    while (space_check(lab, position, direction))
    {
        count++;
        position = position + next_space; 
    }


    int x = rand() % count +1; //estrae un numero random di muri da costruire
    for (int i = 0; i < x; i++)
    {
        lab->map[starting_position] = '#';
        starting_position = starting_position + next_space;
    }
}

/*

//NUOVO PROGETTO PER LO SPAWNA MURI INTERNI

-controllo possibilità di mettere muri interni: ritorna posizione e direzione
-costruisce una linea di muri di una lunghezza random ma non cosi grande che connetta con un altro muro

funzioni:
nuovo_muro: parte da un punto random e con una direzione random, controlla tutta la mappa fino alla fine e ricomincia fino a che non torna al punto random e controlla con un altra direzione per tutte e 4 le direzioni
ritorna la prima posizione e direzione disponibili che rispettano certi criteri. i criteri:
 -deve esserci un muro dietro rispetto a dove stai guardando
 -devono esserci 6 spazi vuoti tipo:
#--
#o- //o è il punto che si prende in esame, se si guarda a dx si ha 5 caselle vuote FATTA: SPACE_CHECK
#--

numero di muri: si fa un controllo data una posizione e la direzione del primo muro che si incontra e si conta le caselle vuote nel mezzo (metti X caselle), si estrae un numero random tra 1 e X-1 e si costruiscono quei muri
*/


// <  >  -> ||


/* APPUNTI
l_create random //crea randomicamente il labirinto

random_edge_position //trova uno spazio in cornice non angolo, serve per scegliere la posizione dell'uscita e del player

wall_spawner //funzione che spawna tutti i muri interni (work in progress)

spaceAround //funzione che controlla intorno a un determinato punto quanti spazi vuoti ci siano

turn_left_or_right //cambia direzione randomicamente, non puo tornare indietro, serve per dare una nuova direzione al muro in costruzione

more_possible_walls //controlla se c'è ancora spazio per mettere muri senza ostruire il passaggio

funzione che dice in base a dove stai guardando se hai 6 caselle libere tipo:
#--
#o- //guardi a dx, hai 5 caselle libere
#--ù

//voglio creare una funzione che ti scannerizza tutto il labirinto in ordine, partendo da una casella random con una direzione random,
 in modo che quando finisce il labirinto ti riparte dall'inizio fino alla casella prima della casella random e voglio fare 
 in modo che se è costruibile un muro in una certa direzione ritorni la posizione di quel muro e la direzione. in caso finisca il controllo senza trovare posizioni possibili per i muri allora ritorna -1


*/

//funzioni in disuso
/*
char turn_left_or_right(playground_t* lab, char previous_move)
{
    int x = rand() % 2;
    if (previous_move == 'w' || previous_move == 's')
        if (x == 0) return 'a';
        else return 'd';
    if(previous_move == 'a' || previous_move == 'd')
        if(x == 0) return 'w';
        else return 's';
}

//controlla intorno a un determinato punto quanti spazi vuoti ci siano
int space_around(playground_t* lab, int position, int range)
{
    //controlla se il controllo va fuori dal labirinto verticalmente
    int original_position = position;
    position = original_position - lab->w - 1;
    for(int i = 0; i < range * 2 + 1; i++)
    {
        if (position < 0 ||
            position >= lab->w*lab->h)
        {
            return -1;
        }
        position += lab->w;
    }

    //controlla se il controllo va fuori dal labirinto orizzontalmente
    position = original_position - range;
    int current_column = 0;
    for(int i = 0; i < range * 2 + 1; i++)
    {
        if (position % lab->w < current_column)
        {
            return -1;
        }
        current_column = position % lab->w;
        position++;
    }

    int count = 0;
    position = original_position -(1 + lab->w) * range;
    for(int i = 0; i < range * 2 + 1; i++)
    {
        for(int j = 0; j < range * 2 + 1; j++)
        {
            if (lab->map[position + i * lab->w + j] == ' ') count++;
        }
    }
    return count;
}




*/