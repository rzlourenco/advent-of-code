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

    static const int play_matrix[3][3] = {
        { 3, 1, 2 },
        { 1, 2, 3 },
        { 2, 3, 1 },
    };

    while (fgets(line, sizeof(line), stdin)) {
        int opponent = line[0] - 'A';
        int result = line[2] - 'X';
        final_score += result * 3;

        int play = play_matrix[opponent][result];
        final_score += play;
    }

    printf("%d\n", final_score);
}
