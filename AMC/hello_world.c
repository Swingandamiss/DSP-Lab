#include <stdint.h>
#include <stdbool.h>
#include "inc\tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

#define LCD_clear()   LCD_command(0x1)        /* Clear display LCD */
#define LCD_origin()  LCD_command(0x2)        /* Set to origin LCD */
#define LCD_row1()    LCD_command(0x80)        /* Begin at Line 1 */
#define LCD_row2()    LCD_command(0xC0)  /* Begin at Line 2 */
void shift_out1(unsigned char str);
void LCD_enable(void);
void LCD_command(unsigned char command);
void LCD_putc(unsigned char ascii);
void LCD_puts(unsigned char *lcd_string);
void LCD_init(void);

int main(void)
{


SYSCTL_RCGCGPIO_R |= 0x039;   /* enable clock to GPIOD/E/A/F */

   
    /* initialize PE3 for AIN4 input  */
    GPIO_PORTD_AFSEL_R |= 8;       /* enable alternate function */
    GPIO_PORTD_DEN_R &= ~8;        /* disable digital function */
    GPIO_PORTD_AMSEL_R |= 8;       /* enable analog function */
GPIO_PORTE_DIR_R |= 0x0C; /*   */
GPIO_PORTE_DEN_R |= 0x0C;
GPIO_PORTA_DIR_R |= 0x20;
GPIO_PORTA_DEN_R |= 0x20;
GPIO_PORTF_DIR_R |= 0x01;
GPIO_PORTF_DEN_R |= 0x01;
   
 LCD_init(); //LCD 16x2 initializing
	LCD_row1();
LCD_puts("     Hello ");
	LCD_row2();
	LCD_puts("     World ");
}

void LCD_puts(unsigned char *lcd_string)
{
      while (*lcd_string)
      {
              LCD_putc(*lcd_string++);
      }
}

void LCD_init()
{
SysCtlDelay(500000);
    LCD_command(0x30);
SysCtlDelay(500000);
    LCD_command(0x30);
SysCtlDelay(500000);
    LCD_command(0x30);
SysCtlDelay(500000);
    LCD_command(0x20);
SysCtlDelay(500000);
 LCD_command(0x20);
SysCtlDelay(500000);
    LCD_command(0x28); //Selecting 4bit LCD 16x2
SysCtlDelay(500000);
    LCD_command(0x0C); //LCD cursor off
SysCtlDelay(500000);
    LCD_command(0x06); //LCD display on
SysCtlDelay(500000);
    LCD_command(0x01); //LCD clear screen
SysCtlDelay(500000);
}

unsigned char PP1 = 0x00;
void LCD_putc(unsigned char ascii)
{
unsigned char volatile  num;
num = ascii;

// here reversing the bits of lcd_data
//e.g 00110011 will become 110110
num = ((num & 0x11)<<3)| ((num & 0x22)<<1)|((num & 0x44)>>1)|((num & 0x88)>>3);
ascii = num;
//Sending higher nibble of lcd data
    PP1 = (PP1 & 0xF0)|((ascii>>4) & 0x0F);
PP1 = PP1 | 0xA0; // setting 7th pin as high RS = 1, EN = 1;
PP1 = PP1 & ~(0x40); //RW = 0

//shifting data bits to shift register
shift_out1(PP1);
SysCtlDelay(500);
PP1 = PP1 & ~(0x20); // En = 0
shift_out1(PP1);
SysCtlDelay(500);
//Sending lower nibble of lcd data
    PP1 = (PP1 & 0xF0)|(ascii & 0x0F);
PP1 = PP1 | 0xA0; // setting 7th bit as high RS = 1, EN = 1;
PP1 = PP1 & ~(0x40); //RW = 0
shift_out1(PP1);
SysCtlDelay(500);
PP1 = PP1 & ~(0x20); // EN = 0
shift_out1(PP1);
SysCtlDelay(500);
}

void shift_out1(unsigned char str)
{
   unsigned char j=0,check;

GPIO_PORTA_DATA_R = 0x00;   //PA5 pin(stk) is low (0000 0000)
for(j=0;j<=7;j++)
{
     
GPIO_PORTE_DATA_R = 0x00;   //PE3 pin(sclk) is low (0000 0000)
check = (str &(1<<j));
if(check)

GPIO_PORTE_DATA_R |= 0x04;   //PE2 pin(sdat) is high (0000 0100)
else

GPIO_PORTE_DATA_R = 0x00;   //PE2 pin(sdat) is low (0000 0000)

GPIO_PORTE_DATA_R |= 0x08;   //PE3 pin(sclk) is high (0000 1000), sclk=1;
}

GPIO_PORTA_DATA_R |= 0x20;   //PA5 pin(stk) is high (0010 0000) , stk=1,storing completed;
}

unsigned char PP0 = 0x00;
void LCD_command(unsigned char command)
{
unsigned char volatile num;
num = command;
// here reversing the bits of lcd_command
//e.g 00110011 will become 11001100
num = ((num & 0x11)<<3)| ((num & 0x22)<<1)|((num & 0x44)>>1)|((num & 0x88)>>3);
command = num;
//Sending higher nibble of lcd data
    PP0 = (PP0 & 0xF0)|((command>>4) & 0x0F);//ok
PP0 = PP0 & (~(3 << 7)); //  setting 7th pin as low RS = 0 , RW = 0
PP0 = PP0 | 0x20; // EN = 1
//shifting bits to shift register
shift_out1(PP0);
SysCtlDelay(500);
PP0 = PP0 & ~(0x20); //EN = 0
shift_out1(PP0);
SysCtlDelay(500);
//Sending lower nibble of lcd command
    PP0 = (PP0 & 0xF0)|(command & 0x0F);
PP0 = PP0 & (~(3 << 7));   // RS = 0,RW = 0
PP0 = PP0 | 0x20; // EN = 1
//Sending lower nibble of lcd command
shift_out1(PP0);
SysCtlDelay(500);
PP0 = PP0 & ~(0x20); // EN = 0
shift_out1(PP0);
SysCtlDelay(500);
}