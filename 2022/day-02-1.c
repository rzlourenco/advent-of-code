#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

enum {
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2,
};

int
main()
{
    int final_score = 0;
    char line[10] = { 0 };
    static int score_matrix[3][3] = {
        { 3, 6, 0 },
        { 0, 3, 6 },
        { 6, 0, 3 },
    };
    while (fgets(line, sizeof(line), stdin)) {
        int opponent = line[0] - 'A';
        int player = line[2] - 'X';

        final_score += player + 1;
        final_score += score_matrix[opponent][player];
    }

    printf("%d\n", final_score);
}
