#include "time_microbit.h"

void delay(unsigned n) {
  unsigned t = 16 * n;
  while (t > 0) {
    /* 62.5nsec per iteration */
    nop();
    t--;
  }
}
