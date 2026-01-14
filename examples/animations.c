#include "animations.h"

void animation_star()
{
    int star_full[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 }
    };

    int star_medium[5][5] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };

    int star_small[5][5] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };

    // Brillar 3 veces
    for (int i = 0; i < 3; i++) {
        show(star_small, 30);
        show(star_medium, 30);
        show(star_full, 50);
        show(star_medium, 30);
        show(star_small, 30);
    }
}

void animation_pacman()
{
    int pacman_open[5][5] = {
        { 0, 1, 1, 1, 0 },
        { 1, 1, 1, 0, 0 },
        { 1, 1, 0, 0, 1 },
        { 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 0 }
    };

    int pacman_closed[5][5] = {
        { 0, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 0 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 0 },
        { 0, 1, 1, 1, 0 }
    };

    for (int i = 0; i < 5; i++) {
        show(pacman_open, 400);
        show(pacman_closed, 400);
    }
}

void animation_x_rotate()
{
    int x_pattern[5][5] = {
        { 1, 0, 0, 0, 1 },
        { 0, 1, 0, 1, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 1, 0, 1, 0 },
        { 1, 0, 0, 0, 1 }
    };

    int plus_pattern[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 1, 1, 1, 1, 1 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0 }
    };

    for (int i = 0; i < 4; i++) {
        show(x_pattern, 150);
        show(plus_pattern, 150);
    }
}

void animation_heart()
{
    int heart_small[5][5] = {
        { 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };

    int heart_big[5][5] = {
        { 0, 1, 0, 1, 0 },
        { 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 }
    };

    int center[5][5];

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            center[i][j] = 0;
        }
    }

    center[2][2] = 1;

    show(heart_big, 750);
    show(heart_small, 75);
    show(center, 75);
    show(heart_small, 75);
}

void animation_arrow()
{
    int arrow_up[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 1, 1, 1, 0 },
        { 1, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0 }
    };

    int arrow_right[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 0, 0, 1, 0 },
        { 1, 1, 1, 1, 1 },
        { 0, 0, 0, 1, 0 },
        { 0, 0, 1, 0, 0 }
    };

    int arrow_down[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 0, 1, 0, 0 },
        { 1, 0, 1, 0, 1 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 }
    };

    int arrow_left[5][5] = {
        { 0, 0, 1, 0, 0 },
        { 0, 1, 0, 0, 0 },
        { 1, 1, 1, 1, 1 },
        { 0, 1, 0, 0, 0 },
        { 0, 0, 1, 0, 0 }
    };

    for (int spin = 0; spin < 3; spin++) {
        show(arrow_up, 175);
        show(arrow_right, 1075);
        show(arrow_down, 1075);
        show(arrow_left, 1075);
    }
}

void animation_smile()
{
    int smile[5][5] = {
        { 0, 0, 0, 0, 0 },
        { 0, 1, 0, 1, 0 },
        { 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1 },
        { 0, 1, 1, 1, 0 }
    };

    int wink[5][5] = {
        { 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1 },
        { 0, 1, 1, 1, 0 }
    };

    show(smile, 100);
    show(wink, 50);
    show(smile, 100);
    show(wink, 50);
    show(smile, 100);
}
