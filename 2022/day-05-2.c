#include "dsa.h"

#include <stdio.h>

int
main()
{
    struct String stacks[9] = { 0 };

    char line[40] = { 0 };
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') {
            continue;
        } else if (line[0] == '[' || line[0] == ' ') {
            char item[9][4] = { 0 };
            memset(item, 0, sizeof(item));
            sscanf(
                line,
                "%3c%*c%3c%*c%3c%*c%3c%*c%3c%*c%3c%*c%3c%*c%3c%*c%3c\n",
                item[0],
                item[1],
                item[2],
                item[3],
                item[4],
                item[5],
                item[6],
                item[7],
                item[8]);

            for (int i = 0; i < 9; ++i) {
                if (item[i][0] == '[') {
                    string_insert_at_beginning(&stacks[i], item[i][1]);
                }
            }
        } else if (strncmp(line, "move ", 5) == 0) {
            int count, from, to;
            sscanf(line, "move %d from %d to %d\n", &count, &from, &to);
            string_move_tail(&stacks[to - 1], &stacks[from - 1], count);
        }
    }

    for (int i = 0; i < 9; i++) {
        if (stacks[i].count > 0) {
            putc(string_peek_back(&stacks[i]), stdout);
        }
    }
    puts("");
}
