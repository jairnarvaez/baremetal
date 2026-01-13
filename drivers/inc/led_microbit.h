#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

#define GPIO0 (*(volatile struct _gpio*)0x50000500)
#define GPIO1 (*(volatile struct _gpio*)0x50000800)

#define ROW1 21
#define ROW2 22
#define ROW3 15
#define ROW4 24
#define ROW5 19

struct _gpio {
    char _pad1[4]; // 0x500
    unsigned OUT; // 0x504
    unsigned OUTSET; // 0x508
    unsigned OUTCLR; // 0x50C
    char _pad2[4]; // 0x510
    unsigned DIR; // 0x514
    unsigned DIRSET; // 0x518
    unsigned DIRCLR; // 0x51C
};

#define _ROW(row, c1, c2, c3, c4, c5) \
    _ROW_GPIO0(row, c1, c2, c3, c5), _ROW_GPIO1(c4)

#define _ROW_GPIO0(row, c1, c2, c3, c5) \
    ((1 << row) | ((!c1) << 28) | ((!c2) << 11) | ((!c3) << 31) | ((!c5) << 30))

#define _ROW_GPIO1(c4) \
    ((!c4) << 5)
