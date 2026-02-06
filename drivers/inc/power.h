#ifndef POWER_H
#define POWER_H

#define BASE 0x40000000
#define POWER (*(volatile struct _power*)BASE)

struct _power {
    char _initial_padding[0x078]; // 0x00
    unsigned TASKS_CONSTLAT; // 0x078
    unsigned TASKS_LOWPWR; // 0x07C
    char _pad1[0x108 - 0x080];
    unsigned EVENTS_POFWARN; // 0x108
    char _pad2[0x114 - 0x10C];
    unsigned EVENTS_SLEEPENTER; // 0x114
    unsigned EVENTS_SLEEPEXIT; // 0x118
    unsigned EVENTS_USBDETECTED; // 0x11c
    unsigned EVENTS_USBREMOVED; // 0x120
    unsigned EVENTS_USBPWRRDY; // 0x124
    char _pad3[0x304 - 0x128];
    unsigned INTENSET; // 0x304
    unsigned INTENCLR; // 0x308
    char _pad4[0x400 - 0x30C];
    unsigned RESETREAS; // 0x400
    char _pad5[0x428 - 0x404];
    unsigned RAMSTATUS; // 0x428
    char _pad6[0x438 - 0x42C];
    unsigned USBREGSTATUS; // 0x438
    char _pad7[0x500 - 0x43C];
    unsigned SYSTEMOFF; // 0x500
    char _pad8[0x510 - 0x504];
    unsigned POFCON; // 0x510
    char _pad9[0x51C - 0x514];
    unsigned GPREGRET; // 0x51c
    unsigned GPREGRET2; // 0x520
    char _pad10[0x578 - 0x524];
    unsigned DCDCEN; // 0x578
    char _pad11[0x640 - 0x57C];
    unsigned MAINREGSTATUS; // 0x640
};

#endif
