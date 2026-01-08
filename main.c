#define GPIO_BASE 0x50000000

#define P0_OUT (*(volatile unsigned int*)(GPIO_BASE + 0x504))
#define P0_OUTCLR (*(volatile unsigned int*)(GPIO_BASE + 0x50C))
#define P0_DIRSET (*(volatile unsigned int*)(GPIO_BASE + 0x518))

#define LED_ROW_3 15 // P21 - fila 3
#define LED_COL_3 31 // P28 - columna 3

int main(void)
{
    P0_DIRSET = (1 << LED_ROW_3) | (1 << LED_COL_3);

    P0_OUT = (1 << LED_ROW_3);
    P0_OUTCLR = (1 << LED_COL_3);

    while (1) {
    }
}
