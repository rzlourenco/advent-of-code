#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((b) < (a) ? (b) : (a))

struct position {
    int8_t x, y;
};
struct position_queue {
    struct position *positions;
    uint32_t head;
    uint32_t count;
    uint32_t capacity;
};

void
position_queue_push_back(struct position_queue *q, struct position pos)
{
    assert(q->head >= 0);
    assert(q->head <= q->capacity);
    assert(q->count >= 0);
    assert(q->count <= q->capacity);
    if (q->count == q->capacity) {
        uint32_t old_capacity = q->capacity;
        q->capacity = q->capacity == 0 ? 8 : (q->capacity * 2);
        q->positions =
            realloc(q->positions, q->capacity * sizeof(q->positions[0]));
        if (old_capacity != 0) {
            for (int i = 0; i < q->count; ++i) {
                q->positions[(q->head + i) % q->capacity] =
                    q->positions[(q->head + i) % old_capacity];
            }
        }
    }
    q->positions[(q->head + q->count) % q->capacity] = pos;
    q->count++;
    assert(q->head >= 0);
    assert(q->head <= q->capacity);
    assert(q->count >= 0);
    assert(q->count <= q->capacity);
}

struct position
position_queue_pop_front(struct position_queue *q)
{
    assert(q->head >= 0);
    assert(q->head < q->capacity);
    assert(q->count > 0);
    assert(q->count <= q->capacity);
    struct position ret = q->positions[q->head++];
    q->count--;
    if (q->head >= q->capacity) {
        q->head -= q->capacity;
    }
    assert(q->head >= 0);
    assert(q->head <= q->capacity);
    assert(q->count >= 0);
    assert(q->count <= q->capacity);
    return ret;
}

int
main()
{
    enum {
        SIZE = 128,
    };
    int width = 0, height = 0;
    char heightmap[SIZE][SIZE] = {};

    char line[SIZE + 2] = {};
    while (fgets(line, sizeof(line), stdin)) {
        if (*line == 0 || *line == '\n') {
            continue;
        }

        char *p = line;
        while (*p && *p != '\n') {
            heightmap[height][p - line] = *p;
            p++;
        }
        width = p - line;
        height++;
    }

    struct position start = { .x = -1, .y = -1 };
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (heightmap[y][x] == 'E') {
                start.x = x;
                start.y = y;
            }
        }
    }
    assert(start.x != -1 && start.y != -1);

    bool visited[SIZE][SIZE];
    int16_t distance[SIZE][SIZE];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            distance[y][x] = -1;
            visited[y][x] = false;
        }
    }

    distance[start.y][start.x] = 0;

    struct position_queue queue = { 0 }, *q = &queue;
    position_queue_push_back(&queue, start);
    while (queue.count) {
        struct position pos = position_queue_pop_front(q);
        if (visited[pos.y][pos.x]) {
            continue;
        }

        visited[pos.y][pos.x] = true;
        struct position deltas[4] = {
            { .x = -1, .y = 0 },
            { .x = 1, .y = 0 },
            { .x = 0, .y = -1 },
            { .x = 0, .y = 1 },
        };

        char current_height = heightmap[pos.y][pos.x];
        for (int i = 0; i < 4; ++i) {
            struct position new_pos = {
                .x = pos.x + deltas[i].x,
                .y = pos.y + deltas[i].y,
            };
            if (new_pos.x < 0 || new_pos.x >= width) {
                continue;
            }
            if (new_pos.y < 0 || new_pos.y >= height) {
                continue;
            }

            char new_height = heightmap[new_pos.y][new_pos.x];
            if ((new_height == 'z' && current_height == 'E')
                || (new_height >= 'a' && new_height >= current_height - 1)) {
                if (distance[new_pos.y][new_pos.x] == -1) {
                    distance[new_pos.y][new_pos.x] = distance[pos.y][pos.x] + 1;
                } else {
                    distance[new_pos.y][new_pos.x] =
                        MIN(distance[new_pos.y][new_pos.x],
                            distance[pos.y][pos.x] + 1);
                }
                position_queue_push_back(q, new_pos);
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                printf(" %3d%c", distance[y][x], heightmap[y][x]);
            }
            puts("");
        }
        puts("");
    }

    int least = INT_MAX;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (heightmap[y][x] == 'a' && distance[y][x] != -1) {
                least = MIN(least, distance[y][x]);
            }
        }
    }

    printf("!!!!!!!!! OFF BY 2: %d\n", least);
}
