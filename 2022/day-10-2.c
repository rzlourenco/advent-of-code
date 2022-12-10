#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

int
main()
{
    char line[64] = {};
    int cycles = 1;
    int X = 1;

    enum {
        WIDTH = 40,
        HEIGHT = 6,
    };
    char display[WIDTH * HEIGHT] = {};
    memset(display, '.', sizeof(display));

    int signal_strength = 0;
    while (fgets(line, sizeof(line), stdin)) {
        int instruction_cycles = 0;
        int delta_X = 0;

        if (*line == 'n') {
            instruction_cycles = 1;
        } else if (*line == 'a') {
            instruction_cycles = 2;
            delta_X = strtol(line + sizeof("addx ") - 1, NULL, 10);
        }

        while (instruction_cycles--) {
            switch (cycles) {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                signal_strength += cycles * X;
                break;
            }

            if (X >= -1 || X <= WIDTH) {
                int horizontal_pos = (cycles - 1) % WIDTH;
                int vertical_pos = (cycles - 1) / WIDTH;
                if (ABS(X - horizontal_pos) <= 1) {
                    display[horizontal_pos + WIDTH * vertical_pos] = '#';
                }
            }

            cycles++;
        }

        X += delta_X;
    }

    for (int i = 0; i < HEIGHT; ++i) {
        printf("%.*s\n", WIDTH, display + i * WIDTH);
    }
}
