#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    int small_directory_sum = 0;

    int depth = 0;
    int size_stack[16] = { 0 };

    char line[100] = { 0 };
    while (fgets(line, sizeof(line), stdin)) {
        if (*line == '$') {
            if (strncmp(line + 2, "cd ", 3) == 0) {
                if (strncmp(line + 5, "..", 2) == 0) {
                    if (size_stack[depth] <= 100000) {
                        small_directory_sum += size_stack[depth];
                    }
                    size_stack[depth - 1] += size_stack[depth];
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
    printf("%d\n", small_directory_sum);
}
