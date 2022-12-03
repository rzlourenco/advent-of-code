#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>

int
main()
{
    int total_priority = 0;

    uint64_t common_items = 0;
    int rucksack_index = 0;

    char *line = NULL;
    size_t line_allocated = 0;
    ssize_t line_size = 0;
    while ((line_size = getline(&line, &line_allocated, stdin)) != -1) {
        if (line[line_size - 1] == '\n') {
            line_size -= 1;
        }

        uint64_t rucksack = 0;
        for (int i = 0; i < line_size; ++i) {
            uint64_t item =
                (line[i] >= 'a') ? (line[i] - 'a') : (line[i] - 'A' + 26);
            item = 1ULL << (item + 1);
            rucksack |= item;
        }

        if (rucksack_index == 0) {
            common_items = rucksack;
        } else {
            common_items &= rucksack;
        }

        if (rucksack_index == 2) {
            int a = __builtin_ctzl(common_items);
            total_priority += a;
        }

        rucksack_index += 1;
        if (rucksack_index == 3) {
            rucksack_index = 0;
        }
    }

    printf("%d\n", total_priority);
}
