#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(a) ((a) < 0 ? (-(a)) : (a))
#define SIGN(a) ((a) == 0 ? 0 : (a) < 0 ? (-1) : 1)

static int
compare_positions(const void *l, const void *r)
{
    return *(uint32_t *)l - *(uint32_t *)r;
}

int
main()
{
    union {
        uint32_t pos;
        struct {
            int16_t x, y;
        };
    } rope[10] = {};

    enum {
        POSITIONS_COUNT = 10000,
    };
    uint32_t positions[POSITIONS_COUNT] = {
        0x0000'0000,
    };
    uint16_t positions_ix = 1;

    char line[10] = {};
    while (fgets(line, sizeof(line), stdin)) {
        char direction = 0;
        short distance = 0;
        sscanf(line, "%c %hd\n", &direction, &distance);

        while (distance-- > 0) {
            switch (direction) {
            case 'U':
                rope[0].y += 1;
                break;
            case 'R':
                rope[0].x += 1;
                break;
            case 'D':
                rope[0].y -= 1;
                break;
            case 'L':
                rope[0].x -= 1;
                break;
            default:
                __builtin_unreachable();
            }

            assert(positions_ix + 1 < POSITIONS_COUNT);
            for (int i = 1; i < 10; i++) {
                short x_displacement = rope[i - 1].x - rope[i].x;
                short y_displacement = rope[i - 1].y - rope[i].y;

                bool displaced = false;
                if (ABS(x_displacement) == 2 || ABS(y_displacement) == 2) {
                    rope[i].x += SIGN(x_displacement);
                    rope[i].y += SIGN(y_displacement);
                    displaced = true;
                }

                if (displaced) {
                    if (i == 9) {
                        positions[positions_ix++] = rope[i].pos;
                    }
                }
            }
        }
    }

    qsort(positions, positions_ix, sizeof(*positions), compare_positions);

    int unique = 1;
    for (int i = 1; i < positions_ix; ++i) {
        if (positions[i] != positions[i - 1]) {
            unique += 1;
        }
    }
    printf("%d\n", unique);
}
