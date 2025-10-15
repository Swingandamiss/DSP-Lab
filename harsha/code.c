/*
Project -> Options for Target->Target->Xtal (MHz)->11.0592
Build
Start/Stop Debugging
View->Serial Windows->UART #1
Run
*/

/*
To change value->Pause Run->Peripherals->I/O Ports->Port 1->Enter new value for ADC (100->0x64,255->0xFF)->Run
*/

#include <reg52.h>
#include <stdio.h>

sbit ADC_INTR = P2^4;
sbit ADC_CS   = P2^5;
sbit ADC_RD   = P2^6;
sbit ADC_WR   = P2^7;
#define ADC_DATA_BUS P1

void delay_ms(unsigned int);
void uart_init(void);
void uart_transmit(unsigned char);
void uart_send_string(char*);
unsigned char read_adc(void);

void delay_ms(unsigned int count) {
    unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 1275; j++);
    }
}

void uart_init(void) {
    TMOD = 0x20;
    TH1  = 0xFD;
    SCON = 0x50;
    TR1  = 1;
}

void uart_transmit(unsigned char serial_data) {
    SBUF = serial_data;
    while(TI == 0);
    TI = 0;
}

void uart_send_string(char *str) {
    while(*str) {
        uart_transmit(*str++);
    }
}

unsigned char read_adc(void) {
    unsigned char adc_value;
    ADC_DATA_BUS = 0xFF; // Set port as input
    
    ADC_CS = 0; // Select ADC
    
    ADC_WR = 0; // Start conversion
    delay_ms(1);
    ADC_WR = 1;
    
    while(ADC_INTR == 1); // Wait for conversion to complete (INTR goes low)
    
    ADC_RD = 0; // Read data
    adc_value = ADC_DATA_BUS; 
    ADC_RD = 1;
    
    ADC_CS = 1; // Deselect ADC
    
    return adc_value;
}

void main(void) {
    unsigned char adc_val;
    unsigned int percentage;
    unsigned int volt_int, volt_frac;
    char buffer[50];

    uart_init();
    
    uart_send_string("ADC Reading Program Started\r\n");
    uart_send_string("-------------------------\r\n");

    while(1) {
        adc_val = read_adc();
        
        // Calculations for display
        percentage = (unsigned long)adc_val * 100 / 255;
        volt_int = (unsigned long)adc_val * 5 / 255;
        volt_frac = ((unsigned long)adc_val * 500 / 255) % 100;

        sprintf(buffer, "Raw: %3d | Percent: %3d%% | Voltage: %d.%02dV\r\n", 
                adc_val, percentage, volt_int, volt_frac);
        
        uart_send_string(buffer);

        delay_ms(500);
    }
}