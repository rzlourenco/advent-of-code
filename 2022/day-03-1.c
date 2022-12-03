#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>

int
main()
{
    int total_priority = 0;

    char *line = NULL;
    size_t line_allocated = 0;
    ssize_t line_size = 0;
    while ((line_size = getline(&line, &line_allocated, stdin)) != -1) {
        if (line[line_size - 1] == '\n') {
            line_size -= 1;
        }

        uint64_t rucksack1 = 0;
        uint64_t rucksack2 = 0;
        for (int i = 0; i < line_size; ++i) {
            uint64_t item =
                (line[i] >= 'a') ? (line[i] - 'a') : (line[i] - 'A' + 26);
            item = 1ULL << (item + 1);
            if (i < line_size / 2) {
                rucksack1 |= item;
            } else {
                rucksack2 |= item;
            }
        }

        int common_item = __builtin_ctzl(rucksack1 & rucksack2);
        total_priority += common_item;
    }

    printf("%d\n", total_priority);
}
