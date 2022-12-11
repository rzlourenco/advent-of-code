#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct monkey {
    struct {
        enum {
            MOT_PLUS,
            MOT_TIMES,
            MOT_SQUARE,
        } type;
        int value;
    } operation;
    int test;
    int monkey_false;
    int monkey_true;

    int *items;
    int items_count;
    int items_capacity;
    int inspected_items;
};

static void
give_monkey_item(struct monkey *monkey, int item)
{
    if (monkey->items_capacity == 0) {
        monkey->items_capacity = 8;
        monkey->items =
            calloc(monkey->items_capacity, sizeof(monkey->items[0]));
    }
    if (monkey->items_count == monkey->items_capacity) {
        monkey->items_capacity *= 2;
        monkey->items = realloc(
            monkey->items, monkey->items_capacity * sizeof(monkey->items[0]));
    }
    monkey->items[monkey->items_count++] = item;
}

static void
print_monkey(struct monkey *monkey)
{
    printf(
        "Monkey %p(op=%d,%2d test=%d[%2d,%2d] inspected=%4d):",
        monkey,
        monkey->operation.type,
        monkey->operation.value,
        monkey->test,
        monkey->monkey_false,
        monkey->monkey_true,
        monkey->inspected_items);
    for (int item_ix = 0; item_ix < monkey->items_count; ++item_ix) {
        printf(" %3d", monkey->items[item_ix]);
    }
    puts("");
}

static void
parse_item_list(struct monkey *monkey, char *items)
{
    char *cursor = items;
    for (;;) {
        int item = strtol(cursor, &cursor, 10);

        // No items.
        if (cursor == items) {
            return;
        }

        give_monkey_item(monkey, item);

        // It was the last item.
        if (*cursor == '\0' || *cursor == '\n') {
            return;
        }

        if (*cursor == ',') {
            cursor += 2;
        }
    }
}

static void
monkey_business(int sz, struct monkey monkeys[static sz])
{
    int rounds = 20;
    while (rounds-- > 0) {
        for (int monkey_ix = 0; monkey_ix < sz; ++monkey_ix) {
            struct monkey *monkey = &monkeys[monkey_ix];

            int to_inspect = monkey->items_count;
            monkey->inspected_items += to_inspect;
            for (int item_ix = 0; item_ix < to_inspect; ++item_ix) {
                int worry_level = monkey->items[item_ix];
                switch (monkey->operation.type) {
                case MOT_PLUS:
                    worry_level += monkey->operation.value;
                    break;
                case MOT_TIMES:
                    worry_level *= monkey->operation.value;
                    break;
                case MOT_SQUARE:
                    worry_level *= worry_level;
                    break;
                }

                worry_level /= 3;
                if (worry_level % monkey->test == 0) {
                    give_monkey_item(
                        &monkeys[monkey->monkey_true], worry_level);
                } else {
                    give_monkey_item(
                        &monkeys[monkey->monkey_false], worry_level);
                }
            }
            monkey->items_count = 0;
        }
    }
}

static int
parse_monkeys(int monkey_count, struct monkey monkeys[static monkey_count])
{
    int max_monkey = 0;
    int monkey_ix = 0;
    char line[80] = {};

    while (fgets(line, sizeof(line), stdin)) {
        if (*line == 'M') {
            monkey_ix = strtol(&line[sizeof("Monkey ") - 1], NULL, 10);
            assert(monkey_ix >= 0 && monkey_ix < 8);
            if (monkey_ix > max_monkey) {
                max_monkey = monkey_ix;
            }
            continue;
        } else if (strncmp(line, "  ", 2) != 0) {
            continue;
        } else if (line[2] == 'S') {
            parse_item_list(
                &monkeys[monkey_ix], &line[sizeof("  Starting items: ") - 1]);
        } else if (line[2] == 'O') {
            char *op = line + (sizeof("  Operation: new = old ") - 1);
            monkeys[monkey_ix].operation.value = strtol(op + 2, NULL, 0);
            if (*op == '+') {
                monkeys[monkey_ix].operation.type = MOT_PLUS;
            } else if (*op == '*' && op[2] == 'o') {
                monkeys[monkey_ix].operation.type = MOT_SQUARE;
            } else {
                monkeys[monkey_ix].operation.type = MOT_TIMES;
            }
        } else if (line[2] == 'T') {
            monkeys[monkey_ix].test =
                strtol(line + sizeof("  Test: divisible by ") - 1, NULL, 10);
        } else {
            char *condition = line + (sizeof("    If ") - 1);
            if (*condition == 't') {
                monkeys[monkey_ix].monkey_true = strtol(
                    condition + (sizeof("true: throw to monkey ") - 1),
                    NULL,
                    10);
            } else {
                monkeys[monkey_ix].monkey_false = strtol(
                    condition + (sizeof("false: throw to monkey ") - 1),
                    NULL,
                    10);
            }
        }
    }
    return max_monkey + 1;
}

int
compare_monkeys(const void *left, const void *right)
{
    return ((struct monkey *)right)->inspected_items
        - ((struct monkey *)left)->inspected_items;
}

int
main()
{
    enum {
        MONKEY_COUNT = 8,
    };
    struct monkey monkeys[MONKEY_COUNT] = {};
    int count = parse_monkeys(MONKEY_COUNT, monkeys);
    assert(count >= 2);
    monkey_business(count, monkeys);

    qsort(monkeys, count, sizeof(*monkeys), compare_monkeys);
    printf("%d\n", monkeys[0].inspected_items * monkeys[1].inspected_items);
}
