/****************************************Copyright (c)****************************************************
                             EXTRAPOINT 2 FRANCESCO GALLO 319989
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "Game.h"
#include "joystick/joystick.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "CAN/CAN.h"

struct Board board;
struct Player player1; 
struct Player player2;
struct Player playerAI;

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)

{
 
    SystemInit(); /* System Initialization (i.e., PLL)  */

    LCD_Initialization();

    init_RIT(RIT_INTERVAL);
    enable_RIT();

    joystick_init();
    BUTTON_init();

    LCD_Clear(White);
    GUI_Text(0, 280, (uint8_t *)" Press INT0 to start the game ", Blue, White);
	  init_timer(1, 0x17D7840);
	  CAN_Init();
	  

	

    LPC_SC->PCON |= 0x1; /* power-down	mode										*/
    LPC_SC->PCON &= ~(0x2);

    while (1)
    {
        __ASM("wfi");
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
