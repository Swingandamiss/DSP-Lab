#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

#define LCD_clear()   LCD_command(0x01)
#define LCD_row1()    LCD_command(0x80)

void LCD_init(void);
void LCD_command(unsigned char cmd);
void LCD_putc(unsigned char c);
void LCD_puts(char *s);
void shift_out1(unsigned char data);
void LCD_scrollLeft(void);

unsigned char PP0 = 0, PP1 = 0;

int main(void)
{
    SYSCTL_RCGCGPIO_R |= 0x039;   // Enable clocks

    GPIO_PORTD_AFSEL_R |= 8;      
    GPIO_PORTD_DEN_R &= ~8;       
    GPIO_PORTD_AMSEL_R |= 8;      

    GPIO_PORTE_DIR_R |= 0x0C; 
    GPIO_PORTE_DEN_R |= 0x0C;

    GPIO_PORTA_DIR_R |= 0x20;
    GPIO_PORTA_DEN_R |= 0x20;

    GPIO_PORTF_DIR_R |= 0x01;
    GPIO_PORTF_DEN_R |= 0x01;

    LCD_init();
    LCD_clear();
    LCD_row1();
    LCD_puts("23ECB0F07");

    while (1) LCD_scrollLeft();  // keep scrolling
}

void LCD_puts(char *s){ while(*s) LCD_putc(*s++); }

void LCD_init(void){
    SysCtlDelay(160000);   // wait >15ms after Vcc rises

    // Safe init sequence (per HD44780 datasheet)
    LCD_command(0x30); SysCtlDelay(160000);
    LCD_command(0x30); SysCtlDelay(50000);
    LCD_command(0x30); SysCtlDelay(50000);
    LCD_command(0x20); SysCtlDelay(50000);

    // Now in 4-bit mode, send standard setup
    LCD_command(0x28);  // Function set: 4-bit, 2-line
    LCD_command(0x0C);  // Display ON, cursor off
    LCD_command(0x06);  // Entry mode set
    LCD_command(0x01);  // Clear display
    SysCtlDelay(2000);
}

void LCD_putc(unsigned char d){
    unsigned char n = ((d&0x11)<<3)|((d&0x22)<<1)|((d&0x44)>>1)|((d&0x88)>>3);
    PP1=(PP1&0xF0)|((n>>4)&0x0F)|0xA0; shift_out1(PP1); SysCtlDelay(500);
    PP1&=~0x20; shift_out1(PP1);
    PP1=(PP1&0xF0)|(n&0x0F)|0xA0; shift_out1(PP1); SysCtlDelay(500);
    PP1&=~0x20; shift_out1(PP1);
}

void LCD_command(unsigned char c){
    unsigned char n=((c&0x11)<<3)|((c&0x22)<<1)|((c&0x44)>>1)|((c&0x88)>>3);
    PP0=(PP0&0xF0)|((n>>4)&0x0F)|0x20; shift_out1(PP0); SysCtlDelay(500);
    PP0&=~0x20; shift_out1(PP0);
    PP0=(PP0&0xF0)|(n&0x0F)|0x20; shift_out1(PP0); SysCtlDelay(500);
    PP0&=~0x20; shift_out1(PP0);
}

void shift_out1(unsigned char d){
    for(int j=0;j<8;j++){
        GPIO_PORTE_DATA_R&=~0x08;
        if(d&(1<<j)) GPIO_PORTE_DATA_R|=0x04; else GPIO_PORTE_DATA_R&=~0x04;
        GPIO_PORTE_DATA_R|=0x08;
    }
    GPIO_PORTA_DATA_R|=0x20;
}

void LCD_scrollLeft(void){
    LCD_command(0x18);     // shift entire display left
    SysCtlDelay(2000000);  // adjust scroll speed
}