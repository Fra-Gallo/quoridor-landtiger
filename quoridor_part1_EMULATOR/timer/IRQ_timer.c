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

int count = SECONDS_PER_MOVE;
extern int is_move;
extern int have_wall;
extern uint16_t new_wall_color;
extern int won;
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
    count--;
    if (count)
    {
        sprintf(output, " TIMER %2d", count);
        GUI_Text(88, 225, (uint8_t *)output, Blue, White);
    }
    if (!count && !won)
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
        count = SECONDS_PER_MOVE;
				GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
    }

    LPC_TIM1->IR = 1; /* clear interrupt flag */
    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
