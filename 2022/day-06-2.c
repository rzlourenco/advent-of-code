#include <stdint.h>
#include <stdio.h>

int
main()
{
    char queue[14] = { 0 };
    int queue_ix = 0, count = 0;

    int push;
    while ((push = fgetc(stdin)) != EOF) {
        char pop = queue[queue_ix];
        queue[queue_ix] = push;
        if (++queue_ix > 13) {
            queue_ix = 0;
        }

        uint64_t mask = 0;
        for (int i = 0; i < 14; ++i) {
            if (queue[i]) {
                mask |= 1ULL << (queue[i] - 'a');
            }
        }

        count++;
        if (__builtin_popcountl(mask) == 14) {
            printf("%d\n", count);
            break;
        }
    }
}
