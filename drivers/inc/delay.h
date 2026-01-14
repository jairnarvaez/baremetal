#define TIME 1000
#define TIME_ERROR 1500

#define nop() asm volatile("nop")

__attribute((noinline, section(".xram"))) void delay(unsigned n);
