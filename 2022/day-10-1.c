#include <stdio.h>
#include <stdlib.h>

int
main()
{
    char line[64] = {};
    int cycles = 1;
    int X = 1;

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

            cycles++;
        }

        X += delta_X;
    }

    printf("%d\n", signal_strength);
}
