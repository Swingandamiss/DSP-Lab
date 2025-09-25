#include <stdint.h>
#include "C:\Users\hp\Desktop\es lab\ti\TivaWare_C_Series-2.2.0.295\inc\tm4c123gh6pm.h"

void delayMs(int n);

int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {}

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x01;

    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DIR_R &= ~0x11;

    GPIO_PORTF_DEN_R |= 0x1F;
    GPIO_PORTF_PUR_R |= 0x11;

    while(1)
    {
        uint8_t sw1 = (GPIO_PORTF_DATA_R & 0x10) == 0;
        uint8_t sw2 = (GPIO_PORTF_DATA_R & 0x01) == 0;
        uint8_t ledData = 0;

        if(sw1 && sw2)
            ledData = 0x0E;
        else if(sw1)
            ledData = 0x08;
        else if(sw2)
            ledData = 0x04;
        else
            ledData = 0x02;

        GPIO_PORTF_DATA_R = ledData;
        delayMs(100);
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < 4180; j++) {}
}


seond code switches