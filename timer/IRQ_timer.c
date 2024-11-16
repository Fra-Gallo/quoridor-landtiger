/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../TouchPanel/TouchPanel.h"
#include "../Game.h"
#include "../CAN/CAN.h"
#include "../RIT/RIT.h"

int count = SECONDS_PER_MOVE;
extern int is_move;
extern int have_wall;
extern uint16_t new_wall_color;
extern int won;
extern uint8_t which_menu;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler(void)
{

	if (LPC_TIM0->MR0 == RIT_INTERVAL){
  

    which_menu = npc_v_player;

    LCD_Clear(White);
    GUI_Text(0, 15, (uint8_t *)" SELECT YOUR PLAYER ", Blue, White);
    GUI_Text(30, 80, (uint8_t *)" NPC ", Blue, White);
    GUI_Text(30, 180, (uint8_t *)" MULTIPLAYER", Blue, White);
    disable_timer(0);
 
		
        enable_RIT();
		
		}
	
		else{
			GUI_Text(18, 225, (uint8_t *)"multiplayer non disponibile", Blue, White);
    	  disable_timer(0);
		
		}
	disable_timer(0);
		
   

    LPC_TIM0->IR = 1; /* clear interrupt flag */
    return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void)
{
    char output[11];
	
	if(is_mp){
	if(board.who_play == my_mp_id)
    count--;
}else{count--;}

    if (count)
    {
        sprintf(output, " TIMER %2d", count);
        GUI_Text(88, 225, (uint8_t *)output, Blue, White);
    }
    if (!count && !won)
    {
        if (is_mp && !is_npc)
        {
            if (!is_move)
            {
                if (board.who_play == my_mp_id)
                {
                    if (have_wall)
                        remove_wall(my_mp_id == 1 ? &player1 : &player2);
                    save_move(board.who_play - 1, 0, 1, 0, 0);

                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                    update_scoreboard(my_mp_id == 1 ? player1 : player2);
                    board.who_play = my_mp_id == 1 ? 2 : 1;
                    send_message(board.moves[board.last_move]);
                }
            }
            else
            {

                GUI_next_moves(my_mp_id == 1 ? player1 : player2, Blue2);
                save_move(board.who_play - 1, 0, 1, 0, 0);

                is_move = 1;
                new_wall_color = 0;
                have_wall = 0;
                update_scoreboard(my_mp_id == 1 ? player1 : player2);
                board.who_play = my_mp_id == 1 ? 2 : 1;
                send_message(board.moves[board.last_move]);
            }
        }
        else if (is_mp && is_npc)
        {

            save_move(my_mp_id - 1, 0, 1, 0, 0);
            is_move = 1;
            new_wall_color = 0;
            have_wall = 0;

            board.who_play = my_mp_id == 1 ? 2 : 1;
            send_message(board.moves[board.last_move]);
        }
        else if (!is_mp && !is_npc)
        {
            if (!is_move)
            {
                if (board.who_play == player1.id)
                {
                    if (have_wall)
                        remove_wall(&player1);
                    save_move(board.who_play - 1, 0, 1, 0, 0);
                    board.who_play = 2;
                    derive_next_moves(&player2, board, player1);
                    GUI_next_moves(player2, Yellow);
                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                    update_scoreboard(player1);
                }
                else
                {

                    if (have_wall)
                        remove_wall(&player2);
                    save_move(board.who_play - 1, 0, 1, 0, 0);
                    board.who_play = 1;
                    derive_next_moves(&player1, board, player2);
                    GUI_next_moves(player1, Yellow);
                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                    update_scoreboard(player2);
                }
            }
            else
            {
                if (board.who_play == player1.id)
                {

                   
                    GUI_next_moves(player1, Blue2);
                    save_move(board.who_play - 1, 0, 1, 0, 0);
                    board.who_play = 2;
                    derive_next_moves(&player2, board, player1);
                    GUI_next_moves(player2, Yellow);
                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                }
                else
                {
                    GUI_next_moves(player2, Blue2);
                    save_move(board.who_play - 1, 0, 1, 0, 0);
                    board.who_play = 1;
                    derive_next_moves(&player1, board, player2);
                    GUI_next_moves(player1, Yellow);
                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                }
            }
        }
        else if (!is_mp && is_npc)
        {

            if (board.who_play != playerAI.id)
            {

                if (!is_move)
                {
                    if (have_wall)
                        remove_wall(playerAI.id == 1 ? &player2 : &player1);
                    save_move(board.who_play - 1, 0, 1, 0, 0);

                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;
                    update_scoreboard(playerAI.id == 1 ? player2 : player1);
                    board.who_play = playerAI.id;
                }

                else
                {

                    GUI_next_moves(playerAI.id == 1 ? player2 : player1, Blue2);
                    save_move(board.who_play - 1, 0, 1, 0, 0);

                    is_move = 1;
                    new_wall_color = 0;
                    have_wall = 0;

                    board.who_play = playerAI.id;
                }
            }
        }

        count = SECONDS_PER_MOVE;
        GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
				disable_timer(1);
    }

    LPC_TIM1->IR = 1; /* clear interrupt flag */
    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
