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
#include "../CAN/CAN.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int downk1 = 0;
volatile int downk2 = 0;
volatile int downI0 = 0;
uint32_t temp_msg = 0xFE000000;
int is_move = 1;
int have_wall = 0;
Direction direction;
uint16_t new_wall_color;
uint8_t is_next_move_valid;
extern int count; // conteggio timer;
int won = 0;
int go_ahead_adv = 0;
uint8_t which_menu = 0;
int is_npc = -1;
int is_mp = -1;
int my_mp_id = -1;
uint8_t go_ahead = 0;

void RIT_IRQHandler(void)
{
    static  int J_select = 0;
    static  int J_down = 0;
    static  int J_up = 0;
    static  int J_right = 0;
    static  int J_left = 0;
    static  int J_up_left = 0;
    static  int J_down_left = 0;
    static  int J_down_right = 0;
    static  int J_up_right = 0;

    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) == 0)
    {
        // DOWN +LEFT

        J_down_left++;
        switch (J_down_left)
        {
        case 1:
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):

                if (is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == my_mp_id)
                        {

                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, giu_sinistra);
                        }

                        direction = giu_sinistra;
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == player1.id)
                        {

                            GUI_next_moves(player1, Yellow);
                            is_next_move_valid = GUI_select_next_move(player1, giu_sinistra);
                        }
                        else
                        {

                            GUI_next_moves(player2, Yellow);
                            is_next_move_valid = GUI_select_next_move(player2, giu_sinistra);
                        }
                        direction = giu_sinistra;
                    }
                }
                else if (!is_mp && is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play != playerAI.id)
                        {
                            struct Player adv = playerAI.id == 1 ? player2 : player1;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, giu_sinistra);
                        }

                        direction = giu_sinistra;
                    }
                }

                break;
            default:
                break;
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
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):
                if (is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == my_mp_id)
                        {

                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, giu_destra);
                            direction = giu_destra;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == player1.id)
                        {

                            GUI_next_moves(player1, Yellow);
                            is_next_move_valid = GUI_select_next_move(player1, giu_destra);
                        }
                        else
                        {

                            GUI_next_moves(player2, Yellow);
                            is_next_move_valid = GUI_select_next_move(player2, giu_destra);
                        }
                        direction = giu_destra;
                    }
                }
                else if (!is_mp && is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play != playerAI.id)
                        {

                            // può selezionare questa casella come prossima mossa
                            struct Player adv = playerAI.id == 1 ? player2 : player1;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, giu_destra);
                        }

                        direction = giu_destra;
                    }
                }
                break;
            default:
                break;
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
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):
                if (is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == my_mp_id)
                        {

                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, su_destra);
                            direction = su_destra;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
                    if (is_move)
                    {
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
                }
                else if (!is_mp && is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play != playerAI.id)
                        {

                            // può selezionare questa casella come prossima mossa
                            struct Player adv = playerAI.id == 1 ? player2 : player1;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, su_destra);
                        }

                        direction = su_destra;
                    }
                }
                break;
            default:
                break;
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
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):
                if (is_mp && !is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play == my_mp_id)
                        {
                            // è il turno di player 1
                            // può selezionare questa casella come prossima mossa
                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, su_sinistra);
                            direction = su_sinistra;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
                    if (is_move)
                    {
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
                }
                else if (!is_mp && is_npc)
                {
                    if (is_move)
                    {
                        if (board.who_play != playerAI.id)
                        {

                            // può selezionare questa casella come prossima mossa
                            struct Player adv = playerAI.id == 1 ? player2 : player1;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, su_sinistra);
                        }

                        direction = su_sinistra;
                    }
                }
                break;
            default:
                break;
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

    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 27)) && (LPC_GPIO1->FIOPIN & (1 << 28)))
    {
        // DOWN

        J_down++;
        switch (J_down)
        {
        case 1:
            switch (which_menu)
            {
            case (n_boards):

                // tot_players = 2;
                is_mp = 1;

                GUI_Square(0, 80, SIDE_SQUARE_BOARD, White);
                GUI_which_choice(0, 180);

                break;

            case (npc_v_player):
            //    enable_RIT();
                if (!is_mp)
                    go_ahead_adv = 1;
                is_npc = 0;
                GUI_Square(0, 80, SIDE_SQUARE_BOARD, White);
                GUI_which_choice(0, 180);
                break;

            case (game):

                if (is_mp && !is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play == my_mp_id)
                        {
                            new_wall_color = move_wall(my_mp_id == 1 ? &player1.walls[player1.last_wall] : &player2.walls[player2.last_wall], giu);
                        }
                    }
                    else
                    {
                        if (board.who_play == my_mp_id)
                        {
                            // è il turno di player 1
                            // può selezionare questa casella come prossima mossa
                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, giu);
                            direction = giu;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
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
                }
                else if (!is_mp && is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play != playerAI.id)
                        {
                            new_wall_color = move_wall(playerAI.id == 2 ? &player1.walls[player1.last_wall] : &player2.walls[player2.last_wall], giu);
                        }
                    }
                    else
                    {
                        if (board.who_play != playerAI.id)
                        {
                            // è il turno di player 1
                            // può selezionare questa casella come prossima mossa
                            struct Player adv = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, giu);
                            direction = giu;
                        }
                    }
                }
                break;
            default:
                break;
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
            switch (which_menu)
            {
            case (n_boards):
                if (is_mp == 0)
                {
                   which_menu++;
                    LCD_Clear(White);
                    GUI_Text(0, 15, (uint8_t *)" SELECT YOUR OPPONENT ", Blue, White);
                    GUI_Text(30, 80, (uint8_t *)" NPC ", Blue, White);
                    GUI_Text(30, 180, (uint8_t *)" MULTIPLAYER", Blue, White);
                }
                if (is_mp == 1)
                {
                    send_message(0xFF000000);
                    init_timer(0, wait_handshake);
                    enable_timer(0);
                }

                break;
            case (npc_v_player):

              go_ahead = 1;
						
						if(go_ahead && is_mp)
							GUI_Text(0, 280, (uint8_t *)" waiting the opponent", Blue, White);
						
						if(is_mp == 1){
                temp_msg += (my_mp_id << 16);
                send_message(temp_msg);
						}
                
                if (is_npc != -1 && go_ahead == 1 && go_ahead_adv == 1)
                {
                    which_menu=game;
                    Draw_board(10, 15, Blue2);



                    GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);

                    init_timer(1, 0x17D7840);
                    enable_timer(1);
                }

                break;
            case (game):

                if (is_mp && !is_npc)
                {
                    struct Player p;
                    if (!is_move)
                    {
                        if (board.who_play == my_mp_id)
                        {
                            if (new_wall_color == Green)
                            { // se il muro è in una posizione invalida il tasto select non ha effetto

                                p = my_mp_id == 1 ? player1 : player2;
                                save_move(p.id - 1, 1, p.walls[p.last_wall].horizontal, p.walls[p.last_wall].j, p.walls[p.last_wall].i);

                                send_message(board.moves[board.last_move]);

                                disable_timer(1);
                                board.who_play = my_mp_id == 1 ? 2 : 1;

                                is_move = 1;
                                new_wall_color = 0;
                                have_wall = 0;
                                count = SECONDS_PER_MOVE;
                                GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);

                                is_next_move_valid = 0;

                                save_move(p.id - 1, 0, 0, p.visual_j, p.visual_i);

                                send_message(board.moves[board.last_move]);
                                disable_timer(1);
                            }
                        }
                    }
                    else
                    {
                        if (board.who_play == my_mp_id)
                        {
                            if (is_next_move_valid)
                            {

                                move_player(my_mp_id == 1 ? &player1 : &player2, direction, my_mp_id == 1 ? player2 : player1);
                                p = my_mp_id == 1 ? player1 : player2;
                                GUI_Player(p, p.id == 1 ? Red : White);
                                board.who_play = p.id == 1 ? 2 : 1;

                                if (!(won = have_won(p)))
                                {

                                    is_move = 1;
                                    is_next_move_valid = 0;
                                    count = SECONDS_PER_MOVE;
                                    GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                                    new_wall_color = 0;
                                    have_wall = 0;
                                    save_move(p.id - 1, 0, 0, p.visual_j, p.visual_i);

                                    /**
                                     * MANDA IL MESSAGGIO DELLA MOSSA SALVATA
                                     */
                                    send_message(board.moves[board.last_move]);
                                    disable_timer(1);
                                }
                                else
                                {
                                    uint32_t idWin = (my_mp_id - 1) << 24;
                                    idWin += 0xFFFFFF;
                                    send_message(idWin);
                                    count = 1;
                                    disable_timer(1);
                                }
                            }
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ;
                    /**
                     *IN QUESTO CASO VIENE INVIATA LA MOSSA DIRETTAMENTE DALLA FUNZIONE
                     *do_received_move() che si  occupa di eseguire la mossa dell'AI
                     */
                }
                else if (!is_mp && !is_npc)
                {

                    if (!is_move)
                    {

                        if (board.who_play == player1.id)
                        {
                            if (new_wall_color == Green)
                            { // se il muro è in una posizione invalida il tasto select non ha effetto
                                save_move(board.who_play - 1, 1, player1.walls[player1.last_wall].horizontal, player1.walls[player1.last_wall].j, player1.walls[player1.last_wall].i);
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
                                save_move(board.who_play - 1, 1, player2.walls[player2.last_wall].horizontal, player2.walls[player2.last_wall].j, player2.walls[player2.last_wall].i);
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

                                move_player(&player1, direction, player2);

                                board.who_play = 2;
                                GUI_Player(player1, Red);

                                if (!(won = have_won(player1)))
                                {

                                    // necessario per la prossima mossa
                                    derive_next_moves(&player2, board, player1);
                                    GUI_next_moves(player2, Yellow);
                                    save_move(board.who_play - 1, 0, 0, player1.visual_j, player1.visual_i);
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

                                move_player(&player2, direction, player1);
                                board.who_play = 1;
                                GUI_Player(player2, White);

                                if (!(won = have_won(player2)))
                                {

                                    // necessario per la prossima mossa
                                    derive_next_moves(&player1, board, player2);
                                    GUI_next_moves(player1, Yellow);
                                    save_move(board.who_play - 1, 0, 0, player2.visual_j, player2.visual_i);
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
                }
                else if (!is_mp && is_npc)
                {
                    struct Player p;
                    if (!is_move)
                    {
                        if (board.who_play != playerAI.id)
                        {
                            if (new_wall_color == Green)
                            { // se il muro è in una posizione invalida il tasto select non ha effetto
                                p = playerAI.id == 1 ? player2 : player1;
                                save_move(p.id - 1, 1, p.walls[p.last_wall].horizontal, p.walls[p.last_wall].j, p.walls[p.last_wall].i);
                                board.who_play = playerAI.id;

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
                        if (board.who_play != playerAI.id)
                        {
                            if (is_next_move_valid)
                            {

                                move_player(playerAI.id == 1 ? &player2 : &player1, direction, playerAI);
                                p = playerAI.id == 1 ? player2 : player1;
                                GUI_Player(p, p.id == 1 ? Red : White);

                                board.who_play = playerAI.id;

                                if (!(won = have_won(p)))
                                {
                                    save_move(p.id - 1, 0, 0, p.visual_j, p.visual_i);
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
                    AI_move();
                }

                break;
            default:
                break;
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

    if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 29)) && (LPC_GPIO1->FIOPIN & (1 << 26)))
    {
        // LEFT

        J_left++;
        switch (J_left)
        {
        case 1:
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):

                if (is_mp && !is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play == my_mp_id)
                        {
                            new_wall_color = move_wall(my_mp_id == 1 ? &player1.walls[player1.last_wall] : &player2.walls[player2.last_wall], sinistra);
                        }
                    }
                    else
                    {
                        if (board.who_play == my_mp_id)
                        {
                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, sinistra);
                            direction = sinistra;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
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
                    }
                }
                else if (!is_mp && is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play != playerAI.id)
                        {
                            new_wall_color = move_wall(playerAI.id == 1 ? &player2.walls[player2.last_wall] : &player1.walls[player1.last_wall], sinistra);
                        }
                    }
                    else
                    {
                        if (board.who_play != playerAI.id)
                        {
                            // è il turno di player 1
                            // può selezionare questa casella come prossima mossa
                            struct Player adv = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, sinistra);
                            direction = sinistra;
                        }
                    }
                }

                break;

            default:
                break;
            }
            J_left = 0;
            break;
        default:
            break;
        }
    }
    else
    {
        J_left = 0;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0 && (LPC_GPIO1->FIOPIN & (1 << 29)) && (LPC_GPIO1->FIOPIN & (1 << 26)))
    {
        // RIGHT

        J_right++;
        switch (J_right)
        {
        case 1:
            switch (which_menu)
            {
            case (n_boards):
                break;
            case (npc_v_player):
                break;
            case (game):
                if (is_mp && !is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play == my_mp_id)
                        {
                            new_wall_color = move_wall(my_mp_id == 1 ? &player1.walls[player1.last_wall] : &player2.walls[player2.last_wall], destra);
                        }
                    }
                    else
                    {
                        if (board.who_play == my_mp_id)
                        {
                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, destra);
                            direction = destra;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
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
                }
                else if (!is_mp && is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play != playerAI.id)
                        {
                            new_wall_color = move_wall(playerAI.id == 1 ? &player2.walls[player2.last_wall] : &player1.walls[player1.last_wall], destra);
                        }
                    }
                    else
                    {
                        if (board.who_play != playerAI.id)
                        {
                            // è il turno di player 1
                            // può selezionare questa casella come prossima mossa
                            struct Player adv = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, destra);
                            direction = destra;
                        }
                    }
                }

                break;
            default:
                break;
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
            switch (which_menu)
            {
            case (n_boards):
                // tot_players = 1;
                is_mp = 0;

                GUI_Square(0, 180, SIDE_SQUARE_BOARD, White);
                GUI_which_choice(0, 80);

                break;
            case (npc_v_player):
             //   enable_RIT();
                if (!is_mp)
                    go_ahead_adv = 1;
                is_npc = 1;
                GUI_Square(0, 180, SIDE_SQUARE_BOARD, White);
                GUI_which_choice(0, 80);
                break;
            case (game):
                if (is_mp && !is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play == my_mp_id)
                        {
                            new_wall_color = move_wall(my_mp_id == 1 ? &player1.walls[player1.last_wall] : &player2.walls[player2.last_wall], su);
                        }
                    }
                    else
                    {
                        if (board.who_play == my_mp_id)
                        {
                            struct Player p = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(p, Yellow);
                            is_next_move_valid = GUI_select_next_move(p, su);
                            direction = su;
                        }
                    }
                }
                else if (is_mp && is_npc)
                {
                    ; // vedi joystick select
                }
                else if (!is_mp && !is_npc)
                {
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
                            GUI_next_moves(player1, Yellow);
                            is_next_move_valid = GUI_select_next_move(player1, su);
                        }
                        else
                        {

                            GUI_next_moves(player2, Yellow);
                            is_next_move_valid = GUI_select_next_move(player2, su);
                        }
                        direction = su;
                    }
                }
                else if (!is_mp && is_npc)
                {
                    if (!is_move)
                    {

                        if (board.who_play != playerAI.id)
                        {
                            new_wall_color = move_wall(playerAI.id == 1 ? &player2.walls[player2.last_wall] : &player1.walls[player1.last_wall], su);
                        }
                    }
                    else
                    {
                        if (board.who_play != playerAI.id)
                        {
                            struct Player adv = my_mp_id == 1 ? player1 : player2;
                            GUI_next_moves(adv, Yellow);
                            is_next_move_valid = GUI_select_next_move(adv, su);
                            direction = su;
                        }
                    }
                }
                break;
            default:
                break;
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
    if (downk1 != 0)
    {

        if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0)
        { /* KEY1 pressed */
            downk1++;
            switch (downk1)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */
                switch (which_menu)
                {
                case (n_boards):
                    break;
                case (npc_v_player):
                    break;
                case (game):

                    if (is_mp && !is_npc)
                    {
                        struct Player p = my_mp_id == 2 ? player2 : player1;
                        if (is_move)
                        {
                            if (board.who_play == my_mp_id)
                            {

                                if (p.tot_walls)
                                {
                                    have_wall = 1;
                                    new_wall(my_mp_id == 2 ? &player2 : &player1);
                                    p = my_mp_id == 2 ? player2 : player1;
                                    update_scoreboard(p);
                                    new_wall_color = Draw_wall(p.walls[p.last_wall].i, p.walls[p.last_wall].j, 1, 0);
                                    GUI_next_moves(p, Blue2);
                                }
                                else
                                {
                                    is_move = 1;
                                }
                            }
                        }
                        else
                        {
                            if (board.who_play == my_mp_id)
                            {

                                if (have_wall)
                                {
                                    remove_wall(my_mp_id == 2 ? &player2 : &player1);
                                    update_scoreboard(my_mp_id == 2 ? player2 : player1);
                                }

                                is_next_move_valid = 0;
                                GUI_next_moves(my_mp_id == 2 ? player2 : player1, Yellow);
                                have_wall = 0;
                            }
                        }
                        is_move = is_move ? 0 : 1;
                    }
                    else if (is_mp && is_npc)
                    {
                        ;
                    }
                    else if (!is_mp && !is_npc)
                    {
                        if (is_move)
                        {
                            if (board.who_play == player1.id)
                            {

                                if (player1.tot_walls)
                                {
                                    have_wall = 1;
                                    new_wall(&player1);
                                    update_scoreboard(player1);
                                    new_wall_color = Draw_wall(player1.walls[player1.last_wall].i, player1.walls[player1.last_wall].j, 1, 0);
                                    GUI_next_moves(player1, Blue2);
                                }
                                else
                                {
                                    is_move = 1;
                                }
                            }
                            else
                            {

                                if (player2.tot_walls)
                                {
                                    have_wall = 1;
                                    new_wall(&player2);
                                    update_scoreboard(player2);
                                    new_wall_color = Draw_wall(player2.walls[player2.last_wall].i, player2.walls[player2.last_wall].j, 1, 0);
                                    GUI_next_moves(player2, Blue2);
                                }
                                else
                                {
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
                        is_move = is_move ? 0 : 1;
                    }
                    else if (!is_mp && is_npc)
                    {

                        struct Player p = playerAI.id == 1 ? player2 : player1;
                        if (is_move)
                        {
                            if (board.who_play != playerAI.id)
                            {

                                if (p.tot_walls)
                                {

                                    have_wall = 1;
                                    new_wall(playerAI.id == 1 ? &player2 : &player1);
                                    p = playerAI.id == 1 ? player2 : player1;
                                    update_scoreboard(p);
                                    new_wall_color = Draw_wall(p.walls[p.last_wall].i, p.walls[p.last_wall].j, 1, 0);
                                    GUI_next_moves(p, Blue2);
                                }
                                else
                                {
                                    is_move = 1;
                                }
                            }
                        }
                        else
                        {
                            if (board.who_play != playerAI.id)
                            {

                                if (have_wall)
                                {
                                    remove_wall(playerAI.id == 1 ? &player2 : &player1);
                                    update_scoreboard(playerAI.id == 1 ? player2 : player1);
                                }

                                is_next_move_valid = 0;
                                GUI_next_moves(playerAI.id == 1 ? player2 : player1, Yellow);
                                have_wall = 0;
                            }
                        }
                        is_move = is_move ? 0 : 1;
                    }

                    break;
                default:
                    break;
                }

              
                break;
            default:
                break;
            }
        }
        else
        {
            downk1 = 0;
            NVIC_EnableIRQ(EINT1_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 22); /* enable Button interrupts			*/
        }
    }

    if (downk2 != 0)
    {

        if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0)
        { /* KEY2 pressed */ //(LPC_GPIO2->FIOPIN & (1<<12)) == 0
            downk2++;
            switch (downk2)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */
                switch (which_menu)
                {
                case (n_boards):
                    break;
                case (npc_v_player):
                    break;
                case (game):
                    if (is_mp && !is_npc)
                    {
                        if (board.who_play == my_mp_id && have_wall)
                            new_wall_color = rotate_wall(my_mp_id == 2 ? &player2 : &player1);
                    }
                    else if (is_mp && is_npc)
                    {
                        ;
                    }
                    else if (!is_mp && !is_npc)
                    {
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
                    }
                    else if (!is_mp && is_npc)
                    {
                        if (board.who_play != playerAI.id && have_wall)
                            new_wall_color = rotate_wall(playerAI.id == 1 ? &player2 : &player1);
                    }
                    break;
                default:
                    break;
                }

                break;
            default:
                break;
            }
        }
        else
        {
            downk2 = 0;
            NVIC_EnableIRQ(EINT2_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 24); /* External interrupt 0 pin selection */
        }
    }

    if (downI0 != 0)
    {

        if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0)
        { /* INT0 pressed */ //(LPC_GPIO2->FIOPIN & (1<<10)) == 0

            downI0++;
            switch (downI0)
            {
            case 2: /* pay attention here: please see slides to understand value 2 */

                which_menu = 0;
                switch (which_menu)
                {
                case (n_boards):
                    LCD_Clear(White);
                    GUI_Text(0, 15, (uint8_t *)" SELECT THE GAME MODE ", Blue, White);

                    GUI_Text(30, 80, (uint8_t *)" Single Board ", Blue, White);
                    GUI_Text(30, 180, (uint8_t *)" Two Board ", Blue, White);
                    my_mp_id = 1;

                    break;
                case (npc_v_player):
                    break;
                case (game):
                    break;
                default:
                    break;
                }

               

                break;
            default:
                break;
            }
        }
        else
        { /* button released */
            downI0 = 0;

            NVIC_EnableIRQ(EINT0_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 20); /* External interrupt 0 pin selection */
        }
    }

    LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
                            // reset_RIT();

    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
