Using timer module to implement delay 

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define CLOCK_CYCLES_PER_MS 16000

void Timer0_DelayMs_Init(void) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
    volatile uint32_t delay = SYSCTL_RCGCTIMER_R;
    (void)delay;
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
    TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER;
    TIMER0_TAMR_R = TIMER_TAMR_TAMR_1_SHOT;
    TIMER0_TAMR_R &= ~TIMER_TAMR_TACDIR;
    TIMER0_TAILR_R = CLOCK_CYCLES_PER_MS - 1;
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;
}

void delayMs(int n) {
    int i;
    for(i = 0; i < n; i++) {
        TIMER0_TAILR_R = CLOCK_CYCLES_PER_MS - 1;
        TIMER0_ICR_R = TIMER_ICR_TATOCINT;
        TIMER0_CTL_R |= TIMER_CTL_TAEN;
        while ((TIMER0_RIS_R & TIMER_RIS_TATORIS) == 0) {
            // Wait
        }
    }
}

int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x0E;

    Timer0_DelayMs_Init();

    while(1)
    {
        GPIO_PORTF_DATA_R = 0x02;
        delayMs(250);

        GPIO_PORTF_DATA_R = 0x04;
        delayMs(250);

        GPIO_PORTF_DATA_R = 0x08;
        delayMs(250);
    }
}

Bluetooth interfacing


#include "inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
void Delay(unsigned long counter); // used to add delay
void HC05_init(void); // Initialize UART5 module for HC-05
char Bluetooth_Read(void); //Read data from Rx5 pin of TM4C123
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin
void Bluetooth_Write_String(char *str); // Transmit a string to HC-05 over Tx5 pin
int main(void)
{
 
HC05_init(); // call HC05_init() to initialze UART5 of TM4C123GH6PM

/* Set PF1, PF2 and PF3 as digital output pins */

SYSCTL_RCGCGPIO_R |= 0x20;   /* enable clock to GPIOF */
    GPIO_PORTF_DIR_R |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
    GPIO_PORTF_DEN_R |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
  Delay(10);

while(1)
{
char c = Bluetooth_Read();          /* get a character from UART5 */

/* Check the received character and take action to control onboard LEDs of TM4C123 */
/* Send status string to Andriod app after turning on/off LEDs */

        if( c=='A'){
GPIO_PORTF_DATA_R |=(1<<1);
Bluetooth_Write_String("RED LED ON\n");
}
else if( c=='B'){
GPIO_PORTF_DATA_R &=~(1<<1);
Bluetooth_Write_String("RED LED OFF\n");
}
else if( c=='C'){
GPIO_PORTF_DATA_R |=(1<<2);
Bluetooth_Write_String("BLUE LED ON\n");
}
else if( c=='D'){
GPIO_PORTF_DATA_R &=~(1<<2);
Bluetooth_Write_String("BLUE LED OFF\n");
}
if( c=='E'){
GPIO_PORTF_DATA_R |=(1<<3);
Bluetooth_Write_String("GREEN LED ON\n");
}
else if( c=='F'){
GPIO_PORTF_DATA_R &=~(1<<3);
Bluetooth_Write_String("GREEN LED OFF\n");
}
}
}

void HC05_init(void)
{
SYSCTL_RCGCUART_R |= 0x20;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    Delay(1);
    /* UART0 initialization */
    UART5_CTL_R = 0;         /* UART5 module disbable */
    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5_CC_R = 0;          /*select system clock*/
    UART5_LCRH_R = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTE_DEN_R = 0x30;      /* set PE4 and PE5 as digital */
    GPIO_PORTE_AFSEL_R = 0x30;    /* Use PE4,PE5 alternate function */
    GPIO_PORTE_AMSEL_R = 0;    /* Turn off analg function*/
    GPIO_PORTE_PCTL_R = 0x00110000;     /* configure PE4 and PE5 for UART */
}
char Bluetooth_Read(void)  
{
    char data;
 while((UART5_FR_R & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5_DR_R ;   /* before giving it another byte */
    return (unsigned char) data;
}

void Bluetooth_Write(unsigned char data)  
{
    while((UART5_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5_DR_R = data;                  /* before giving it another byte */
}

void Bluetooth_Write_String(char *str)
{
  while(*str)
{
Bluetooth_Write(*(str++));
}
}

void Delay(unsigned long counter)
{
unsigned long i = 0;

for(i=0; i< counter; i++);
}

analog keypad interfacing using uart

#include <stdint.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"

void UART0_Init(void);
void UART_TxChar(char data);
void UART_TxString(char *str);
unsigned char key_scan(unsigned int volatile rec_val);
void delay_ms(uint32_t ms);

int main(void)
{
    uint32_t ui32ADC0Value;
    unsigned char current_key = '\0';
    unsigned char last_key = '\0';
    
    SYSCTL_RCGCGPIO_R |= (1 << 3);
    SYSCTL_RCGCADC_R |= 1;
    
    GPIO_PORTD_AFSEL_R |= 0x04;
    GPIO_PORTD_DEN_R &= ~0x04;
    GPIO_PORTD_AMSEL_R |= 0x04;
    
    ADC0_ACTSS_R &= ~1;
    ADC0_EMUX_R &= ~0x000F;
    ADC0_SSMUX0_R |= 0x05;
    ADC0_SSCTL0_R |= 0x06;
    ADC0_ACTSS_R |= 0x01;
    
    UART0_Init();
    UART_TxString("Analog Keypad Ready\n\r");
    UART_TxString("Press a key...\n\r");
    
    while(1)
    {
        ADC0_PSSI_R |= 1;
        while((ADC0_RIS_R & 1) == 0);
        ui32ADC0Value = ADC0_SSFIFO0_R;
        ADC0_ISC_R = 1;
        
        current_key = key_scan(ui32ADC0Value / 10);
        
        if (current_key != 'G' && current_key != last_key) {
            UART_TxString("Key Pressed: ");
            UART_TxChar(current_key);
            UART_TxString("\n\r");
            
            last_key = current_key;
        }
        
        delay_ms(100);
    }
}

unsigned char key_scan(unsigned int volatile rec_val)
{
    if (rec_val >= 373 && rec_val <= 374) return '0';
    else if (rec_val >= 376 && rec_val <= 377) return '1';
    else if (rec_val >= 380 && rec_val <= 382) return '2';
    else if (rec_val >= 381 && rec_val <= 382) return '3';
    else if (rec_val >= 357 && rec_val <= 358) return '4';
    else if (rec_val >= 362 && rec_val <= 363) return '5';
    else if (rec_val >= 367 && rec_val <= 369) return '6';
    else if (rec_val >= 371 && rec_val <= 372) return '7';
    else if (rec_val >= 317 && rec_val <= 318) return '8';
    else if (rec_val >= 333 && rec_val <= 334) return '9';
    else if (rec_val >= 345 && rec_val <= 347) return 'A';
    else if (rec_val >= 353 && rec_val <= 354) return 'B';
    else if (rec_val == 0) return 'C';
    else if (rec_val >= 203 && rec_val <= 204) return 'D';
    else if (rec_val >= 272 && rec_val <= 274) return 'E';
    else if (rec_val >= 306 && rec_val <= 308) return 'F';
    else
        return 'G';
}

void UART0_Init(void)
{
    SYSCTL_RCGCUART_R |= (1 << 0);
    SYSCTL_RCGCGPIO_R |= (1 << 0);
    
    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTA_DEN_R |= 0x03;
    
    UART0_CTL_R &= ~0x01;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = 0x70;
    UART0_CTL_R |= 0x301;
}

void UART_TxChar(char data) {
    while ((UART0_FR_R & 0x20) != 0);
    UART0_DR_R = data;
}

void UART_TxString(char *str) {
    while (*str) {
        UART_TxChar(*str++);
    }
}

void delay_ms(uint32_t ms) {
    uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 3180; j++) {}
    }
}

analog keypad interfacing using lcd

#include <stdint.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

#define LCD_clear() LCD_command(0x1)
#define LCD_origin() LCD_command(0x2)
#define LCD_row1() LCD_command(0x80)
#define LCD_row2() LCD_command(0xC0)

void shift_outl(unsigned char str);
void LCD_enable(void);
void LCD_command(unsigned char command);
void LCD_putc(unsigned char ascii);
void LCD_puts(char *lcd_string);
void LCD_init(void);
unsigned char key_scan(unsigned int volatile rec_val);

int main(void)
{
    uint32_t ui32ADC0Value;
    unsigned char Dig_val;
    volatile uint32_t ui32TempAvg, Temp_val;
    
    SYSCTL_RCGCGPIO_R |= 0x039;
    SYSCTL_RCGCADC_R |= 1;
    
    GPIO_PORTD_AFSEL_R |= 8;
    GPIO_PORTD_DEN_R &= ~8;
    GPIO_PORTD_AMSEL_R |= 8;
    GPIO_PORTE_DIR_R |= 0x0C;
    GPIO_PORTE_DEN_R |= 0x0C;
    GPIO_PORTA_DIR_R |= 0x20;
    GPIO_PORTA_DEN_R |= 0x20;
    GPIO_PORTF_DIR_R |= 0x01;
    GPIO_PORTF_DEN_R |= 0x01;
    
    ADC0_ACTSS_R &= ~1;
    ADC0_EMUX_R &= ~0x000F;
    ADC0_SSMUX0_R |= 0x05;
    ADC0_SSCTL0_R |= 0x06;
    ADC0_ACTSS_R |= 0x01;
    
    LCD_init();
    LCD_row1();
    LCD_puts("Interfacing");
    LCD_row2();
    LCD_puts("Analog KEYPAD");
    SysCtlDelay(5000000);
    LCD_clear();

    while(1)
    {
        ADC0_PSSI_R |= 1;
        while((ADC0_RIS_R & 1) == 0);
        ui32ADC0Value = ADC0_SSFIFO0_R;
        ADC0_ISC_R = 1;
        Temp_val = ui32ADC0Value/10;
        Dig_val = key_scan(Temp_val);
        SysCtlDelay(50000);
        LCD_row1();
        LCD_puts("Press any Key");
        LCD_row2();
        if(Dig_val <= 'F')
            LCD_putc(Dig_val);
    }
}

unsigned char key_scan(unsigned int volatile rec_val)
{
    if(rec_val>=373 && rec_val<= 374) return '0';
    else if(rec_val >= 376 && rec_val <= 377) return '1';
    else if(rec_val >= 379 && rec_val <= 380) return '2';
    else if(rec_val >= 381 && rec_val <= 382) return '3';
    else if(rec_val >= 357 && rec_val <= 358) return '4';
    else if(rec_val >= 362 && rec_val <= 363) return '5';
    else if(rec_val >= 367 && rec_val <= 368) return '6';
    else if(rec_val >= 371 && rec_val <= 372) return '7';
    else if(rec_val >= 317 && rec_val <= 318) return '8';
    else if(rec_val >= 333 && rec_val <= 334) return '9';
    else if(rec_val >= 345 && rec_val <= 346) return 'A';
    else if(rec_val >= 353 && rec_val <= 354) return 'B';
    else if(rec_val == 0) return 'C';
    else if(rec_val >= 203 && rec_val <= 204) return 'D';
    else if(rec_val >= 272 && rec_val <= 273) return 'E';
    else if(rec_val >= 306 && rec_val <= 307) return 'F';
    else
        return 'G';
}

void LCD_puts(char *lcd_string)
{
    while(*lcd_string)
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
    LCD_command(0x28);
    SysCtlDelay(500000);
    LCD_command(0x0C);
    SysCtlDelay(500000);
    LCD_command(0x06);
    SysCtlDelay(500000);
    LCD_command(0x01);
    SysCtlDelay(500000);
}

unsigned char PP1 = 0x00;
void LCD_putc(unsigned char ascii)
{
    unsigned char volatile num;
    num = ascii;
    num = ((num & 0x11)<<3)|((num & 0x22)<<1)|((num & 0x44)>>1)|((num & 0x88)>>3);
    ascii = num;
    PP1 = (PP1 & 0xF0)|((ascii>>4) & 0x0F);
    PP1 = PP1 | 0xA0;
    PP1 = PP1 & ~(0x40);
    shift_outl(PP1);
    SysCtlDelay(500);
    PP1 = PP1 & ~(0x20);
    shift_outl(PP1);
    SysCtlDelay(500);
    PP1 = (PP1 & 0xF0)|(ascii & 0x0F);
    PP1 = PP1 | 0xA0;
    PP1 = PP1 & ~(0x40);
    shift_outl(PP1);
    SysCtlDelay(500);
    PP1 = PP1 & ~(0x20);
    shift_outl(PP1);
    SysCtlDelay(500);
}

void shift_outl(unsigned char str)
{
    unsigned char j=0, check;
    GPIO_PORTA_DATA_R = 0x00;
    for(j=0;j<=7;j++)
    {
        GPIO_PORTE_DATA_R = 0x00;
        check = (str & (1<<j));
        if(check)
            GPIO_PORTE_DATA_R |= 0x04;
        else
            GPIO_PORTE_DATA_R = 0x00;
        GPIO_PORTE_DATA_R |= 0x08;
    }
    GPIO_PORTA_DATA_R |= 0x20;
}

unsigned char PP0 = 0x00;
void LCD_command(unsigned char command)
{
    unsigned char volatile num;
    num = command;
    num = ((num & 0x11)<<3)|((num & 0x22)<<1)|((num & 0x44)>>1)|((num & 0x88)>>3);
    command = num;
    PP0 = (PP0 & 0xF0)|((command>>4) & 0x0F);
    PP0 = PP0 | (1<<(7));
    shift_outl(PP0);
    SysCtlDelay(500);
    PP0 = PP0 & ~(0x20);
    shift_outl(PP0);
    SysCtlDelay(500);
    PP0 = (PP0 & 0xF0)|(command & 0x0F);
    PP0 = PP0 | 0x80;
    shift_outl(PP0);
    SysCtlDelay(500);
    PP0 = PP0 & ~(0x20);
    shift_outl(PP0);
    SysCtlDelay(500);
}

generation of sine wave through dac using i2c module

void MCP4725_data(int n) {
    char data[2];
    
    data[1] = (n << 4) & 0xFF;
    data[0] = (n >> 4) & 0xFF;
    I2C1_burstWrite(SLAVE_ADDR, 0x40, 2, data);
}

void I2C1_init(void) {
    SYSCTL->RCGCI2C |= 0x02;
    SYSCTL->RCGCGPIO |= 0x01;
    
    GPIOA->AFSEL |= 0xC0;
    GPIOA->PCTL &= ~0xFF000000;
    GPIOA->PCTL |= 0x33000000;
    GPIOA->DEN |= 0xC0;
    GPIOA->ODR |= 0x80;
    
    I2C1->MCR = 0x10;
    I2C1->MTPR = 24;
}

static int I2C_wait_till_done(void) {
    while(I2C1->MCS & 1);
    return I2C1->MCS & 0xE;
}

int I2C1_burstWrite(int slaveAddr, char command, int byteCount, const char* data) {
    char error;
    
    if (byteCount <= 0)
        return -1;
    
    I2C1->MSA = slaveAddr << 1;
    I2C1->MDR = command;
    I2C1->MCS = 3;
    
    error = I2C_wait_till_done();
    if (error) return error;
    
    while (byteCount > 1)
    {
        I2C1->MDR = *data++;
        I2C1->MCS = 1;
        error = I2C_wait_till_done();
        if (error) return error;
        byteCount--;
    }
    
    I2C1->MDR = *data++;
    I2C1->MCS = 5;
    error = I2C_wait_till_done();
    while(I2C1->MCS & 0x40);
    if (error) return error;
    
    return 0;
}

void delayMs(int n) {
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < 6265; j++)
        {} 
}

pwm

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"

int main(void)
{
void delayMs(int n);
int x = 0xFF00;

SYSCTL_RCGCPWM_R = 0x02; 
SYSCTL_RCGCGPIO_R = 0x20; 
SYSCTL_RCC_R = 0x00100000; 
SYSCTL_RCC_R = 0x000E0000; 

GPIO_PORTF_AFSEL_R = 0x08; 
GPIO_PORTF_PCTL_R = ~0x0000F000; 
GPIO_PORTF_PCTL_R = 0x00005000;
GPIO_PORTF_DEN_R = 0x08; 

PWM1_3_CTL_R = 0; 
PWM1_3_GENB_R = 0x0000008C; 

PWM1_3_LOAD_R = 0xFFFF; 
PWM1_3_CMPA_R = x; 
PWM1_3_CTL_R = 0x01; 
PWM1_ENABLE_R = 0x80; 

for(;;)
{

PWM1_3_CMPA_R = x;
delayMs(300);
}
}

void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++)
{} 
}


rgb interfacing

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"
void delayMs(int n);
int main(void)
{
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
GPIO_PORTF_DIR_R = 0x0E;
GPIO_PORTF_DEN_R = 0x0E;
while(1)
{
GPIO_PORTF_DATA_R = 0x02;
delayMs(500);
GPIO_PORTF_DATA_R = 0x04;
delayMs(500);
GPIO_PORTF_DATA_R = 0x08;
delayMs(500);
}}
void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 4180; j++)
{}
}

controlling led and buzzer using sw1 switch

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"
void delayMs(int n);
int value;
int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    GPIO_PORTD_LOCK_R = 0x4C4F434B;
    GPIO_PORTD_CR_R = 0x80;
    GPIO_PORTD_DIR_R = 0xf0;
    GPIO_PORTD_DEN_R = 0xf0;
    GPIO_PORTF_DIR_R = 0x08;
    GPIO_PORTF_DEN_R = 0x18;
    GPIO_PORTF_PUR_R = 0x10;
    while (1)
    {
        value = GPIO_PORTF_DATA_R;
        value = ~value;
        value = value >> 1;
        GPIO_PORTF_DATA_R = value;
        value = value << 1;
        GPIO_PORTD_DATA_R = value;
    }
}
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 4180; j++)
            {}
}

external interrupt via switch sw1 to glow led

#include<stdint.h>
#include "inc\TM4C123GH6PM.h"
void delayMs (int n);
int value;
int main(void)
{
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

GPIO_PORTD_LOCK_R = 0x4C4F434B;
GPIO_PORTD_CR_R = 0x80;
GPIO_PORTD_DIR_R = 0Xf0;
GPIO_PORTD_DEN_R = 0Xf0;

GPIO_PORTF_DIR_R = 0x08;
GPIO_PORTD_DEN_R = 0X18;
GPIO_PORTD_PUR_R = 0X10;

while (1)
{
value = GPIO_PORTF_DATA_R;
value = ~value;
value = value >> 1;
GPIO_PORTF_DATA_R = value;
value = value << 4;
GPIO_PORTD_DATA_R = value;
}
}

void delayMs (int n)
{
int i,j;
for (i=0;i<n;i++)
for (j=0;j<4180;j++){
}
}

dc motor (clockwise and anticlockwise)

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"

void delayMs(int n);

int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x0E;

    while(1)
    {
        GPIO_PORTF_DATA_R = 0x08;
        delayMs(500);
         GPIO_PORTF_DATA_R = 0x04;
        delayMs(500);
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 4180; j++)
            {}
}

stepper motor clockwise and anticlockwise

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"

void delayMs(int n);

int main(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;

    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTB_DIR_R = 0x08;
GPIO_PORTC_DIR_R = 0x10;

GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTB_DEN_R = 0x08;
GPIO_PORTC_DEN_R = 0x10;

    while(1)
    {
        GPIO_PORTF_DATA_R = 0x04;
        delayMs(500);
GPIO_PORTF_DATA_R = 0;
        delayMs(500);

GPIO_PORTF_DATA_R = 0x08;
        delayMs(500);
GPIO_PORTF_DATA_R = 0;
        delayMs(500);

GPIO_PORTF_DATA_R = 0x08;
        delayMs(500);
GPIO_PORTF_DATA_R = 0;
        delayMs(500);

GPIO_PORTF_DATA_R = 0x10;
        delayMs(500);
GPIO_PORTF_DATA_R = 0;
        delayMs(500);
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 4180; j++)
            {}
}