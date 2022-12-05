#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

int
main()
{
    int overlaps = 0;

    char line[50];
    while (fgets(line, sizeof(line), stdin)) {
        int low1, high1, low2, high2;
        sscanf(line, "%d-%d,%d-%d\n", &low1, &high1, &low2, &high2);

        overlaps += (low2 >= low1 && low2 <= high1)
            || (high2 >= low1 && high2 <= high1)
            || (low1 >= low2 && low1 <= high2)
            || (high1 >= low2 && high1 <= high2);
    }

    printf("%d\n", overlaps);
}
