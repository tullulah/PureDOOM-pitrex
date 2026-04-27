/*
 * test_blink.c - minimal baremetal test for Pi Zero 2 W
 * Just blinks the ACT LED - no vectrex, no DOOM, no complexity
 */

#define GPIO_BASE       0x3F200000UL
#define GPFSEL2         (*(volatile unsigned int*)(GPIO_BASE + 0x08))
#define GPSET0          (*(volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPCLR0          (*(volatile unsigned int*)(GPIO_BASE + 0x28))
#define ACT_PIN         29

static void delay(int cycles)
{
    for (volatile int i = 0; i < cycles; i++) {}
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

    /* Set GPIO29 as output */
    GPFSEL2 &= ~(7u << 27);
    GPFSEL2 |=  (1u << 27);

    /* Blink forever: 1 second on, 1 second off */
    for (;;) {
        GPSET0 = (1u << ACT_PIN);
        delay(10000000);
        GPCLR0 = (1u << ACT_PIN);
        delay(10000000);
    }

    return 0;
}
