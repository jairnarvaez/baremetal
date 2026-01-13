#include "led_microbit.h"
#include "time_microbit.h"

int main(void) {

  static const unsigned set[] = {
      _ROW(ROW1, 1, 0, 0, 0, 0), _ROW(ROW1, 1, 1, 0, 0, 0),
      _ROW(ROW1, 1, 1, 1, 0, 0), _ROW(ROW1, 0, 1, 1, 1, 0),
      _ROW(ROW1, 0, 0, 1, 1, 1), _ROW(ROW1, 0, 0, 0, 1, 1),
      _ROW(ROW1, 0, 0, 0, 0, 1), _ROW(ROW1, 0, 0, 0, 0, 0),

      _ROW(ROW2, 1, 0, 0, 0, 0), _ROW(ROW2, 1, 1, 0, 0, 0),
      _ROW(ROW2, 1, 1, 1, 0, 0), _ROW(ROW2, 0, 1, 1, 1, 0),
      _ROW(ROW2, 0, 0, 1, 1, 1), _ROW(ROW2, 0, 0, 0, 1, 1),
      _ROW(ROW2, 0, 0, 0, 0, 1), _ROW(ROW2, 0, 0, 0, 0, 0),

      _ROW(ROW3, 1, 0, 0, 0, 0), _ROW(ROW3, 1, 1, 0, 0, 0),
      _ROW(ROW3, 1, 1, 1, 0, 0), _ROW(ROW3, 0, 1, 1, 1, 0),
      _ROW(ROW3, 0, 0, 1, 1, 1), _ROW(ROW3, 0, 0, 0, 1, 1),
      _ROW(ROW3, 0, 0, 0, 0, 1), _ROW(ROW3, 0, 0, 0, 0, 0),

      _ROW(ROW4, 1, 0, 0, 0, 0), _ROW(ROW4, 1, 1, 0, 0, 0),
      _ROW(ROW4, 1, 1, 1, 0, 0), _ROW(ROW4, 0, 1, 1, 1, 0),
      _ROW(ROW4, 0, 0, 1, 1, 1), _ROW(ROW4, 0, 0, 0, 1, 1),
      _ROW(ROW4, 0, 0, 0, 0, 1), _ROW(ROW4, 0, 0, 0, 0, 0),

      _ROW(ROW5, 1, 0, 0, 0, 0), _ROW(ROW5, 1, 1, 0, 0, 0),
      _ROW(ROW5, 1, 1, 1, 0, 0), _ROW(ROW5, 0, 1, 1, 1, 0),
      _ROW(ROW5, 0, 0, 1, 1, 1), _ROW(ROW5, 0, 0, 0, 1, 1),
      _ROW(ROW5, 0, 0, 0, 0, 1), _ROW(ROW5, 0, 0, 0, 0, 0)};

  GPIO0.DIRSET = LED_MASK0;
  GPIO1.DIRSET = LED_MASK1;

  while (1) {
    for (int i = 0; i < 80; i = i + 2) {
      GPIO0.OUTSET = set[i];
      GPIO1.OUTSET = set[i + 1];

      delay(20000);

      GPIO0.OUTCLR = set[i];
      GPIO1.OUTCLR = set[i + 1];
    }
  }
}
