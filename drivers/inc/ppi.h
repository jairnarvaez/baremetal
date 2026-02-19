#ifndef PPI_H
#define PPI_H

#define BASE_PPI 0x4001F000
#define PPI (*(volatile struct _ppi*)BASE_PPI)

typedef struct {
    unsigned ENABLE; // 0x000
    unsigned DISABLE; // 0x004
} Task_CHG_T;

typedef struct {
    unsigned EEP;
    unsigned TEP;
} CH_T;

struct _ppi {
    Task_CHG_T Task_CHG[6]; // 0x000
    char _pad1[0x500 - 0x030];
    unsigned CHEN; // 0x500
    unsigned CHENSET; // 0x504
    unsigned CHENCLR; // 0x508
    char _pad2[0x510 - 0x50C];
    CH_T CH[20]; // 0x510
    char _pad3[0x800 - 0x5B0];
    unsigned CHG[6];
    char _pad4[0x910 - 0x818];
    unsigned FORK_TEP[32]; // 0x910
};
#endif
