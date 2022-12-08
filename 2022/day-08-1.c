#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    };
    char directions[4][101] = { 0 };
    char visible[101][101] = {};
    memset(visible, ' ', sizeof(visible));

    for (int i = 0; i < length; ++i) {
        directions[NORTH][i] = visible[0][i] = trees[0][i];
        directions[EAST][i] = visible[i][length - 1] = trees[i][length - 1];
        directions[SOUTH][i] = visible[length - 1][i] = trees[length - 1][i];
        directions[WEST][i] = visible[i][0] = trees[i][0];
    }

    for (int sweep = 1; sweep < length; ++sweep) {
        for (int i = 0; i < length; ++i) {
            if (trees[sweep][i] > directions[NORTH][i]) {
                directions[NORTH][i] = visible[sweep][i] = trees[sweep][i];
            }
            if (trees[i][length - 1 - sweep] > directions[EAST][i]) {
                directions[EAST][i] = visible[i][length - 1 - sweep] =
                    trees[i][length - 1 - sweep];
            }
            if (trees[length - 1 - sweep][i] > directions[SOUTH][i]) {
                directions[SOUTH][i] = visible[length - 1 - sweep][i] =
                    trees[length - 1 - sweep][i];
            }
            if (trees[i][sweep] > directions[WEST][i]) {
                directions[WEST][i] = visible[i][sweep] = trees[i][sweep];
            }
        }
    }

    int total = 0;
    for (int row = 0; row < length; ++row) {
        for (int col = 0; col < length; ++col) {
            if (visible[row][col] != ' ') {
                total++;
            }
        }
    }

    printf("%d\n", total);
}
