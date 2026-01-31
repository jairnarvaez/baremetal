#define BASE0 0x50000500
#define BASE1 0x50000800

#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

#define GPIO0 (*(volatile struct _gpio*)BASE0)
#define GPIO1 (*(volatile struct _gpio*)BASE1)

typedef enum {
    LED_ROW1 = 21,
    LED_ROW2 = 22,
    LED_ROW3 = 15,
    LED_ROW4 = 24,
    LED_ROW5 = 19,

    LED_COL1 = 28,
    LED_COL2 = 11,
    LED_COL3 = 31,
    LED_COL4 = 5,
    LED_COL5 = 30,
} GPIO_LedMatrix_t;

struct _gpio {
    char _pad1[4]; // 0x500
    unsigned OUT; // 0x504
    unsigned OUTSET; // 0x508
    unsigned OUTCLR; // 0x50C
    char _pad2[4]; // 0x510
    unsigned DIR; // 0x514
    unsigned DIRSET; // 0x518
    unsigned DIRCLR; // 0x51C
    char _pad3[480]; // 0x520
    unsigned PIN_CNF[32]; // 0x700
};

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LedState;

#define SET_BIT(p) (1 << (p))

#define MATRIX_ROW_MASK (SET_BIT(LED_ROW1) | SET_BIT(LED_ROW2) | SET_BIT(LED_ROW3) | SET_BIT(LED_ROW4) | SET_BIT(LED_ROW5))

#define MATRIX_COL_PORT0_MASK (SET_BIT(LED_COL1) | SET_BIT(LED_COL2) | SET_BIT(LED_COL3) | SET_BIT(LED_COL5))

#define MATRIX_COL_PORT1_MASK SET_BIT(LED_COL4)

#define PIN_CNF_DIR 0
#define PIN_CNF_INPUT 1
#define PIN_CNF_PULL 2
#define PIN_CNF_DRIVE 8
#define PIN_CNF_SENSE 16

#define CNF_DIR_INPUT 0
#define CNF_DIR_OUTPUT 1
#define CNF_INPUT_CONNECT 0
#define CNF_INPUT_DISCONNECT 1
#define CNF_PULL_DISABLED 0
#define CNF_PULL_DOWN 1
#define CNF_PULL_UP 3
