#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

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
    } head = {}, previous_head = {}, tail = {};

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
            previous_head = head;

            switch (direction) {
            case 'U':
                head.y += 1;
                break;
            case 'R':
                head.x += 1;
                break;
            case 'D':
                head.y -= 1;
                break;
            case 'L':
                head.x -= 1;
                break;
            default:
                __builtin_unreachable();
            }

            assert(positions_ix + 2 < POSITIONS_COUNT);
            short x_displacement = ABS(head.x - tail.x);
            short y_displacement = ABS(head.y - tail.y);
            if (x_displacement == 2 || y_displacement == 2) {
                tail = previous_head;
                positions[positions_ix++] = tail.pos;
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
