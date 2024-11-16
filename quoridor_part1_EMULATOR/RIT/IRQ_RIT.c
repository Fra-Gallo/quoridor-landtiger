/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../Game.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down = 0;
volatile int wich_button = 0;
int is_move = 1;
int have_wall = 0;
Direction direction;
uint16_t new_wall_color;
uint8_t is_next_move_valid;
extern int count; // conteggio timer;
int won = 0;

void RIT_IRQHandler(void)
{
    static int J_select = 0;
    static int J_down = 0;
    static int J_up = 0;
    static int J_right = 0;
    static int J_left = 0;
    // static int position=0;
	  static int J_up_left =0;
	  static int J_down_left =0;
	  static int J_down_right =0;
	  static int J_up_right = 0; 
	
	if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
    {
        // DOWN +LEFT 

        J_down_left++;
        switch (J_down_left)
        {
        case 1:
					if(is_move){
					  if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, giu_sinistra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, giu_sinistra);
                }
                direction = giu_sinistra;
							}


            J_down_left = 0;

            break;
        default:
            break;
        }
    }
    else
    {
        J_down_left = 0;
    }
		if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
    {
        // DOWN +RIGHT

        J_down_right++;
        switch (J_down_right)
        {
        case 1:
					if(is_move){
					  if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, giu_destra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, giu_destra);
                }
                direction = giu_destra;
							}


            J_down_right = 0;

            break;
        default:
            break;
        }
    }
    else
    {
        J_down_right = 0;
    }
			if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 28)) == 0)
    {
        // UP + RIGHT

        J_up_right++;
        switch (J_up_right)
        {
        case 1:
					if(is_move){
					  if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, su_destra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, su_destra);
                }
                direction = su_destra;
							}


            J_up_right = 0;

            break;
        default:
            break;
        }
    }
    else
    {
        J_up_right = 0;
    }
			if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
    {
        // UP + LEFT

        J_up_left++;
        switch (J_up_left)
        {
        case 1:
					if(is_move){
					  if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, su_sinistra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, su_sinistra);
                }
                direction = su_sinistra;
							}


            J_up_left = 0;

            break;
        default:
            break;
        }
    }
    else
    {
        J_up_left = 0;
    }
		

    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) && (LPC_GPIO1->FIOPIN & (1 << 28)) )
    {
       // DOWN

        J_down++;
        switch (J_down)
        {
        case 1:

            if (!is_move)
            {

                if (board.who_play == player1.id)
                {
                    new_wall_color = move_wall(&player1.walls[player1.last_wall], giu);
                }
                else
                {
                    new_wall_color = move_wall(&player2.walls[player2.last_wall], giu);
                }
            }
            else
            {
                if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, giu);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, giu);
                }
                direction = giu;
            }

            J_down = 0;

            break;
        default:
            break;
        }
    }
    else
    {
        J_down = 0;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0)
    {
        // SELECT

        J_select++;
        switch (J_select)
        {
        case 1:

            if (!is_move)
            {

                if (board.who_play == player1.id)
                {
                    if (new_wall_color == Green)
                    { // se il muro è in una posizione invalida il tasto select non ha effetto
                        save_move(board.who_play - 1, 1, player1.walls[player1.last_wall].horizontal, player1.visual_j, player1.visual_i);
                        board.who_play = 2;
                        derive_next_moves(&player2, board, player1);
                        GUI_next_moves(player2, Yellow);
                        is_move = 1;
                        new_wall_color = 0;
                        have_wall = 0;
                        count = SECONDS_PER_MOVE;
											 GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                    }
                }

                else
                {
                    if (new_wall_color == Green)
                    {
                        save_move(board.who_play - 1, 1, player2.walls[player2.last_wall].horizontal, player2.visual_j, player2.visual_i);
                        board.who_play = 1;
                        derive_next_moves(&player1, board, player2);
                        GUI_next_moves(player1, Yellow);
                        is_move = 1;
                        new_wall_color = 0;
                        have_wall = 0;
                        count = SECONDS_PER_MOVE;
											 GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                    }
                }
            }
            else
            {
                if (board.who_play == player1.id)
                {
                    if (is_next_move_valid)
                    {
                        save_move(board.who_play - 1, 0, 0, player1.visual_j, player1.visual_i);
                        move_player(&player1, direction, player2);

                        board.who_play = 2;
                        GUI_Player(player1, Red);

                        if (!(won = have_won(player1)))
                        {

                            // necessario per la prossima mossa
                            derive_next_moves(&player2, board, player1);
                            GUI_next_moves(player2, Yellow);

                            is_move = 1;
                            is_next_move_valid = 0;
                            count = SECONDS_PER_MOVE;
													 GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                        }
                        else
                        {
                            count = 1;
                            disable_timer(1);
                        }
                    }
                }
                else
                {
                    if (is_next_move_valid)
                    {
                        save_move(board.who_play - 1, 0, 0, player2.visual_j, player2.visual_i);
                        move_player(&player2, direction, player1);
                        board.who_play = 1;
                        GUI_Player(player2, White);

                        if (!(won = have_won(player2)))
                        {

                            // necessario per la prossima mossa
                            derive_next_moves(&player1, board, player2);
                            GUI_next_moves(player1, Yellow);

                            is_move = 1;
                            is_next_move_valid = 0;
                            count = SECONDS_PER_MOVE;
													 GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                        }
                        else
                        {
                            count = 1;
                            disable_timer(1);
                        }
                    }
                }
            }

            J_select = 0;
            break;
        default:
            break;
        }
    }
    else
    {
        J_select = 0;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 29)) &&  (LPC_GPIO1->FIOPIN & (1 << 26)) )
    {
        // LEFT

        J_left++;
        switch (J_left)
        {
        case 1:

            if (!is_move)
            {

                if (board.who_play == player1.id)
                {
                    new_wall_color = move_wall(&player1.walls[player1.last_wall], sinistra);
                }
                else
                {
                    new_wall_color = move_wall(&player2.walls[player2.last_wall], sinistra);
                }
            }
            else
            {

                if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, sinistra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, sinistra);
                }
                direction = sinistra;
                J_left = 0;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        J_left = 0;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 29)) &&  (LPC_GPIO1->FIOPIN & (1 << 26)))
    {
        // RIGHT

        J_right++;
        switch (J_right)
        {
        case 1:

            if (!is_move)
            {

                if (board.who_play == player1.id)
                {
                    new_wall_color = move_wall(&player1.walls[player1.last_wall], destra);
                }
                else
                {
                    new_wall_color = move_wall(&player2.walls[player2.last_wall], destra);
                }
            }
            else
            {
                if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, destra);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, destra);
                }
                direction = destra;
            }

            J_right = 0;
            break;
        default:
            break;
        }
    }
    else
    {
        J_right = 0;
    }
    if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) && (LPC_GPIO1->FIOPIN & (1 << 28)))
    {
        // UP

        J_up++;
        switch (J_up)
        {
        case 1:

            if (!is_move)
            {

                if (board.who_play == player1.id)
                {
                    new_wall_color = move_wall(&player1.walls[player1.last_wall], su);
                }
                else
                {
                    new_wall_color = move_wall(&player2.walls[player2.last_wall], su);
                }
            }
            else
            {
                if (board.who_play == player1.id)
                {
                    // è il turno di player 1
                    // può selezionare questa casella come prossima mossa
                    GUI_next_moves(player1, Yellow);
                    is_next_move_valid = GUI_select_next_move(player1, su);
                }
                else
                {
                    // è il turno di player 2
                    GUI_next_moves(player2, Yellow);
                    is_next_move_valid = GUI_select_next_move(player2, su);
                }
                direction = su;
            }

            J_up = 0;
            break;
        default:
            break;
        }
    }
    else
    {
        J_up = 0;
    }

    /* button management */
    if (down != 0)
    {

        if (wich_button == 1)
        { /* KEY1 pressed */
            down++;
            switch (down)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */

                is_move = is_move ? 0 : 1;

                if (!is_move)
                {
                    if (board.who_play == player1.id)
                    {

											if(player1.tot_walls){
                        have_wall = 1;
                        new_wall(&player1);
                        update_scoreboard(player1);
                        new_wall_color = Draw_wall(player1.walls[player1.last_wall].i, player1.walls[player1.last_wall].j, 1, 0);
                        GUI_next_moves(player1, Blue2);
                    }else{
											is_move = 1;
										}
									}
                    else
                    {
					
											if(player2.tot_walls){
                        have_wall = 1;
                        new_wall(&player2);
                        update_scoreboard(player2);
                        new_wall_color = Draw_wall(player2.walls[player2.last_wall].i, player2.walls[player2.last_wall].j, 1, 0);
                        GUI_next_moves(player2, Blue2);
											}else{
												is_move = 1;
											}
                    }
                }
                else
                {
                    if (board.who_play == player1.id)
                    {

                        if (have_wall)
                        {
                            remove_wall(&player1);
                            update_scoreboard(player1);
                        }
                        is_next_move_valid = 0;

                        GUI_next_moves(player1, Yellow);
                        have_wall = 0;
                    }
                    else
                    {
                        if (have_wall)
                        {
                            remove_wall(&player2);
                            update_scoreboard(player2);
                        }

                        is_next_move_valid = 0;
                        GUI_next_moves(player2, Yellow);
                        have_wall = 0;
                    }
                }

                down = 0;
                wich_button = -1;
                NVIC_EnableIRQ(EINT1_IRQn);
                LPC_PINCON->PINSEL4 |= (1 << 22);
                break;
            default:
                break;
            }
        }

        else if (wich_button == 2)
        { /* KEY2 pressed */ //(LPC_GPIO2->FIOPIN & (1<<12)) == 0
            down++;
            switch (down)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */

                if (!is_move)
                {

                    if (board.who_play == player1.id)
                    {

                        if (have_wall)
                            new_wall_color = rotate_wall(&player1);
                    }
                    else
                    {
                        if (have_wall)
                            new_wall_color = rotate_wall(&player2);
                    }
                }

                down = 0;
                wich_button = -1;
                NVIC_EnableIRQ(EINT2_IRQn);
                LPC_PINCON->PINSEL4 |= (1 << 24); /* enable Button interrupts			*/

                break;
            default:
                break;
            }
        }

        else if (wich_button == 0)
        { /* INT0 pressed */ //(LPC_GPIO2->FIOPIN & (1<<10)) == 0

            down++;
            switch (down)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */
							                init_timer(1, 0x17D7840);
                enable_timer(1);
				
               

                Draw_board(10, 15, Blue2);

                /*dati visibili giocatore 1*/
                GUI_rectangle(10, 250, 50, 100, Red);
                GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);
                update_scoreboard(player1);
                //   GUI_Text(90,255,(uint8_t *)"8",Blue,White);

                /*dati visibili giocatore 2*/
                GUI_rectangle(120, 250, 50, 100, White);
                GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);
                update_scoreboard(player2);
                //  GUI_Text(200,255,(uint8_t *)"8",Blue,White);
						 GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);



                NVIC_EnableIRQ(EINT0_IRQn);
                LPC_PINCON->PINSEL4 |= (1 << 20); /* External interrupt 0 pin selection */
                down = 0;
                wich_button = -1;

                break;
            default:
                break;
            }
        }
        else
        { /* button released */
            down = 0;
            wich_button = -1;

            NVIC_EnableIRQ(EINT0_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 20); /* External interrupt 0 pin selection */
            NVIC_EnableIRQ(EINT1_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 22); /* enable Button interrupts			*/
            NVIC_EnableIRQ(EINT2_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 24); /* enable Button interrupts			*/
        }
    }

    LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
                            // reset_RIT();

    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
