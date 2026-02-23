#ifndef ERROR_H
#define ERROR_H

#define LED_MASK0 0xd1688800
#define LED_MASK1 0x00000020

void error_blink();

#define ERR_UNKNOWN_CMD "ERR 01\n"
#define ERR_INVALID_ARGS "ERR 02\n"
#define ERR_INVALID_VALUE "ERR 03\n"
#define ERR_NOT_INIT "ERR 04\n"
#define ERR_INVALID_PIN "ERR 05\n"
#define ERR_TIMEOUT "ERR 06\n"

#endif
