#include "GLCD/GLCD.h"
#include "Game.h"

int bug_AI = 0;

int check_wall_in_board(int coordinate);

/*funzioni AI*/
struct Wall place_all_wall(int old_mp_adv, int old_my_mp);
int place_wall(int i, int j, int is_horizontal, int old_adv_mp, int old_my_mp);
Direction move_all_direction(void);
void init_check_board(void);
int check_wall(int i, int j, int is_horizontal);

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

    board.who_play = 1; // SEMPRE IL PRIMO AD INIZIARE

    if (!is_npc)
    {
        init_player(7, 1, 0, 3, &player1, 1, 90, 255, destra);
        init_player(7, 13, 6, 3, &player2, 2, 200, 255, sinistra);

        derive_next_moves(&player1, board, player2);
        derive_next_moves(&player2, board, player1);

        GUI_Player(player1, Red);
        GUI_Player(player2, White);

        GUI_rectangle(10, 250, 50, 100, Red);
        GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);

        GUI_rectangle(120, 250, 50, 100, White);
        GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);

        update_scoreboard(player1);
        update_scoreboard(player2);

        board.who_play = 1;

        if (my_mp_id == board.who_play)
            GUI_next_moves(my_mp_id == 1 ? player1 : player2, Yellow);
    }
    else
    {
        if (is_mp && my_mp_id == 1)
        {
            init_player(7, 1, 0, 3, &playerAI, 1, 90, 255, destra);
            init_player(7, 13, 6, 3, &player2, 2, 200, 255, sinistra);
            derive_next_moves(&player2, board, playerAI);
            derive_next_moves(&playerAI, board, player2);

            GUI_Player(playerAI, Red);
            GUI_Player(player2, White);

            GUI_rectangle(10, 250, 50, 100, Red);
            GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);

            GUI_rectangle(120, 250, 50, 100, White);
            GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);

            update_scoreboard(playerAI);
            update_scoreboard(player2);

            AI_move();
        }

        else if (is_mp == 1 && my_mp_id == 2)
        {
            init_player(7, 13, 6, 3, &playerAI, 2, 200, 255, sinistra);
            init_player(7, 1, 0, 3, &player1, 1, 90, 255, destra);
            derive_next_moves(&player1, board, playerAI);
            derive_next_moves(&playerAI, board, player1);

            GUI_Player(playerAI, White);
            GUI_Player(player1, Red);

            GUI_rectangle(10, 250, 50, 100, Red);
            GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);

            GUI_rectangle(120, 250, 50, 100, White);
            GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);

            update_scoreboard(player1);
            update_scoreboard(playerAI);
        }

        else if (!is_mp && my_mp_id == 1)
        {

            init_player(7, 1, 0, 3, &player1, 1, 90, 255, destra);
            init_player(7, 13, 6, 3, &playerAI, 2, 200, 255, sinistra);
            derive_next_moves(&player1, board, playerAI);
            derive_next_moves(&playerAI, board, player1);
            GUI_Player(player1, Red);
            GUI_Player(playerAI, White);

            GUI_rectangle(10, 250, 50, 100, Red);
            GUI_Text(15, 255, (uint8_t *)" P1 wall: ", Blue, White);

            GUI_rectangle(120, 250, 50, 100, White);
            GUI_Text(125, 255, (uint8_t *)" P2 wall: ", Blue, White);

            update_scoreboard(player1);
            update_scoreboard(playerAI);

            GUI_next_moves(player1, Yellow);
        }
    }

    board.last_move = -1;
}

int is_valid(void)
{
    int checkp1 = -1;
    int checkp2 = -1;
    struct Player adv = playerAI.id == 1 ? player2 : player1;

    if (!is_npc)
        findPath(newPoint(player1.my_i, player1.my_j), newPoint(player2.my_i, player2.my_j), player1.direction, player2.direction, &checkp1, &checkp2);
    else
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &checkp1, &checkp2);

    return checkp1 == -1 || checkp2 == -1 ? 0 : 1;
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

    int i;

    for (i = 0; i < 8; i++)
    {
        p.is_nm_valid[i] = 0;
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
            }
            else
            {

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
            }
            else
            {

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
            }
            else
            {

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
            else
            {

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
    int vis_i = i % C_BOARD, vis_j = j % R_BOARD;

    /*in mY_board i muri stanno sulle colonne e righe pari*/
    int my_i = is_horizontal ? vis_j * 2 : (vis_j * 2) + 1;
    int my_j = is_horizontal ? (vis_i * 2) + 1 : vis_i * 2;

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

    if (!del)
    {

        if (is_horizontal)
        {

            if (check_wall_in_board(vis_i) || check_wall_in_board(vis_i + 1) || check_wall_in_board(vis_i + 2) || check_wall_in_board(vis_j))
            {
                color = Red;
            }
            color = board.my_board[my_i][my_j] || board.my_board[my_i][my_j + 1] || board.my_board[my_i][my_j + 2] ? Red : Green;

            board.my_board[my_i][my_j]++;
            board.my_board[my_i][my_j + 1]++;
            board.my_board[my_i][my_j + 2]++;
        }
        else
        {

            color = board.my_board[my_i][my_j] || board.my_board[my_i + 1][my_j] || board.my_board[my_i + 2][my_j] ? Red : Green;

            board.my_board[my_i][my_j]++;
            board.my_board[my_i + 1][my_j]++;
            board.my_board[my_i + 2][my_j]++;
        }

        init_check_board();
        if (color != Red && !is_valid())
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

            if (!board.my_board[my_i][my_j] || check_wall_in_board(vis_i) || check_wall_in_board(vis_j))
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Black);
            }
            else
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Green);
            }
            if (!board.my_board[my_i][my_j + 1] || check_wall_in_board(vis_i + 1) || check_wall_in_board(vis_j))
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Black);
            }
            else
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Green);
            }
            if (!board.my_board[my_i][my_j + 2] || check_wall_in_board(vis_i + 1) || check_wall_in_board(vis_j))
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

            board.my_board[my_i][my_j] -= 1;
            board.my_board[my_i + 1][my_j] -= 1;
            board.my_board[my_i + 2][my_j] -= 1;

            if (!board.my_board[my_i][my_j] || check_wall_in_board(vis_i) || check_wall_in_board(vis_j))
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Black);
            }
            else
            {
                GUI_rectangle(x1W, y1W, high1, lenght1, Green);
            }
            if (!board.my_board[my_i + 1][my_j] || check_wall_in_board(vis_i) || check_wall_in_board(vis_j + 1))
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Black);
            }
            else
            {
                GUI_rectangle(x2W, y2W, high2, lenght2, Green);
            }
            if (!board.my_board[my_i + 2][my_j] || check_wall_in_board(vis_i) || check_wall_in_board(vis_j + 1))
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
int check_wall_in_board(int coordinate)
{
    int c = coordinate;
    return c <= 0 || c >= R_BOARD;
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
    case (su_destra):
        p1.my_i -= (p1.is_nm_valid[su_destra] * 2);
        p1.visual_j -= p1.is_nm_valid[su_destra];
        p1.my_j += (p1.is_nm_valid[su_destra] * 2);
        p1.visual_i += p1.is_nm_valid[su_destra];
        p1.position.posX = p1.next_moves[su_destra].posX;
        p1.position.posY = p1.next_moves[su_destra].posY;
        break;

    case (su_sinistra):
        p1.my_i -= (p1.is_nm_valid[su_sinistra] * 2);
        p1.visual_j -= p1.is_nm_valid[su_sinistra];
        p1.my_j -= (p1.is_nm_valid[su_sinistra] * 2);
        p1.visual_i -= p1.is_nm_valid[su_sinistra];
        p1.position.posX = p1.next_moves[su_sinistra].posX;
        p1.position.posY = p1.next_moves[su_sinistra].posY;
        break;

    case (giu_destra):
        p1.my_i += (p1.is_nm_valid[giu_destra] * 2);
        p1.visual_j += p1.is_nm_valid[giu_destra];
        p1.my_j += (p1.is_nm_valid[giu_destra] * 2);
        p1.visual_i += p1.is_nm_valid[giu_destra];
        p1.position.posX = p1.next_moves[giu_destra].posX;
        p1.position.posY = p1.next_moves[giu_destra].posY;
        break;

    case (giu_sinistra):
        p1.my_i += (p1.is_nm_valid[giu_sinistra] * 2);
        p1.visual_j += p1.is_nm_valid[giu_sinistra];
        p1.my_j -= (p1.is_nm_valid[giu_sinistra] * 2);
        p1.visual_i -= p1.is_nm_valid[giu_sinistra];
        p1.position.posX = p1.next_moves[giu_sinistra].posX;
        p1.position.posY = p1.next_moves[giu_sinistra].posY;
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
    uint32_t move = 0;

    if (!id)
    {
        move += 0x00000000;
    }
    else
    {
        move += 0x01000000;
    }
    if (is_wall)
    {
        move += 0x00100000;
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

    uint16_t color = p.id == 1 ? Red : White;
    char n_wall[2];
    sprintf(n_wall, "%2d", p.tot_walls);
    GUI_Text(p.scoreboard.posX, p.scoreboard.posY, (uint8_t *)n_wall, Blue, White);
    if (!p.tot_walls)
    {
        GUI_Text(p.scoreboard.posX - 75, p.scoreboard.posY + 20, (uint8_t *)"muri finiti", Blue, White);
    }
    else
    {
        GUI_Text(p.scoreboard.posX - 75, p.scoreboard.posY + 20, (uint8_t *)"muri finiti", color, color);
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

void unwrap_move(uint32_t move)
{

    uint8_t id = (uint8_t)(move >> 24) + 1;
    uint8_t is_wall = (uint8_t)((move >> 20) & (0x0000000F));
    uint8_t is_horizontal = (uint8_t)((move >> 16) & (0x000000F));
    uint8_t j = (uint8_t)((move >> 8) & (0x000000FF));
    uint8_t i = (uint8_t)(move & (0x000000FF));
    struct Player p1 = id == 1 ? player1 : player2;

    if (id && is_wall == 0x0F && is_horizontal == 0x0F && j == 0xFF && i == 0xFF)
    {

        char string_win[30];
        sprintf(string_win, "il giocatore %d ha vinto !!!", id);

        won = 1;

        LCD_Clear(Black);
        GUI_Text(0, 280, (uint8_t *)string_win, Blue, White);
        disable_timer(1);
        return;
    }

    if (!is_wall && is_horizontal)
    { // nessuna mossa è stata effettuata
        if (id == 1)
        {
            board.who_play = 2;
            if (!is_npc)
            {
                derive_next_moves(&player2, board, player1);
                GUI_next_moves(player2, Yellow);
            }
            is_move = 1;
            new_wall_color = 0;
            have_wall = 0;
            update_scoreboard(player1);
        }
        else
        {
            board.who_play = 1;
            if (!is_npc)
            {
                derive_next_moves(&player1, board, player2);
                GUI_next_moves(player1, Yellow);
            }
            is_move = 1;
            new_wall_color = 0;
            have_wall = 0;
            update_scoreboard(player2);
        }

        count = SECONDS_PER_MOVE;
        GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
        is_next_move_valid = 0;

        count = 20;
        init_timer(1, 0x17D7840);
        enable_timer(1);

        return;
    }

    if (p1.visual_i == i && p1.visual_j == j && !is_wall)
    {
        bug_AI = 1;
        return;
    }

    // effettuo la nuova mossa (la mossa se effettuata è valida)
    if (!is_wall)
    {
        GUI_Square(p1.position.posX, p1.position.posY, SIDE_SQUARE_BOARD, Blue2);
        p1.visual_i = i;
        p1.visual_j = j;
        p1.position.posX = board.visual_board[i][j].posX;
        p1.position.posY = board.visual_board[i][j].posY;
        p1.my_i = (j * 2) + 1;
        p1.my_j = (i * 2) + 1;
    }
    else
    {
        Draw_wall(i, j, is_horizontal, 0);
        p1.last_wall++;
        p1.tot_walls--;
        p1.walls[p1.last_wall].i = i;
        p1.walls[p1.last_wall].j = j;
        p1.walls[p1.last_wall].horizontal = is_horizontal;
    }

    if (p1.id == 1)
    {

        if (!is_wall)
        {
            player1.visual_i = p1.visual_i;
            player1.visual_j = p1.visual_j;
            player1.my_i = p1.my_i;
            player1.my_j = p1.my_j;
            player1.position.posX = p1.position.posX;
            player1.position.posY = p1.position.posY;
        }
        else
        {
            player1.last_wall = p1.last_wall;
            player1.tot_walls = p1.tot_walls;
            player1.walls[player1.last_wall].i = p1.walls[p1.last_wall].i;
            player1.walls[player1.last_wall].j = p1.walls[p1.last_wall].j;
            player1.walls[player1.last_wall].horizontal = is_horizontal;
        }
    }
    else
    {
        if (!is_wall)
        {
            player2.visual_i = p1.visual_i;
            player2.visual_j = p1.visual_j;
            player2.my_i = p1.my_i;
            player2.my_j = p1.my_j;
            player2.position.posX = p1.position.posX;
            player2.position.posY = p1.position.posY;
        }
        else
        {
            player2.last_wall = p1.last_wall;
            player2.tot_walls = p1.tot_walls;
            player2.walls[player2.last_wall].i = p1.walls[p1.last_wall].i;
            player2.walls[player2.last_wall].j = p1.walls[p1.last_wall].j;
            player2.walls[player2.last_wall].horizontal = is_horizontal;
        }
    }

    if (p1.id == 1)
    {
        GUI_Player(p1, Red);
    }
    else
    {
        GUI_Player(p1, White);
    }

    derive_next_moves(p1.id == 1 ? &player2 : &player1, board, p1.id == 1 ? player1 : player2);
    board.who_play = p1.id == 1 ? 2 : 1;
    GUI_next_moves(p1.id == 1 ? player2 : player1, Yellow);
    count = SECONDS_PER_MOVE;
    GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
    is_next_move_valid = 0;
    is_move = 1;
    new_wall_color = 0;
    have_wall = 0;
    update_scoreboard(p1.id == 1 ? player1 : player2);

    count = 20;
    
    enable_timer(1);
}

void GUI_which_choice(uint16_t X, uint16_t Y)
{

    uint16_t posY = Y;
    uint16_t posX = X;

    int r = SIDE_SQUARE_BOARD + posY;
    int l = 0 + posY;
    int i, j;
    for (i = 0; i < SIDE_SQUARE_BOARD; i++)
    {
        for (j = 0; j < SIDE_SQUARE_BOARD; j++)
        {
            if (j + posY >= l && j + posY <= r)
            {
                LCD_SetPoint(posX + i, posY + j, Black);
            }
        }
        if (i % 2)
        {
            r--;
            l++;
        }
    }
}

void enqueue(Queue *q, Point p)
{

    q->array[q->tail] = p;
    q->tail++;
    return;
}

Point dequeue(Queue *q)
{
    return q->array[q->head++];
}

uint8_t isEmpty(Queue *q)
{
    return q->tail == q->head;
}

Point newPoint(int i, int j)
{
    Point p;
    p.i = i;
    p.j = j;
    return p;
}

Queue newQueue(void)
{
    Queue q;
    int i;
    for (i = 0; i < MY_C * MY_R; i++)
    {
        q.array[i].i = 0;
        q.array[i].j = 0;
    }
    q.head = 0;
    q.tail = 0;
    return q;
};

uint8_t isValid(int visitated[MY_R][MY_C], Point p)
{
    return p.i >= 1 && p.j >= 1 && p.i < MY_R - 1 && p.j < MY_C - 1 && !board.check_board[p.i][p.j] && !visitated[p.i][p.j];
}

void findPath(Point start_player1, Point start_player2, Direction dir_p1, Direction dir_p2, int *min_path_p1, int *min_path_p2)
{
    /*
     * LA  funzione trova il numero del cammino minimo di entrambi i giocatori
     * per vincere la partita
     */
    Point start = start_player1;
    Direction d = dir_p1;
    uint8_t path_find_p1 = 0;
    uint8_t path_find_p2 = 0;
    int visited[MY_R][MY_C];
    int passi;
    Queue q = newQueue();
    Point current;
    Point nexts[4];
    int i, j;

    for (i = 0; i < MY_R; i++)
    {
        for (j = 0; j < MY_C; j++)
        {
            visited[i][j] = 0;
        }
    }

    /* INIZIO RICERCA PLAYER 1*/
    visited[start.i][start.j] = 1;
    enqueue(&q, start);

    while (!isEmpty(&q))
    {

        current = dequeue(&q);
        passi = visited[current.i][current.j];

        if (d == destra)
        {
            if (current.j >= MY_C - 2)
            {
                *min_path_p1 = passi - 1;
                path_find_p1 = 1;
                break;
            }
        }
        else
        {
            if (current.j <= 1)
            {
                *min_path_p1 = passi - 1;
                path_find_p1 = 1;
                break;
            }
        }

        nexts[0] = newPoint(current.i - 1, current.j);
        nexts[1] = newPoint(current.i, current.j - 1);
        nexts[2] = newPoint(current.i, current.j + 1);
        nexts[3] = newPoint(current.i + 1, current.j);

        for (i = 0; i < 4; i++)
        {
            if (isValid(visited, nexts[i]))
            {
                enqueue(&q, nexts[i]);
                visited[nexts[i].i][nexts[i].j] = passi + 1;
            }
        }
    }

    if (!path_find_p1)
    {
        *min_path_p1 = -1;
    }

    /*INIZIO RICERCA PLAYER 2*/
    start = start_player2;
    passi = 0;
    d = dir_p2;
    q = newQueue();

    for (i = 0; i < MY_R; i++)
    {
        for (j = 0; j < MY_C; j++)
        {
            visited[i][j] = 0;
        }
    }

    visited[start.i][start.j] = 1;
    enqueue(&q, start);

    while (!isEmpty(&q))
    {
        current = dequeue(&q);
        passi = visited[current.i][current.j];

        if (d == destra)
        {
            if (current.j >= MY_C - 2)
            {
                *min_path_p2 = passi - 1;
                path_find_p2 = 1;
                break;
            }
        }
        else
        {
            if (current.j <= 1)
            {
                *min_path_p2 = passi - 1;
                path_find_p2 = 1;
                break;
            }
        }

        nexts[0] = newPoint(current.i - 1, current.j);
        nexts[1] = newPoint(current.i, current.j - 1);
        nexts[2] = newPoint(current.i, current.j + 1);
        nexts[3] = newPoint(current.i + 1, current.j);

        for (i = 0; i < 4; i++)
        {
            if (isValid(visited, nexts[i]))
            {
                enqueue(&q, nexts[i]);
                visited[nexts[i].i][nexts[i].j] = passi + 1;
            }
        }
    }

    if (!path_find_p2)
    {
        *min_path_p2 = -1;
    }
    return;
}

int is_AI_passed(int i_playerAdv, int i_playerAI, Direction dir_playerAI)
{
    if (dir_playerAI == sinistra)
    {
        if (i_playerAdv > i_playerAI)
            return 1;
        else
            return 0;
    }
    else
    {
        if (i_playerAI > i_playerAdv)
            return 1;
        else
            return 0;
    }
}

/*FUNZIONI AI*/
void AI_move(void)
{

    int adv_mp;
    int my_mp;
    struct Player adv = playerAI.id == 1 ? player2 : player1;
    uint16_t AI_color = playerAI.id == 1 ? Red : White;

    struct Wall nextWall;

    if (bug_AI)
    {
        bug_AI = 0;
        return;
    }

    if (playerAI.id != board.who_play)
        return;

    derive_next_moves(&playerAI, board, adv);
    init_check_board();

    /*trova il cammino minimo tra i due giocatori*/
    findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &adv_mp, &my_mp);

    if (my_mp > adv_mp && playerAI.tot_walls != 0 && (adv.visual_j != playerAI.visual_j || is_AI_passed(adv.visual_i, playerAI.visual_i, playerAI.direction)))
    {

        nextWall = place_all_wall(adv_mp, my_mp);
        if ((nextWall.i == 0 && nextWall.horizontal == 0) || (nextWall.horizontal == 1 && nextWall.j == 0) || !check_wall(nextWall.i, nextWall.j, nextWall.horizontal))
        {
            move_player(&playerAI, move_all_direction(), adv);
            GUI_Player(playerAI, AI_color);

            if (!(won = have_won(playerAI)))
            {

                is_move = 1;
                is_next_move_valid = 0;
                count = SECONDS_PER_MOVE;
                GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
                save_move(playerAI.id - 1, 0, 0, playerAI.visual_j, playerAI.visual_i);

                if (is_mp)
                {
                    send_message(board.moves[board.last_move]);
                }
                else
                {
                    // necessario per la prossima mossa
                    derive_next_moves(adv.id == 1 ? &player1 : &player2, board, playerAI);
                    GUI_next_moves(adv.id == 1 ? player1 : player2, Yellow);
                }
            }
            else
            {

                if (is_mp)
                {
                    uint32_t msg_won = 0;
                    msg_won = ((playerAI.id - 1) << 24) + msg_won;
                    msg_won += 0xFFFFFF;
                    send_message(msg_won);
                }

                count = 1;
            }
        }
        else
        {

            Draw_wall(nextWall.i, nextWall.j, nextWall.horizontal, 0);

            playerAI.tot_walls--;
            update_scoreboard(playerAI);

            is_move = 1;
            new_wall_color = 0;
            have_wall = 0;
            count = SECONDS_PER_MOVE;
            GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
            save_move(playerAI.id - 1, 1, nextWall.horizontal, nextWall.j, nextWall.i);
            if (is_mp)
                send_message(board.moves[board.last_move]);
            else
            {
                // necessario per la prossima mossa
                derive_next_moves(adv.id == 1 ? &player1 : &player2, board, playerAI);
                GUI_next_moves(adv.id == 1 ? player1 : player2, Yellow);
            }
        }

        board.who_play = adv.id;
    }

    else
    {

        move_player(&playerAI, move_all_direction(), adv);
        GUI_Player(playerAI, AI_color);

        if (!(won = have_won(playerAI)))
        {

            is_move = 1;
            is_next_move_valid = 0;
            count = SECONDS_PER_MOVE;
            GUI_Text(88, 225, (uint8_t *)" TIMER 20", Blue, White);
            save_move(playerAI.id - 1, 0, 0, playerAI.visual_j, playerAI.visual_i);

            if (is_mp)
            {
                send_message(board.moves[board.last_move]);
            }
            else
            {
                // necessario per la prossima mossa
                derive_next_moves(adv.id == 1 ? &player1 : &player2, board, playerAI);
                GUI_next_moves(adv.id == 1 ? player1 : player2, Yellow);
            }
        }
        else
        {

            if (is_mp)
            {
                uint32_t msg_won = 0;
                msg_won = ((playerAI.id - 1) << 24) + msg_won;
                msg_won += 0xFFFFFF;
                send_message(msg_won);
            }

            count = 1;
        }
    }

    board.who_play = adv.id;
    if (is_mp)
        disable_timer(1);
}

struct Wall place_all_wall(int old_mp_adv, int old_my_mp)
{

    int i, j;
    int max = old_mp_adv - old_my_mp, maxi, maxj, maxIsHorizontal;
    struct Wall wall_out;
    int temp = 0;

    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 7; j++)
        {
            temp = place_wall(i, j, 0, old_mp_adv, old_my_mp);
            if (max < temp)
            {
                max = temp;
                maxi = i;
                maxj = j;
                maxIsHorizontal = 0;
            }

            temp = place_wall(i, j, 1, old_mp_adv, old_my_mp);
            if (max < temp)
            {
                max = temp;
                maxi = i;
                maxj = j;
                maxIsHorizontal = 1;
            }
        }
    }

    wall_out.horizontal = maxIsHorizontal;
    wall_out.i = maxi;
    wall_out.j = maxj;

    return wall_out; // ritorna il muro che mette più in difficoltà l'avversario, ma non se stesso
}

int place_wall(int i, int j, int is_horizontal, int old_adv_mp, int old_my_mp)
{

    struct Player adv = playerAI.id == 1 ? player2 : player1;
    int my_i = is_horizontal ? j * 2 : (j * 2) + 1;
    int my_j = is_horizontal ? (i * 2) + 1 : i * 2;
    int mp_adv;
    int my_mp;

    init_check_board();

    if (is_horizontal)
    {

        if (board.check_board[my_i][my_j] || board.check_board[my_i][my_j + 1] || board.check_board[my_i][my_j + 2])
            return old_adv_mp - old_my_mp;

        board.check_board[my_i][my_j]++;
        board.check_board[my_i][my_j + 1]++;
        board.check_board[my_i][my_j + 2]++;

        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &mp_adv, &my_mp);

        init_check_board();

        if (mp_adv == -1 || my_mp == -1)
            return old_adv_mp - old_my_mp;
        if (mp_adv < old_adv_mp) // sto facilitando l'avversario
            return old_adv_mp - old_my_mp;

        return mp_adv - my_mp;
    }
    else
    {
        if (board.check_board[my_i][my_j] || board.check_board[my_i + 1][my_j] || board.check_board[my_i + 2][my_j])
            return old_adv_mp - old_my_mp;

        board.check_board[my_i][my_j]++;
        board.check_board[my_i + 1][my_j]++;
        board.check_board[my_i + 2][my_j]++;

        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &mp_adv, &my_mp);

        init_check_board();

        if (mp_adv == -1 || my_mp == -1)
            return old_adv_mp - old_my_mp;
        if (mp_adv < old_adv_mp)
            return old_adv_mp - old_my_mp;

        return mp_adv - my_mp;
    }
}

Direction move_all_direction(void)
{

    int i;
    struct Player adv = playerAI.id == 1 ? player2 : player1;
    int su_mp, giu_mp, destra_mp, sinistra_mp, su_destra_mp, su_sinistra_mp, giu_destra_mp, giu_sinistra_mp;
    int array[8];
    int min_mp = INT_MAX;
    int adv_mp;
    Direction dir_out;

    init_check_board();

    for (i = 0; i < 8; i++)
    {
        array[i] = INT_MAX;
    }

    if (playerAI.is_nm_valid[su])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i - (playerAI.is_nm_valid[su] * 2), playerAI.my_j), adv.direction, playerAI.direction, &adv_mp, &su_mp);
        array[su] = su_mp;
    }

    if (playerAI.is_nm_valid[giu])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i + (playerAI.is_nm_valid[giu] * 2), playerAI.my_j), adv.direction, playerAI.direction, &adv_mp, &giu_mp);
        array[giu] = giu_mp;
    }

    if (playerAI.is_nm_valid[destra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j + (playerAI.is_nm_valid[destra] * 2)), adv.direction, playerAI.direction, &adv_mp, &destra_mp);
        array[destra] = destra_mp;
    }

    if (playerAI.is_nm_valid[sinistra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j - (playerAI.is_nm_valid[sinistra] * 2)), adv.direction, playerAI.direction, &adv_mp, &sinistra_mp);
        array[sinistra] = sinistra_mp;
    }

    if (playerAI.is_nm_valid[su_destra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i - (playerAI.is_nm_valid[su_destra] * 2), playerAI.my_j + (playerAI.is_nm_valid[su_destra] * 2)), adv.direction, playerAI.direction, &adv_mp, &su_destra_mp);
        array[su_destra] = su_destra_mp;
    }

    if (playerAI.is_nm_valid[su_sinistra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i - (playerAI.is_nm_valid[su_sinistra] * 2), playerAI.my_j - (playerAI.is_nm_valid[su_sinistra] * 2)), adv.direction, playerAI.direction, &adv_mp, &su_sinistra_mp);
        array[su_sinistra] = su_sinistra_mp;
    }

    if (playerAI.is_nm_valid[giu_destra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i + (playerAI.is_nm_valid[giu_destra] * 2), playerAI.my_j + (playerAI.is_nm_valid[giu_destra] * 2)), adv.direction, playerAI.direction, &adv_mp, &giu_destra_mp);
        array[giu_destra] = giu_destra_mp;
    }

    if (playerAI.is_nm_valid[giu_sinistra])
    {
        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i + (playerAI.is_nm_valid[giu_sinistra] * 2), playerAI.my_j - (playerAI.is_nm_valid[giu_sinistra] * 2)), adv.direction, playerAI.direction, &adv_mp, &giu_sinistra_mp);
        array[giu_sinistra] = giu_sinistra_mp;
    }

    for (i = 0; i < 8; i++)
    {
        if (min_mp > array[i])
        {
            min_mp = array[i];
            dir_out = (Direction)i;
        }
    }

    return dir_out;
}
void fill_wall(void)
{

    int i;
    int j;
    for (i = 0; i < MY_R; i++)
    {

        for (j = 0; j < MY_C; j++)
        {

            if ((i > 0 && i < MY_R - 1) &&
                (i + 1 > 0 && i + 1 < MY_R - 1) &&
                (i + 2 > 0 && i + 2 < MY_R - 1) &&
                (i + 3 > 0 && i + 3 < MY_R - 1) &&
                (i - 1 > 0 && i - 1 < MY_R - 1) &&
                (i - 2 > 0 && i - 2 < MY_R - 1) &&
                (i - 3 > 0 && i - 3 < MY_R - 1))
            {

                if (board.check_board[i][j] == 0 && board.check_board[i + 1][j] && board.check_board[i - 1][j] && board.check_board[i + 2][j] && board.check_board[i - 2][j] && board.check_board[i + 3][j] && board.check_board[i - 3][j])

                {
                    board.check_board[i][j] = 1;
                }
            }

            if ((j > 0 && j < MY_C - 1) &&
                (j + 1 > 0 && j + 1 < MY_C - 1) &&
                (j + 2 > 0 && j + 2 < MY_C - 1) &&
                (j + 3 > 0 && j + 3 < MY_C - 1) &&
                (j - 1 > 0 && j - 1 < MY_C - 1) &&
                (j - 2 > 0 && j - 2 < MY_C - 1) &&
                (j - 3 > 0 && j - 3 < MY_C - 1))
            {

                if (board.check_board[i][j] == 0 && board.check_board[i][j + 1] && board.check_board[i][j - 1] && board.check_board[i][j + 2] && board.check_board[i][j - 2] && board.check_board[i][j + 3] && board.check_board[i][j - 3])
                {

                    board.check_board[i][j] = 1;
                }
            }
        }
    }
}
void init_check_board(void)
{
    int i, j;
    for (i = 0; i < MY_R; i++)
    {
        for (j = 0; j < MY_C; j++)
        {
            board.check_board[i][j] = board.my_board[i][j];
        }
    }
    fill_wall();
}

uint32_t convert_move2general(uint32_t move)
{
    uint8_t j = (uint8_t)((move >> 8) & (0x000000FF));
    uint8_t i = (uint8_t)(move & (0x000000FF));
    uint8_t x, y;
    uint8_t is_wall = (uint8_t)((move >> 20) & (0x0000000F));
    uint8_t hor = (uint8_t)(move >> 16) & 0xF;

    x = j;
    y = i;

    hor = hor == 0 ? 1 : 0;

    if (is_wall)
        y--;

    move = (move & (0xFFFF0000)) + (y << 8) + x;
    return move;
}

uint32_t convert_general2move(uint32_t move)
{

    uint8_t y = (uint8_t)((move >> 8) & (0x000000FF));
    uint8_t x = (uint8_t)(move & (0x000000FF));
    uint8_t i, j;
    uint8_t is_wall = (uint8_t)((move >> 20) & (0x0000000F));
    uint8_t hor = (uint8_t)(move >> 16) & 0xF;

    hor = hor == 0 ? 1 : 0;

    j = x;
    i = y;

    if (is_wall)
        i++;

    move = (move & (0xFFFF0000)) + (j << 8) + i;
    return move;
}
int check_wall(int i, int j, int is_horizontal)
{
    int my_i = is_horizontal ? j * 2 : (j * 2) + 1;
    int my_j = is_horizontal ? (i * 2) + 1 : i * 2;
    int checkAdv = 0, checkAI = 0;
    struct Player adv = playerAI.id == 1 ? player2 : player1;

    init_check_board();

    if (!is_horizontal && (board.check_board[my_i][my_j] || board.check_board[my_i + 1][my_j] || board.check_board[my_i + 2][my_j]))
        return 0;
    if(!is_horizontal)
    {
        board.my_board[my_i][my_j]++;
        board.my_board[my_i + 1][my_j]++;
        board.my_board[my_i + 2][my_j]++;
        init_check_board();

        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &checkAdv, &checkAI);

        board.my_board[my_i][my_j]--;
        board.my_board[my_i + 1][my_j]--;
        board.my_board[my_i + 2][my_j]--;
        init_check_board();
    }
    if (is_horizontal && (board.check_board[my_i][my_j] || board.check_board[my_i][my_j + 1] || board.check_board[my_i][my_j + 2]))
        return 0;
    if(is_horizontal)
    {
        board.my_board[my_i][my_j]++;
        board.my_board[my_i][my_j + 1]++;
        board.my_board[my_i][my_j + 2]++;
        init_check_board();

        findPath(newPoint(adv.my_i, adv.my_j), newPoint(playerAI.my_i, playerAI.my_j), adv.direction, playerAI.direction, &checkAdv, &checkAI);

        board.my_board[my_i][my_j]--;
        board.my_board[my_i][my_j + 1]--;
        board.my_board[my_i][my_j + 2]--;
        init_check_board();
    }

    return checkAdv != -1 && checkAI != -1;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
