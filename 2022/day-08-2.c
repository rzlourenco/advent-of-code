#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

int
main()
{
    char trees[101][101] = { 0 };

    int height = 0;
    int width = 0;
    char line[101] = { 0 };
    while (fgets(line, sizeof(line), stdin)) {
        if (*line == '\n') {
            break;
        }

        char *p = line, *q = trees[height];
        while (*p != '\n') {
            *q++ = *p++;
        }
        if (p - line > width) {
            width = p - line;
        }
        height++;
    }

    assert(width == height);
    int length = width;

    enum {
        NORTH,
        EAST,
        SOUTH,
        WEST,
        DIR_MAX,
    };

    char previous_pos[DIR_MAX][101][10] = { 0 };
    char tall_runs[DIR_MAX][101][101] = { 0 };
    for (int j = 1; j < length - 1; ++j) {
        for (int i = 1; i < length - 1; ++i) {
            tall_runs[NORTH][j][i] = 1;
            tall_runs[EAST][j][i] = 1;
            tall_runs[SOUTH][j][i] = 1;
            tall_runs[WEST][j][i] = 1;
        }
    }

    for (int sweep = 1; sweep < length - 1; ++sweep) {
        for (int offset = 1; offset < length - 1; ++offset) {
            { // North
                int row = sweep;
                int col = offset;

                int this_tree = trees[row][col];
                int obstacle = 0;
                for (int i = this_tree - '0'; i < 10; ++i) {
                    obstacle = MAX(obstacle, previous_pos[NORTH][offset][i]);
                }

                previous_pos[NORTH][offset][this_tree - '0'] = row;
                tall_runs[NORTH][row][col] = row - obstacle;
            }

            { // East
                int row = offset;
                int col = length - 1 - sweep;

                int this_tree = trees[row][col];
                int obstacle = 0;
                for (int i = this_tree - '0'; i < 10; ++i) {
                    obstacle = MAX(obstacle, previous_pos[EAST][offset][i]);
                }

                previous_pos[EAST][offset][this_tree - '0'] = length - 1 - col;
                tall_runs[EAST][row][col] = (length - 1 - col) - obstacle;
            }

            { // South
                int row = length - 1 - sweep;
                int col = offset;

                int this_tree = trees[row][col];
                int obstacle = 0;
                for (int i = this_tree - '0'; i < 10; ++i) {
                    obstacle = MAX(obstacle, previous_pos[SOUTH][offset][i]);
                }

                previous_pos[SOUTH][offset][this_tree - '0'] = length - 1 - row;
                tall_runs[SOUTH][row][col] = (length - 1 - row) - obstacle;
            }

            { // West
                int row = offset;
                int col = sweep;

                int this_tree = trees[row][col];
                int obstacle = 0;
                for (int i = this_tree - '0'; i < 10; ++i) {
                    obstacle = MAX(obstacle, previous_pos[WEST][offset][i]);
                }

                previous_pos[WEST][offset][this_tree - '0'] = col;
                tall_runs[WEST][row][col] = col - obstacle;
            }
        }
    }

    int best_sight_tm_2022_4k_hdr = 0;
    for (int row = 1; row < length - 1; ++row) {
        for (int col = 1; col < length - 1; ++col) {
            int this_sight = tall_runs[NORTH][row][col]
                * tall_runs[EAST][row][col] * tall_runs[SOUTH][row][col]
                * tall_runs[WEST][row][col];
            if (this_sight > best_sight_tm_2022_4k_hdr) {
                best_sight_tm_2022_4k_hdr = this_sight;
            }
        }
    }
    printf("%d\n", best_sight_tm_2022_4k_hdr);
}
