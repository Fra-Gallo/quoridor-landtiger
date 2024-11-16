/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <lpc17xx.h> /* LPC17xx definitions */
#include "CAN.h"     /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include "../Game.h"
#include "../RIT/RIT.h"

extern uint8_t icr; // icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg; /* CAN message for sending */
extern CAN_msg CAN_RxMsg; /* CAN message for receiving */
extern uint8_t which_menu;
extern int increment_menu;

void do_received_move(CAN_msg msg);
int receive_handshake(CAN_msg msg);
int i_am_ready(CAN_msg msg);

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void)
{

    /* check CAN controller 1 */
    icr = 0;
    icr = (LPC_CAN1->ICR | icr) & 0xFF; /* clear interrupts */

    if (icr & (1 << 0))
    {                             /* CAN Controller #1 meassage is received */
        CAN_rdMsg(1, &CAN_RxMsg); /* Read the message */
        LPC_CAN1->CMR = (1 << 2); /* Release receive buffer */

        do_received_move(CAN_RxMsg);
    }

    /* check CAN controller 2 */
    icr = 0;
    icr = (LPC_CAN2->ICR | icr) & 0xFF; /* clear interrupts */

    if (icr & (1 << 0))
    {                             /* CAN Controller #2 meassage is received */
        CAN_rdMsg(2, &CAN_RxMsg); /* Read the message */
        LPC_CAN2->CMR = (1 << 2); /* Release receive buffer */

        do_received_move(CAN_RxMsg);
    }
}

void do_received_move(CAN_msg msg)
{

    uint32_t move = (msg.data[3] << 24) + (msg.data[2] << 16) + (msg.data[1] << 8) + msg.data[0];
	

    if (receive_handshake(msg))
    {
        return;
    }

    if (i_am_ready(msg))
        return;
    move = convert_general2move(move);
    board.moves[board.last_move] = move;
    unwrap_move(move);



    if (is_mp > 0 && is_npc > 0)
    {
        
        AI_move();
      
    }
}
int receive_handshake(CAN_msg msg)
{
    if (msg.data[3] == 0xFF)
    {
        if (which_menu < 1)
        {
            if (my_mp_id == -1)
            {
                my_mp_id = 2;
                is_mp = 1;
	
                which_menu = npc_v_player;
							
			
							
                send_message(0xFF000000);
                LCD_Clear(White);
                GUI_Text(0, 15, (uint8_t *)" SELECT YOUR PLAYER ", Blue, White);
                GUI_Text(30, 80, (uint8_t *)" NPC ", Blue, White);
                GUI_Text(30, 180, (uint8_t *)" MULTIPLAYER", Blue, White);
      
			
                return 1;
            }
            else if(my_mp_id == 1)
            {
  
							
							disable_RIT();
								init_timer(0,RIT_INTERVAL);
							enable_timer(0);
				

                return 1;
            }

        }
        return 1; // IGNORA IL MESSAGGIO SENZA EFFETTI
    }
    return 0;
}
void send_message(uint32_t message)
{

	message = convert_move2general(message);
	
	
    CAN_TxMsg.data[3] = message >> 24;
    CAN_TxMsg.data[2] = (message & 0x00FF0000) >> 16;
    CAN_TxMsg.data[1] = (message & 0x0000FF00) >> 8;
    CAN_TxMsg.data[0] = (message & 0x000000FF);
    CAN_TxMsg.len = 4;
    CAN_TxMsg.format = STANDARD_FORMAT;
    CAN_TxMsg.type = DATA_FRAME;
    CAN_TxMsg.id = 1;
    CAN_wrMsg(1, &CAN_TxMsg);
    CAN_TxMsg.id = 2;
    CAN_wrMsg(2, &CAN_TxMsg);
}

int i_am_ready(CAN_msg msg)
{

    if (msg.data[3] == 0xFE)
    {

        go_ahead_adv = 1;

        if (is_npc != -1 && go_ahead == 1 && go_ahead_adv == 1)
        {
            GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);

					if(my_mp_id == 1)
           init_timer(1, 0x17D7840);
            which_menu=game;
            Draw_board(10, 15, Blue2);

            if (is_npc)
            {

                if (my_mp_id == 1)
                {
                    GUI_rectangle(10, 250, 50, 100, Red);
                    GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);
                    update_scoreboard(player2);

                    GUI_rectangle(120, 250, 50, 100, White);
                    GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);
                    update_scoreboard(playerAI);

                    AI_move();
                }
                else
                {
                    GUI_rectangle(10, 250, 50, 100, Red);
                    GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);
                    update_scoreboard(playerAI);

                    GUI_rectangle(120, 250, 50, 100, White);
                    GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);
                    update_scoreboard(player1);
                    enable_timer(1);
                }
            }
            else
            {

                GUI_rectangle(10, 250, 50, 100, Red);
                GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);
                update_scoreboard(player1);

                GUI_rectangle(120, 250, 50, 100, White);
                GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);
                update_scoreboard(player2);
                enable_timer(1);
            }
        }
        return 1;
    }
    return 0;
}

