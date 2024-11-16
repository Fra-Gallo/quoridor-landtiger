
#include "LPC17xx.h"
#include "timer/timer.h" 
#include "CAN/CAN.h"
#include <limits.h>


#define R_BOARD 7
#define C_BOARD 7
#define MY_R 15
#define MY_C 15
#define SIDE_SQUARE_BOARD 25
#define N_WALLS 8
#define MAX_P 55
#define wait_handshake 0xEE6B20
#define RIT_INTERVAL 0xF4240



extern int is_move; //il giocatore sta effettuando una mossa di movimento
extern int have_wall; //il giocatore sta posizionando un muro
extern uint16_t new_wall_color; //il muro posizionato dal giocatore è valido o meno
extern int count; //conteggio secondi rimanenti 
extern struct Board board; //struttura dati che rappresenta la scacchiera
extern struct Player player1; // primo giocatore
extern struct Player player2; // secondo giocatore
extern struct Player playerAI; // giocatore NPC
extern int won; //variabile di controllo. Segna la vittoria di un giocatore.
extern uint8_t is_next_move_valid;//variabile di controllo. Segna se la mossa selezionata è valida.
extern int is_npc; // variabile di controllo. Segna se il giocatore è un NPC.
extern int my_mp_id; //variabile di controllo. Segna il proprio ID in modalità multiplayer (di default = 1).
extern uint8_t go_ahead; //variabile di controllo. Rapprensenta la scelta compiuta(1) o meno(0) del giocatore.
extern int is_mp ; //variabile di controllo. (1) se si sta giocando su due board (0) altrimenti
extern int go_ahead_adv; //variabile di controllo. Rapprensenta la scelta compiuta(1) o meno(0) dell'avversario.

/*STRUTTURE DATI PER LA RICERCA DEL CAMMINO MINIMO*/
typedef struct
{
  uint8_t i;
  uint8_t j;
} Point;


typedef struct
{
  Point array[MY_R * MY_C];
  int head, tail;
} Queue;

/*STRUTTURA DATI PER LA DIREZIONE DELLA PROSSIMA MOSSA O SPOSTAMENTO DI UN MURO*/
typedef enum
{
    su,
    giu,
	destra,
	sinistra,
	su_sinistra,
	su_destra,
	giu_sinistra,
	giu_destra
	
} Direction;

/*STRUTTURE DATI PER LA VISUALIZZAZIONE E CONTROLLO DEL GIOCO*/
struct Coordinates{
	uint16_t posX;
	uint16_t posY;
};

 struct Board{
	 uint8_t my_board[MY_R][MY_C];
	 uint8_t check_board[MY_R][MY_C];
	 struct Coordinates visual_board[R_BOARD][C_BOARD];
	 uint32_t moves[100];
	 int last_move;
	 int who_play;

 };
 
 struct Wall{
	 uint8_t horizontal;
	 uint8_t i;
	 uint8_t j;
 };
 
 struct Player {
	  
	  int id;
	  uint8_t my_i;
	  uint8_t my_j;
	  uint8_t visual_i;
	  uint8_t visual_j ;
    struct Coordinates position; 
	  struct Coordinates next_moves[8];
	  uint8_t is_nm_valid[8];
	  uint8_t tot_walls;
	  uint8_t last_wall;
	  struct Wall walls[N_WALLS];
	  struct Coordinates scoreboard;
	 Direction direction;
	  
 };
 /*STRUTTURA DATI PER LO STATO DEL GIOCO*/
 typedef enum
{
    n_boards,
    npc_v_player,
    game,
    in_game,
} Menu;

/*Disegna la scacchiera di gioco, inizializza i giocatori*/
void Draw_board(uint16_t Xpos, uint16_t Ypos, uint16_t color);

/*Ricerca l'esistenza di un cammino per ogni giocatore. Utile per verificare che il muro non rappresenti una trappola*/
int is_valid(void);

/*Funzioni per la rappresentazione grafica*/
void GUI_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t high, uint16_t  lenght, uint16_t color);
void GUI_Square(uint16_t Xpos, uint16_t Ypos, uint16_t side, uint16_t color);
void GUI_Board(uint16_t Xpos, uint16_t Ypos, uint16_t R, uint16_t C,uint16_t color );
void GUI_Player(struct Player player, uint16_t color);

/*Disegna un muro e ne verifica la correttezza della posizione*/
uint16_t Draw_wall(int i, int j, int is_horizontal, int del );

/*Muove il giocatore nella direzione fornitagli in input*/
void move_player(struct Player *p,Direction direction,struct Player adversary);

/*Muove il muro nella direzione fornitagli in input*/
uint16_t move_wall(struct Wall *wall, Direction d);

/*Rimuove il muro*/
void remove_wall(struct Player *player);	

/*Aggiunge un nuovo muro*/
void new_wall(struct Player *player);

/*Ruota il muro*/
uint16_t rotate_wall(struct Player *player);

/*Deriva le mossse disponibili del giocatore*/
void derive_next_moves(struct Player *player , struct Board b, struct Player adversary);

/*Rappresenta graficamente le mosse disponibili*/
void GUI_next_moves(struct Player p, uint16_t color);

/*Inizializza la struttura dati di un giocatore*/
void init_player(uint8_t my_i, uint8_t my_j, uint8_t vis_i, uint8_t vis_j, struct Player *player, int id,uint16_t X_scoreboard,uint16_t Y_scoreboard, Direction direction);

/*Rappresenta graficamente la mossa selezionata*/
uint8_t GUI_select_next_move(struct Player p, Direction dir);

/*Salva la mossa all'interno dell'apposita struttura dati per tenere traccia della storia del gioco*/
void save_move(int id, int is_wall,int is_horizontal,int y, int x);

/*Aggiorna il tabellone di gioco*/
void update_scoreboard(struct Player p);

/*Valuta la vittoria del giocatore*/
uint8_t have_won(struct Player p);

/*Rappresenta graficamente la scelta nel menu*/
void GUI_which_choice(uint16_t X, uint16_t Y);

/*Funzioni di controllo*/
Queue newQueue(void);
Point newPoint(int i, int j);
void enqueue(Queue *q, Point p);
Point dequeue(Queue *q);
void findPath(Point start_player1, Point start_player2, Direction dir_p1, Direction dir_p2, int *min_path_p1, int *min_path_p2);

/*Mossa del giocatore NPC*/
void AI_move(void);

/*Funzioni per il modulo CAN*/
uint32_t convert_general2move(uint32_t move);
uint32_t convert_move2general(uint32_t move);
void unwrap_move(uint32_t move);
/******************************************************************************
**                            End Of File
******************************************************************************/