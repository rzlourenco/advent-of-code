#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define SIGN(a) ((a) == 0 ? 0 : (a) < 0 ? -1 : 1)

int
main()
{
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    static uint8_t cave_blocked[1024][1024] = {};
    memset(cave_blocked, '.', sizeof(cave_blocked));

    char line[512] = {};
    while (fgets(line, sizeof(line), stdin)) {
        if (*line == '\n') {
            continue;
        }

        int last_x = -1, last_y = -1;
        int x = 0, y = 0;
        char *current = line, *next = NULL;

        do {
            x = strtol(current, &next, 10);
            assert(current != next);
            assert(*next == ',');
            current = next + 1;
            y = strtol(current, &next, 10);
            assert(current != next);
            current = next;

            min_x = MIN(min_x, x);
            max_x = MAX(max_x, x);
            min_y = MIN(min_y, y);
            max_y = MAX(max_y, y);
            if (last_x == -1) {
                last_x = x;
                last_y = y;
                continue;
            }

            assert(x == last_x || y == last_y);

            int d_x = SIGN(x - last_x);
            int d_y = SIGN(y - last_y);
            assert(d_x == 0 || d_y == 0);
            assert(d_x != d_y);
            cave_blocked[y][x] = '#';
            if (d_x) {
                for (int i = last_x; i != x; i += d_x) {
                    cave_blocked[y][i] = '#';
                }
            }
            if (d_y) {
                for (int j = last_y; j != y; j += d_y) {
                    cave_blocked[j][x] = '#';
                }
            }

            last_x = x;
            last_y = y;
        } while (strncmp(current, " -> ", 4) == 0 && (current += 4));
    }

    int floor = (max_y += 2);
    for (int x = 0; x < 1024; ++x) {
        cave_blocked[floor][x] = '#';
    }

    bool clogged = false;
    int stopped_grains = 0;
    while (!clogged) {
        int sand_x = 500, sand_y = 0;

        while (true) {
            if (cave_blocked[sand_y + 1][sand_x] == '.') {
                sand_y += 1;
            } else if (cave_blocked[sand_y + 1][sand_x - 1] == '.') {
                sand_y += 1;
                sand_x -= 1;
            } else if (cave_blocked[sand_y + 1][sand_x + 1] == '.') {
                sand_y += 1;
                sand_x += 1;
            } else if (cave_blocked[sand_y][sand_x] != '.') {
                clogged = true;
                break;
            } else {
                stopped_grains += 1;
                cave_blocked[sand_y][sand_x] = 'o';
                break;
            }
            min_x = MIN(min_x, sand_x);
            max_x = MAX(max_x, sand_x);
            assert(sand_x >= 0 && sand_x < 1024);
        }
    }

    printf("%d\n", stopped_grains);
}
