
#include "LPC17xx.h"

#define R_BOARD 7
#define C_BOARD 7
#define MY_R 15
#define MY_C 15
#define SIDE_SQUARE_BOARD 25
#define N_WALLS 8
#define MAX_P 55


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


struct Coordinates{
	uint16_t posX;
	uint16_t posY;
};

 struct Board{
	 uint8_t my_board[MY_R][MY_C];
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
 
extern struct Board board;
extern struct Player player1;
extern struct Player player2;

void Draw_board(uint16_t Xpos, uint16_t Ypos, uint16_t color);
int is_valid(uint8_t matrix[MY_R][MY_C], uint8_t iP, uint8_t jP);
void GUI_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t high, uint16_t  lenght, uint16_t color);
void GUI_Square(uint16_t Xpos, uint16_t Ypos, uint16_t side, uint16_t color);
void GUI_Board(uint16_t Xpos, uint16_t Ypos, uint16_t R, uint16_t C,uint16_t color );
void GUI_Player(struct Player player, uint16_t color);
uint16_t Draw_wall(int i, int j, int is_horizontal, int del );
void move_player(struct Player *p,Direction direction,struct Player adversary);
uint16_t move_wall(struct Wall *wall, Direction d);
void remove_wall(struct Player *player);	
void new_wall(struct Player *player);
uint16_t rotate_wall(struct Player *player);
void derive_next_moves(struct Player *player , struct Board b, struct Player adversary);
void GUI_next_moves(struct Player p, uint16_t color);
void init_player(uint8_t my_i, uint8_t my_j, uint8_t vis_i, uint8_t vis_j, struct Player *player, int id,uint16_t X_scoreboard,uint16_t Y_scoreboard, Direction direction);
uint8_t GUI_select_next_move(struct Player p, Direction dir);
void save_move(int id, int is_wall,int is_horizontal,int y, int x);
void update_scoreboard(struct Player p);
uint8_t have_won(struct Player p);

/******************************************************************************
**                            End Of File
******************************************************************************/