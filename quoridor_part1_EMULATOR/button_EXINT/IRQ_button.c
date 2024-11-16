#include "button.h"
#include "lpc17xx.h"

extern int down;
extern int wich_button;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1 << 20);     /* GPIO pin selection */
	down=1;
	wich_button = 0;
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4 &= ~(1 << 22);     /* GPIO pin selection */
	down=1;
	wich_button =1;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1 << 24);     /* GPIO pin selection */
	down=1;
	wich_button = 2;
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


