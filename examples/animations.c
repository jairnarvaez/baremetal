#include "animations.h"
#include <stdint.h>

const uint8_t PACMAN_OPEN[] = { 0x0E, 0x1C, 0x19, 0x1C, 0x0E };
const uint8_t PACMAN_CLOSED[] = { 0x0E, 0x1E, 0x1F, 0x1E, 0x0E };

const uint8_t STAR_FULL[] = { 0x04, 0x0E, 0x1F, 0x0E, 0x04 };
const uint8_t STAR_MEDIUM[] = { 0x00, 0x04, 0x0E, 0x04, 0x00 };
const uint8_t STAR_SMALL[] = { 0x00, 0x00, 0x04, 0x00, 0x00 };

const uint8_t ARROW_UP[] = { 0x04, 0x0E, 0x15, 0x04, 0x04 };
const uint8_t ARROW_RIGHT[] = { 0x04, 0x02, 0x1F, 0x02, 0x04 };
const uint8_t ARROW_DOWN[] = { 0x04, 0x04, 0x15, 0x0E, 0x04 };
const uint8_t ARROW_LEFT[] = { 0x04, 0x08, 0x1F, 0x08, 0x04 };

const uint8_t HEART_SMALL[] = { 0x00, 0x0A, 0x0E, 0x04, 0x00 };
const uint8_t HEART_BIG[] = { 0x0A, 0x1F, 0x1F, 0x0E, 0x04 };

const uint8_t SMILE[] = { 0x00, 0x0A, 0x00, 0x11, 0x0E };
const uint8_t WINK[] = { 0x00, 0x08, 0x00, 0x11, 0x0E };

// Otra definicion
const uint8_t X_PATTERN[] = {
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001
};

const uint8_t PLUS_PATTERN[] = {
    0b00100,
    0b00100,
    0b11111,
    0b00100,
    0b00100
};

void animation_star(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(STAR_SMALL, time);
        show(STAR_MEDIUM, time);
        show(STAR_FULL, time);
    }
}

void animation_pacman(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(PACMAN_OPEN, time);
        show(PACMAN_CLOSED, time);
    }
}

void animation_x_rotate(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(X_PATTERN, time);
        show(PLUS_PATTERN, time);
    }
}

void animation_heart(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(HEART_SMALL, time);
        show(HEART_BIG, time);
    }
}

void animation_arrow(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(ARROW_UP, time);
        show(ARROW_RIGHT, time);
        show(ARROW_DOWN, time);
        show(ARROW_LEFT, time);
    }
}

void animation_smile(uint32_t time, uint32_t repetitions)
{
    for (int i = 0; i < repetitions; i++) {
        show(SMILE, time);
        show(WINK, time);
    }
}
