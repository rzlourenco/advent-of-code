#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    int depth = 0;
    int size_stack[16] = { 0 };

    int num_directories = 0;
    int directory_sizes[1024] = { 0 };

    char line[100] = { 0 };
    while (fgets(line, sizeof(line), stdin)) {
        if (*line == '$') {
            if (strncmp(line + 2, "cd ", 3) == 0) {
                if (strncmp(line + 5, "..", 2) == 0) {
                    size_stack[depth - 1] += size_stack[depth];
                    directory_sizes[num_directories++] = size_stack[depth];
                    size_stack[depth--] = 0;
                } else if (strncmp(line + 5, "/", 1) == 0) {
                    depth = 0;
                } else {
                    depth++;
                }
            } else if (strncmp(line + 2, "ls", 2) == 0) {
                // Don't care?
            }
        } else if (strncmp(line, "dir ", 4) == 0) {
            // Don't care?
        } else {
            char *end = NULL;
            int file_size = strtol(line, &end, 10);
            size_stack[depth] += file_size;
        }
    }

    while (depth > 0) {
        directory_sizes[num_directories++] += size_stack[depth];
        size_stack[depth - 1] += size_stack[depth];
        size_stack[depth--] = 0;
    }

    int used_space = size_stack[0];
    int total_space = 70000000;
    int target = 30000000 - (total_space - used_space);
    int best_yeet = INT_MAX;
    for (int i = 0; i < num_directories; i++) {
        if (directory_sizes[i] >= target && directory_sizes[i] < best_yeet) {
            best_yeet = directory_sizes[i];
        }
    }
    printf("%d\n", best_yeet);
}
