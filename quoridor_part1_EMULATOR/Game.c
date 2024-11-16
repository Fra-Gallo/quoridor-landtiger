#include "GLCD/GLCD.h"
#include "Game.h"

static int check(uint8_t i, uint8_t j, uint8_t iP, uint8_t jP, Direction direction);
int is_trap(uint8_t matrix[MY_R][MY_C], uint8_t i, uint8_t j, uint8_t iP, uint8_t jP, uint8_t iO, uint8_t jO, Direction direction, uint8_t *find, int count_per);

void Draw_board(uint16_t Xpos, uint16_t Ypos, uint16_t color)
{

    int i, j;

    LCD_Clear(Black);
    GUI_Board(10, 15, R_BOARD, C_BOARD, Blue2);

    for (i = 0, j = 0; j < MY_C; j++)
    {
        board.my_board[i][j] = 1;
    }

    for (i = MY_R - 1, j = 0; j < MY_C; j++)
    {
        board.my_board[i][j] = 1;
    }

    for (i = 0, j = 0; i < MY_R; i++)
    {
        board.my_board[i][j] = 1;
    }

    for (i = 0, j = MY_C - 1; i < MY_R; i++)
    {
        board.my_board[i][j] = 1;
    }

    for (i = 1; i < MY_R - 1; i++)
    {
        for (j = 1; j < MY_C - 1; j++)
        {
            board.my_board[i][j] = 0;
        }
    }

    init_player(7, 1, 0, 3, &player1, 1, 90, 255, destra);
    init_player(7, 13, 6, 3, &player2, 2, 200, 255, sinistra);

    derive_next_moves(&player1, board, player2);
    derive_next_moves(&player2, board, player1);

    GUI_Player(player1, Red);
    GUI_Player(player2, White);

    GUI_next_moves(player1, Yellow);

    board.last_move = -1;
    board.who_play = 1;
}

int is_valid(uint8_t matrix[MY_R][MY_C], uint8_t iP, uint8_t jP)
{

    uint8_t is_trap_v = 0;

    uint8_t i;
    uint8_t j;
    for (i = 1; i < MY_R - 1; i++)
    {
        for (j = 1; j < MY_C - 1; j++)
        {
            if (matrix[i][j] && i <= iP && j <= jP)
            {

                if (matrix[i][j + 1] && check(i, j + 1, iP, jP, destra))
                    is_trap(matrix, i, j + 1, iP, jP, i, j, destra, &is_trap_v, 0);

                if (matrix[i + 1][j] && check(i + 1, j, iP, jP, giu))
                    is_trap(matrix, i + 1, j, iP, jP, i, j, giu, &is_trap_v, 0);

                if (matrix[i - 1][j] && check(i - 1, j, iP, jP, su))
                    is_trap(matrix, i - 1, j, iP, jP, i, j, su, &is_trap_v, 0);

                if (matrix[i][j - 1] && check(i, j - 1, iP, jP, sinistra))
                    is_trap(matrix, i, j - 1, iP, jP, i, j, sinistra, &is_trap_v, 0);
            }
        }
    }

    return is_trap_v ? 0 : 1;
}

void derive_next_moves(struct Player *player, struct Board b, struct Player adversary)
{

    struct Player a = adversary;
    struct Player p = *player;

    uint8_t my_i = p.my_i;
    uint8_t my_j = p.my_j;

    uint8_t temp_my_i = my_i;
    uint8_t temp_my_j = my_j;

    uint8_t vis_i = p.visual_i;
    uint8_t vis_j = p.visual_j;
    uint8_t temp_vis_i = vis_i;
    uint8_t temp_vis_j = vis_j;

    uint8_t a_vis_i = a.visual_i;
    uint8_t a_vis_j = a.visual_j;
    uint8_t a_my_i = a.my_i;
    uint8_t a_my_j = a.my_j;
	
	int i ;
	
	for(i=0; i<8; i++){
	 p.is_nm_valid[i]=0;
	}

    if (!b.my_board[my_i - 1][my_j] && !b.my_board[my_i - 2][my_j])
    {

        if (vis_i == a_vis_i && vis_j - 1 == a_vis_j)
        {

            my_i = a_my_i;
            my_j = a_my_j;
            vis_i = a_vis_i;
            vis_j = a_vis_j;

            if (!b.my_board[my_i - 1][my_j] && !b.my_board[my_i - 2][my_j])
            {

                // controlla su rispetto all'avversario

                p.next_moves[su].posX = b.visual_board[vis_i][vis_j - 1].posX;
                p.next_moves[su].posY = b.visual_board[vis_i][vis_j - 1].posY;
                p.is_nm_valid[su] = 2;
            }else{


            if (!b.my_board[my_i][my_j + 1] && !b.my_board[my_i][my_j + 2])
            {

                p.is_nm_valid[su_destra] = 1;
                p.next_moves[su_destra].posX = b.visual_board[vis_i + 1][vis_j].posX;
                p.next_moves[su_destra].posY = b.visual_board[vis_i + 1][vis_j].posY;
            }

            if (!b.my_board[my_i][my_j - 1] && !b.my_board[my_i][my_j - 2])
            {

                p.is_nm_valid[su_sinistra] = 1;
                p.next_moves[su_sinistra].posX = b.visual_board[vis_i - 1][vis_j].posX;
                p.next_moves[su_sinistra].posY = b.visual_board[vis_i - 1][vis_j].posY;
            }
					}

            my_i = temp_my_i;
            my_j = temp_my_j;
            vis_i = temp_vis_i;
            vis_j = temp_vis_j;
        }
        else
        {

            p.next_moves[su].posX = b.visual_board[vis_i][vis_j - 1].posX;
            p.next_moves[su].posY = b.visual_board[vis_i][vis_j - 1].posY;
            p.is_nm_valid[su] = 1;
        }
    }
    else
    {
        p.is_nm_valid[su] = 0;
    }

    if (!b.my_board[my_i + 1][my_j] && !b.my_board[my_i + 2][my_j])
    {

        if (vis_i == a_vis_i && vis_j + 1 == a_vis_j)
        {

            my_i = a_my_i;
            my_j = a_my_j;
            vis_i = a_vis_i;
            vis_j = a_vis_j;

            if (!b.my_board[my_i + 1][my_j] && !b.my_board[my_i + 2][my_j])
            {

                p.next_moves[giu].posX = b.visual_board[vis_i][vis_j + 1].posX;
                p.next_moves[giu].posY = b.visual_board[vis_i][vis_j + 1].posY;
                p.is_nm_valid[giu] = 2;
            }else{
							  

            if (!b.my_board[my_i][my_j + 1] && !b.my_board[my_i][my_j + 2])
            {

                p.is_nm_valid[giu_destra] = 1;
                p.next_moves[giu_destra].posX = b.visual_board[vis_i + 1][vis_j].posX;
                p.next_moves[giu_destra].posY = b.visual_board[vis_i + 1][vis_j].posY;
            }

            if (!b.my_board[my_i][my_j - 1] && !b.my_board[my_i][my_j - 2])
            {

                p.is_nm_valid[giu_sinistra] = 1;
                p.next_moves[giu_sinistra].posX = b.visual_board[vis_i - 1][vis_j].posX;
                p.next_moves[giu_sinistra].posY = b.visual_board[vis_i - 1][vis_j].posY;
            }
					}

            my_i = temp_my_i;
            my_j = temp_my_j;
            vis_i = temp_vis_i;
            vis_j = temp_vis_j;
        }

        else
        {
            p.next_moves[giu].posX = b.visual_board[vis_i][vis_j + 1].posX;
            p.next_moves[giu].posY = b.visual_board[vis_i][vis_j + 1].posY;
            p.is_nm_valid[giu] = 1;
        }
    }
    else
    {
        p.is_nm_valid[giu] = 0;
    }

    if (!b.my_board[my_i][my_j + 1] && !b.my_board[my_i][my_j + 2])
    {

        if (vis_i + 1 == a_vis_i && vis_j == a_vis_j)
        {

            my_i = a_my_i;
            my_j = a_my_j;
            vis_i = a_vis_i;
            vis_j = a_vis_j;
					
				if (!b.my_board[my_i][my_j + 1] && !b.my_board[my_i][my_j + 2])
            {

                p.is_nm_valid[destra] = 2;
                p.next_moves[destra].posX = b.visual_board[vis_i + 1][vis_j].posX;
                p.next_moves[destra].posY = b.visual_board[vis_i + 1][vis_j].posY;
            }else{
							

            if (!b.my_board[my_i - 1][my_j] && !b.my_board[my_i - 2][my_j])
            {

                p.next_moves[su_destra].posX = b.visual_board[vis_i][vis_j - 1].posX;
                p.next_moves[su_destra].posY = b.visual_board[vis_i][vis_j - 1].posY;
                p.is_nm_valid[su_destra] = 1;
            }

            // non controlla giu perchè sarebbe da dove è arrivato

            if (!b.my_board[my_i + 1][my_j] && !b.my_board[my_i + 2][my_j])
            {

                p.next_moves[giu_destra].posX = b.visual_board[vis_i][vis_j + 1].posX;
                p.next_moves[giu_destra].posY = b.visual_board[vis_i][vis_j + 1].posY;
                p.is_nm_valid[giu_destra] = 1;
            }

					}

            my_i = temp_my_i;
            my_j = temp_my_j;
            vis_i = temp_vis_i;
            vis_j = temp_vis_j;
        }

        else
        {
            p.is_nm_valid[destra] = 1;
            p.next_moves[destra].posX = b.visual_board[vis_i + 1][vis_j].posX;
            p.next_moves[destra].posY = b.visual_board[vis_i + 1][vis_j].posY;
        }
    }
    else
    {
        p.is_nm_valid[destra] = 0;
    }

    if (!b.my_board[my_i][my_j - 1] && !b.my_board[my_i][my_j - 2])
    {

        if (vis_i - 1 == a_vis_i && vis_j == a_vis_j)
        {

            my_i = a_my_i;
            my_j = a_my_j;
            vis_i = a_vis_i;
            vis_j = a_vis_j;
					
			  if (!b.my_board[my_i][my_j - 1] && !b.my_board[my_i][my_j - 2])
            {

                p.is_nm_valid[sinistra] = 2;
                p.next_moves[sinistra].posX = b.visual_board[vis_i - 1][vis_j].posX;
                p.next_moves[sinistra].posY = b.visual_board[vis_i - 1][vis_j].posY;
            }
				else{

            if (!b.my_board[my_i - 1][my_j] && !b.my_board[my_i - 2][my_j])
            {

                p.next_moves[su_sinistra].posX = b.visual_board[vis_i][vis_j - 1].posX;
                p.next_moves[su_sinistra].posY = b.visual_board[vis_i][vis_j - 1].posY;
                p.is_nm_valid[su_sinistra] = 1;
            }

            if (!b.my_board[my_i + 1][my_j] && !b.my_board[my_i + 2][my_j])
            {

                p.next_moves[giu_sinistra].posX = b.visual_board[vis_i][vis_j + 1].posX;
                p.next_moves[giu_sinistra].posY = b.visual_board[vis_i][vis_j + 1].posY;
                p.is_nm_valid[giu_sinistra] = 1;
            }
					}


            my_i = temp_my_i;
            my_j = temp_my_j;
            vis_i = temp_vis_i;
            vis_j = temp_vis_j;
        }

        else
        {
            p.is_nm_valid[sinistra] = 1;
            p.next_moves[sinistra].posX = b.visual_board[vis_i - 1][vis_j].posX;
            p.next_moves[sinistra].posY = b.visual_board[vis_i - 1][vis_j].posY;
        }
    }
    else
    {
        p.is_nm_valid[sinistra] = 0;
    }

    *player = p;
}

void GUI_next_moves(struct Player p, uint16_t color)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (p.is_nm_valid[i])
        {
            GUI_Square(p.next_moves[i].posX, p.next_moves[i].posY, SIDE_SQUARE_BOARD, color);
        }
    }
}

int is_trap(uint8_t matrix[MY_R][MY_C], uint8_t i, uint8_t j, uint8_t iP, uint8_t jP, uint8_t iO, uint8_t jO, Direction direction, uint8_t *find, int count_per)
{
    if (count_per < MAX_P && i < MY_R && j < MY_C && i >= 0 && j >= 0)
    {

        if (i == iO && j == jO)
        {
            *find = 1;
            return 1;
        }
        if (i == MY_R - 1 || j == MY_C - 1 || i == 0 || j == 0)
        {

            if (*find == 0 && direction != sinistra && matrix[i][j + 1] && check(i, j + 1, iP, jP, destra) && i != MY_C - 1 && i > 0 && j + 1 < MY_C)
                is_trap(matrix, i, j + 1, iP, jP, iO, jO, destra, find, count_per);

            if (*find == 0 && direction != su && matrix[i + 1][j] && check(i + 1, j, iP, jP, giu) && j != MY_R - 1 && j > 0 && i + 1 < MY_R)
                is_trap(matrix, i + 1, j, iP, jP, iO, jO, giu, find, count_per);

            if (*find == 0 && direction != giu && matrix[i - 1][j] && check(i - 1, j, iP, jP, su) && j != MY_R - 1 && j > 0 && i - 1 >= 0)
                is_trap(matrix, i - 1, j, iP, jP, iO, jO, su, find, count_per);

            if (*find == 0 && direction != destra && matrix[i][j - 1] && check(i, j - 1, iP, jP, sinistra) && i != MY_C - 1 && i > 0 && j - 1 >= 0)
                is_trap(matrix, i, j - 1, iP, jP, iO, jO, sinistra, find, count_per);
        }

        count_per = (i == MY_R - 1 || j == MY_C - 1 || i == 0 || j == 0) ? count_per + 1 : count_per;

        if (*find == 0 && direction != sinistra && matrix[i][j + 1] && check(i, j + 1, iP, jP, destra) && j + 1 < MY_C)
            is_trap(matrix, i, j + 1, iP, jP, iO, jO, destra, find, count_per);

        if (*find == 0 && direction != su && matrix[i + 1][j] && check(i + 1, j, iP, jP, giu) && i + 1 < MY_R)
            is_trap(matrix, i + 1, j, iP, jP, iO, jO, giu, find, count_per);

        if (*find == 0 && direction != giu && matrix[i - 1][j] && check(i - 1, j, iP, jP, su) && i - 1 >= 0)
            is_trap(matrix, i - 1, j, iP, jP, iO, jO, su, find, count_per);

        if (*find == 0 && direction != destra && matrix[i][j - 1] && check(i, j - 1, iP, jP, sinistra) && j - 1 >= 0)
            is_trap(matrix, i, j - 1, iP, jP, iO, jO, sinistra, find, count_per);
    }

    return 0;
}

int check(uint8_t i, uint8_t j, uint8_t iP, uint8_t jP, Direction direction)
{

    int output = 1;

    if (iP != i && jP != j)
        return 1;

    switch (direction)
    {
    case su:
        if (j > jP)
            output = 0;
        break;

    case giu:
        if (j < jP)
            output = 0;
        break;

    case destra:
        if (i > iP)
            output = 0;
        break;

    case sinistra:
        if (i < iP)
            output = 0;
        break;
		default:
			break;
    }
    return output;
}

void GUI_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t high, uint16_t lenght, uint16_t color)
{
    int i;
    int j;
    for (i = 0; i < lenght; i++)
    {
        for (j = 0; j < high; j++)
        {
            LCD_SetPoint(Xpos + i, Ypos + j, color);
        }
    }
}

void GUI_Square(uint16_t Xpos, uint16_t Ypos, uint16_t side, uint16_t color)
{
    int i;
    int j;
    for (i = 0; i < side; i++)
    {
        for (j = 0; j < side; j++)
        {
            LCD_SetPoint(Xpos + i, Ypos + j, color);
        }
    }
}

void GUI_Board(uint16_t Xpos, uint16_t Ypos, uint16_t R, uint16_t C, uint16_t color)
{
    int i, j;
    for (i = 0; i < R; i++)
    {
        for (j = 0; j < C; j++)
        {

            board.visual_board[i][j].posX = Xpos + i * 30;
            board.visual_board[i][j].posY = Ypos + j * 30;

            GUI_Square(Xpos + i * 30, Ypos + j * 30, SIDE_SQUARE_BOARD, color);
        }
    }
}

void GUI_Player(struct Player player, uint16_t color)
{

    uint16_t posY = player.position.posY;
    uint16_t posX = player.position.posX;

    int y_centro = posY + SIDE_SQUARE_BOARD / 2;
    int r = y_centro;
    int l = y_centro;
    int i, j;
    for (i = 0; i < SIDE_SQUARE_BOARD; i++)
    {
        for (j = 0; j < SIDE_SQUARE_BOARD; j++)
        {
            if (j + posY >= l && j + posY <= r)
            {
                LCD_SetPoint(posX + j, posY + i, color);
            }
        }
        if (i % 2)
        {
            r++;
            l--;
        }
    }
}
uint16_t Draw_wall(int i, int j, int is_horizontal, int del)
{
    int vis_i = i, vis_j = j;

    /*in mY_board i muri stanno sulle colonne e righe pari*/
    int my_i = is_horizontal ? vis_j * 2 : (vis_j * 2) + 1;
    int my_j = is_horizontal ? (vis_i * 2) + 1 : vis_i * 2;
	
	int first_check=0;

    uint16_t x1W, y1W, lenght1, high1;
    uint16_t x2W, y2W, lenght2, high2;
    uint16_t x3W, y3W, lenght3, high3;
    uint16_t color = Green;

    /*dato il vertice destro del quadrato, il muro sarà disegnato sotto di esso*/
    if (is_horizontal)
    {
        high1 = 5;
        high2 = 5;
        high3 = 5;

        x1W = board.visual_board[vis_i][vis_j].posX;
        y1W = board.visual_board[vis_i][vis_j].posY - 5;

        x2W = x1W + SIDE_SQUARE_BOARD;
        y2W = y1W;

        x3W = x2W + 5;
        y3W = y2W;

        lenght1 = SIDE_SQUARE_BOARD;
        lenght2 = 5;
        lenght3 = SIDE_SQUARE_BOARD;
    }
    else
    {

        x1W = board.visual_board[vis_i][vis_j].posX - 5;
        x2W = x1W;
        x3W = x2W;

        y1W = board.visual_board[vis_i][vis_j].posY;
        y2W = y1W + SIDE_SQUARE_BOARD;
        y3W = y2W + 5;

        lenght1 = 5;
        lenght2 = 5;
        lenght3 = 5;

        high1 = SIDE_SQUARE_BOARD;
        high2 = 5;
        high3 = SIDE_SQUARE_BOARD;
    }

    /*il colore dipende da:
      -il muro è posizionato sopra un'altro muro (invalido -> rosso)
      -il muro costituisce una trappola(invalido -> rosso)
      - altrimenti è valido -> verde
      - è da cancellare -> nero
    */
       if (!del)
    {

        if (is_horizontal)
        {
            color = board.my_board[my_i][my_j] || (board.my_board[my_i][my_j + 1] && board.my_board[my_i - 1][my_j + 1] && board.my_board[my_i + 1][my_j + 1]) || board.my_board[my_i][my_j + 2] ? Red : Green;
            board.my_board[my_i][my_j]++;
            board.my_board[my_i][my_j + 1]++;
            board.my_board[my_i][my_j + 2]++;

            board.my_board[my_i][my_j + 3]++;
            board.my_board[my_i][my_j - 1]++;
        }
        else
        {
            
					  if((board.my_board[my_i + 1][my_j] && board.my_board[my_i + 1][my_j+1] && board.my_board[my_i + 1][my_j-1]) || (board.my_board[my_i][my_j] && board.my_board[my_i][my_j+1] && board.my_board[my_i][my_j-1] ) || (board.my_board[my_i+2][my_j] && board.my_board[my_i+2][my_j+1] && board.my_board[my_i +2][my_j-1] ) ){
						first_check =1;
						color = Red;
						}
						if(!((my_j-3)<= 0 || my_j+3 >= MY_C -1 ))
						if(first_check &&   board.my_board[my_i+1][my_j + 1] &&  board.my_board[my_i+1][my_j + 2] &&  board.my_board[my_i+1][my_j + 3] && board.my_board[my_i+1][my_j - 1] &&  board.my_board[my_i+1][my_j - 2] &&  board.my_board[my_i+1][my_j - 3] ){
						       first_check =0;
						       color = Green;
						}
						 if(! first_check){
						    if(board.my_board[my_i + 1][my_j] > 2 || board.my_board[my_i][my_j] > 2 || board.my_board[my_i+2][my_j]> 2)
									color = Red;
						 }
						
            board.my_board[my_i][my_j]+=3;
            board.my_board[my_i + 1][my_j]+=3;
            board.my_board[my_i + 2][my_j]+=3;

            board.my_board[my_i - 1][my_j]++;
            board.my_board[my_i + 3][my_j]++;
        }
        if (color != Red && (!is_valid(board.my_board, player1.my_i, player1.my_j) || !is_valid(board.my_board, player2.my_i, player2.my_j)))
        {
            color = Red;
        }
        GUI_rectangle(x1W, y1W, high1, lenght1, color);
        GUI_rectangle(x2W, y2W, high2, lenght2, color);
        GUI_rectangle(x3W, y3W, high3, lenght3, color);
    }
    else
    {

        if (is_horizontal)
        {

            board.my_board[my_i][my_j]--;
            board.my_board[my_i][my_j + 1]--;
            board.my_board[my_i][my_j + 2]--;

            board.my_board[my_i][my_j + 3]--;
            board.my_board[my_i][my_j - 1]--;

            if (!board.my_board[my_i][my_j])
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Black);
            }
            else
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Green);
            }
            if (!board.my_board[my_i][my_j + 1])
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Black);
            }
            else
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Green);
            }
            if (!board.my_board[my_i][my_j + 2])
            {
                GUI_rectangle(x3W, y3W, high3, lenght3, Black);
            }
            else
            {
                GUI_rectangle(x3W, y3W, high3, lenght3, Green);
            }
            color = board.my_board[my_i][my_j] || board.my_board[my_i][my_j + 1] || board.my_board[my_i][my_j + 2] ? Green : Black;
        }
        else
        {

            board.my_board[my_i + 3][my_j]--;
            board.my_board[my_i - 1][my_j]--;

            board.my_board[my_i][my_j]-=3;
            board.my_board[my_i + 1][my_j]-=3;
            board.my_board[my_i + 2][my_j]-=3;

            if (!board.my_board[my_i][my_j])
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Black);
            }
            else
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Green);
            }
            if (!board.my_board[my_i + 1][my_j])
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Black);
            }
            else
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Green);
            }
            if (!board.my_board[my_i + 2][my_j])
            {
                GUI_rectangle(x3W, y3W, high3, lenght3, Black);
            }
            else
            {
                GUI_rectangle(x3W, y3W, high3, lenght3, Green);
            }
            color = board.my_board[my_i][my_j] || board.my_board[my_i + 1][my_j] || board.my_board[my_i + 2][my_j] ? Green : Black;
        }
    }

    return color;
}
void move_player(struct Player *p, Direction direction, struct Player adversary)
{

    struct Player p1 = *p;

    int i = 0;
    for (i = 0; i < 8; i++)
    {
        if (p1.is_nm_valid[i])
        {
            GUI_Square(p1.next_moves[i].posX, p1.next_moves[i].posY, SIDE_SQUARE_BOARD, Blue2);
        }
    }

    GUI_Square(p1.position.posX, p1.position.posY, SIDE_SQUARE_BOARD, Blue2);

    switch (direction)
    {
    case (su):
        p1.my_i -= (p1.is_nm_valid[su] * 2);
        p1.visual_j -= p1.is_nm_valid[su];
        p1.position.posX = p1.next_moves[su].posX;
        p1.position.posY = p1.next_moves[su].posY;
        break;
    case (giu):
        p1.my_i += (p1.is_nm_valid[giu] * 2);
        p1.visual_j += p1.is_nm_valid[giu];
        p1.position.posX = p1.next_moves[giu].posX;
        p1.position.posY = p1.next_moves[giu].posY;
        break;
    case (destra):
        p1.my_j += (p1.is_nm_valid[destra] * 2);
        p1.visual_i += p1.is_nm_valid[destra];
        p1.position.posX = p1.next_moves[destra].posX;
        p1.position.posY = p1.next_moves[destra].posY;
        break;
    case (sinistra):
        p1.my_j -= (p1.is_nm_valid[sinistra] * 2);
        p1.visual_i -= p1.is_nm_valid[sinistra];
        p1.position.posX = p1.next_moves[sinistra].posX;
        p1.position.posY = p1.next_moves[sinistra].posY;
        break;
		case(su_destra):
			  p1.my_i -= (p1.is_nm_valid[su_destra] * 2);
        p1.visual_j -= p1.is_nm_valid[su_destra];
		    p1.my_j += (p1.is_nm_valid[su_destra] * 2);
        p1.visual_i += p1.is_nm_valid[su_destra];
		    p1.position.posX= p1.next_moves[su_destra].posX;
		    p1.position.posY = p1.next_moves[su_destra].posY;		
			break;
		
		case(su_sinistra):
			p1.my_i -= (p1.is_nm_valid[su_sinistra] * 2);
        p1.visual_j -= p1.is_nm_valid[su_sinistra];
		p1.my_j -= (p1.is_nm_valid[su_sinistra] * 2);
        p1.visual_i -= p1.is_nm_valid[su_sinistra];
		p1.position.posX = p1.next_moves[su_sinistra].posX;
		p1.position.posY= p1.next_moves[su_sinistra].posY;		
			break;
		
		case(giu_destra):
			 p1.my_i += (p1.is_nm_valid[giu_destra] * 2);
        p1.visual_j += p1.is_nm_valid[giu_destra];
		        p1.my_j += (p1.is_nm_valid[giu_destra] * 2);
        p1.visual_i += p1.is_nm_valid[giu_destra];
		p1.position.posX = p1.next_moves[giu_destra].posX;
		p1.position.posY = p1.next_moves[giu_destra].posY;
			break;
		
		case(giu_sinistra):
			p1.my_i += (p1.is_nm_valid[giu_sinistra] * 2);
        p1.visual_j += p1.is_nm_valid[giu_sinistra];
		 p1.my_j -= (p1.is_nm_valid[giu_sinistra] * 2);
        p1.visual_i -= p1.is_nm_valid[giu_sinistra];
		p1.position.posX=p1.next_moves[giu_sinistra].posX;
		p1.position.posY=p1.next_moves[giu_sinistra].posY;			
			break;
    }

    derive_next_moves(&p1, board, adversary);
    *p = p1;
}

uint16_t rotate_wall(struct Player *player)
{
    uint16_t output;

    struct Player p = *player;

    Draw_wall(p.walls[p.last_wall].i, p.walls[p.last_wall].j, p.walls[p.last_wall].horizontal, 1);
    p.walls[p.last_wall].horizontal = p.walls[p.last_wall].horizontal ? 0 : 1;
    output = Draw_wall(p.walls[p.last_wall].i, p.walls[p.last_wall].j, p.walls[p.last_wall].horizontal, 0);
    *player = p;
    return output;
}

void new_wall(struct Player *player)
{
    struct Player p = *player;
    p.last_wall++;
    p.tot_walls--;
    p.walls[p.last_wall].i = 3;
    p.walls[p.last_wall].j = 3;
    p.walls[p.last_wall].horizontal = 1;

    *player = p;
}

void remove_wall(struct Player *player)
{
    struct Player p = *player;
    Draw_wall(p.walls[p.last_wall].i, p.walls[p.last_wall].j, p.walls[p.last_wall].horizontal, 1);
    p.last_wall--;
    p.tot_walls++;
    *player = p;
}

uint16_t move_wall(struct Wall *wall, Direction d)
{

    struct Wall w = *wall;
    Draw_wall(w.i, w.j, w.horizontal, 1);

    switch (d)
    {
    case su:
        w.j--;
        break;

    case giu:
        w.j++;
        break;

    case destra:
        w.i++;
        break;

    case sinistra:
        w.i--;
        break;
		default:
			break;
    }

    *wall = w;
    return Draw_wall(w.i, w.j, w.horizontal, 0);
}

void init_player(uint8_t my_i, uint8_t my_j, uint8_t vis_i, uint8_t vis_j, struct Player *player, int id, uint16_t X_scoreboard, uint16_t Y_scoreboard, Direction direction)
{

    struct Player p = *player;
    int i;

    p.id = id;
    p.my_i = my_i;
    p.my_j = my_j;
    p.visual_i = vis_i;
    p.visual_j = vis_j;

    p.direction = direction;

    p.scoreboard.posX = X_scoreboard;
    p.scoreboard.posY = Y_scoreboard;

    p.tot_walls = N_WALLS;
    p.position.posX = board.visual_board[p.visual_i][p.visual_j].posX;
    p.position.posY = board.visual_board[p.visual_i][p.visual_j].posY;
    p.last_wall = -1;
    for (i = 0; i < N_WALLS; i++)
    {
        p.walls[i].horizontal = 1;
        p.walls[i].i = 3;
        p.walls[i].j = 3;
    }
    *player = p;
}

uint8_t GUI_select_next_move(struct Player p, Direction dir)
{
    uint8_t is_valid;
    if (p.is_nm_valid[dir])
    {
        GUI_Square(p.next_moves[dir].posX, p.next_moves[dir].posY, SIDE_SQUARE_BOARD, Green);
        is_valid = 1;
    }
    else
    {
        is_valid = 0;
    }
    return is_valid;
}

void save_move(int id, int is_wall, int is_horizontal, int y, int x)
{
    uint32_t move;
    if (id)
    {
        move += 0x00000000;
    }
    else
    {
        move += 0x01000000;
    }
    if (is_wall)
    {
        move += 0x0010000;
    }
    if (is_horizontal)
    {
        move += 0x00010000;
    }
    y = (uint32_t)y << 8;
    move += (uint32_t)y + (uint32_t)x;

    board.last_move++;
    board.moves[board.last_move] = move;
}

void update_scoreboard(struct Player p)
{

    char n_wall[2];
    sprintf(n_wall, "%2d", p.tot_walls);
    GUI_Text(p.scoreboard.posX, p.scoreboard.posY, (uint8_t *)n_wall, Blue, White);
    if (!p.tot_walls)
    {
        GUI_Text(p.scoreboard.posX - 75, p.scoreboard.posY + 20, (uint8_t *)"muri finiti", Blue, White);
    }
    else
    {
        
            GUI_Text(player1.scoreboard.posX - 75, player1.scoreboard.posY + 20, (uint8_t *)"muri finiti", Red, Red);       
            GUI_Text(player2.scoreboard.posX - 75, player2.scoreboard.posY + 20, (uint8_t *)"muri finiti", White, White);
        
    }
}

uint8_t have_won(struct Player p)
{

    uint8_t win = 0;
    char string_win[30];
    sprintf(string_win, "il giocatore %d ha vinto !!!", p.id);

    if (p.direction == destra && p.visual_i == C_BOARD - 1)
    {
        LCD_Clear(Black);
        GUI_Text(0, 280, (uint8_t *)string_win, Blue, White);
        win = 1;
    }
    if (p.direction == sinistra && p.visual_i == 0)
    {
        LCD_Clear(Black);
        GUI_Text(0, 280, (uint8_t *)string_win, Blue, White);
        win = 1;
    }
    return win;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
