void delayMs(int n);
void shift_out1(unsigned char str);

int main(void)
{
    unsigned char a[16] = {
        0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0,
        0xFE, 0xF6, 0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E
    };

    SYSCTL_RCGCGPIO_R = 0x10;
    GPIO_PORTE_DIR_R = 0x1C;
    GPIO_PORTE_DEN_R = 0x1C;

    while(1)
    {
        shift_out1(a[1]);   // '0'
        shift_out1(a[15]);  // 'F'
        shift_out1(a[0]);   // '1'
        shift_out1(a[4]);   // '4'

        GPIO_PORTE_DATA_R |= 0x10;
        delayMs(1);
    }
}

void shift_out1(unsigned char str)
{
    unsigned char j;
    for(j = 0; j < 8; j++)
    {
        GPIO_PORTE_DATA_R = 0x00;
        if(str & (1 << j))
            GPIO_PORTE_DATA_R = 0x04;
        GPIO_PORTE_DATA_R |= 0x08;
    }
}

void delayMs(int n)
{
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < 4180; j++) {}
}