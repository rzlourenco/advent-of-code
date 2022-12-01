#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    long high_mark[4] = { 0 };
    long current_mark = 0;

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, stdin) != -1) {
        if (strncmp(line, "\n", 1) == 0) {
            high_mark[3] = current_mark;
            for (int i = 3; i > 0; --i) {
                if (high_mark[i] > high_mark[i - 1]) {
                    long tmp = high_mark[i - 1];
                    high_mark[i - 1] = high_mark[i];
                    high_mark[i] = tmp;
                } else {
                    break;
                }
            }
            current_mark = 0;
            continue;
        }

        char *err = NULL;
        long value = strtol(line, &err, 10);
        current_mark += value;
    }

    current_mark = high_mark[0] + high_mark[1] + high_mark[2];
    printf("%ld\n", current_mark);
}
