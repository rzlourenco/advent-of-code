#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    long high_mark = 0;
    long current_mark = 0;

    char *line = NULL;
    size_t line_size = 0;
    while (getline(&line, &line_size, stdin) != -1) {
        if (strncmp(line, "\n", 1) == 0) {
            if (current_mark > high_mark) {
                high_mark = current_mark;
            }
            current_mark = 0;
            continue;
        }

        char *err = NULL;
        long value = strtol(line, &err, 10);
        current_mark += value;
    }

    printf("%ld\n", high_mark);
}
