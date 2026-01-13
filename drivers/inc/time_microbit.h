#define nop() asm volatile("nop")

__attribute((noinline, section(".xram"))) void delay(unsigned n);
