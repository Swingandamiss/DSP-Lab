#include <stdint.h>
#include <stdio.h>
#include "inc\tm4c123gh6pm.h"

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 4180; j++)
        {}  /* do nothing for 1 ms */
}


int main(void)
{
    /* enable clock to GPIOF/B/C at clock gating control register */
	
	     
      SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;//PF
	  	 
    /* enable the GPIO pins for the PF2/PF3/PB3/PC4 as output */
GPIO_PORTF_DIR_R = 0x0E;//PF2/3
	
    /* enable the GPIO pins for digital function */
    GPIO_PORTF_DEN_R = 0x0E;
		 
  while(1)
    {
       	
				GPIO_PORTF_DATA_R = 0x02;
        delayMs(1000);
				GPIO_PORTF_DATA_R = 0x04; 
        delayMs(1000);
        GPIO_PORTF_DATA_R = 0x08; 
        delayMs(1000);
				
					}
}

/* delay n milliseconds (16 MHz CPU clock) */


/* This function is called by the startup assembly code to perform system specific initialization tasks. */