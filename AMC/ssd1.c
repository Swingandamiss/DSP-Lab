/* ************************************************
 *Interfacing of 7 segment LED
 **************************************************/
#include <stdint.h>
#include "inc\tm4c123gh6pm.h"

void delayMs(int n);
void shift_out1(unsigned char str);
int main(void)
{
unsigned int i;
unsigned char a[16] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0xEE,0x3E,0x9C,0x7A,0x9E,0x8E};
	SYSCTL_RCGCGPIO_R = 0x10;
  GPIO_PORTE_DIR_R = 0x1F;
  GPIO_PORTE_DEN_R = 0x1F;

	while(1){
		for(i=0;i<=15;i++)	
	{
	    shift_out1(a[i]);
			shift_out1(a[i]);
			shift_out1(a[i]);
			shift_out1(a[i]);
			shift_out1(a[i]);

			delayMs(1000);
	}
}
}
void shift_out1(unsigned char str)
{
unsigned char j=0,check;

	for(j=0;j<=7;j++)
	{
		GPIO_PORTE_DATA_R = 0x00;	//PE3 pin(sclk) is low (0000 0000)
		check = (str &(1<<j));
		if(check)
			GPIO_PORTE_DATA_R =  0x04;	//PE2 pin(sdat) is high (0000 0100)
		 
else
				           GPIO_PORTE_DATA_R |=  0x00;
                   GPIO_PORTE_DATA_R |=  0x08;	//PE3 pin(sclk) is high (0000 1000)
		               GPIO_PORTE_DATA_R |=  0x10;
	}

}
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 4180; j++)
        {}  /* do nothing for 1 ms */
				}
