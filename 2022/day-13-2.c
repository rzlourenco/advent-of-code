#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    ORDERED = -1,
    NO_DECISION = 0,
    NOT_ORDERED = 1,
};

static void print_compare(char *left, char *right, int depth);
static int
order(char *left, char *right, char **next_left, char **next_right, int depth);
static int order_number_number(
    char *left, char *right, char **next_left, char **next_right, int depth);

static int
value_length(char *value)
{
    char *p = value;
    if (*p == '[') {
        ++p;
        int depth = 1;
        while (*p && depth > 0) {
            if (*p == ']') {
                depth--;
            }
            if (*p == '[') {
                depth++;
            }
            ++p;
        }
    }
    while (*p >= '0' && *p <= '9') {
        ++p;
    }
    assert(p - value > 0);
    return p - value;
}

static void
indent(int depth)
{
    while (depth-- > 0) {
        // printf("  ");
    }
}

static void
print_compare(char *left, char *right, int depth)
{
    indent(depth);
    int len_left = value_length(left);
    int len_right = value_length(right);
    // printf("- Compare %.*s vs %.*s\n", len_left, left, len_right, right);
}

static void
discard_to_end_of_list(char *p, char **next_p)
{
    // printf("!! DISCARDING {%s}\n", p);
    int depth = 1;
    while (*p && depth > 0) {
        if (*p == '[') {
            depth++;
        }
        if (*p == ']') {
            if (--depth == 0) {
                break;
            };
        }
        ++p;
    }
    // printf("!! REMAINING {%s}\n", p);
    if (next_p) {
        *next_p = p;
    }
}

static int
order_list_list(
    char *left, char *right, char **next_left, char **next_right, int depth)
{
    print_compare(left - 1, right - 1, depth);

    int r = NO_DECISION;
    while (*left != ']' && *right != ']') {
        r = order(left, right, &left, &right, depth + 1);
        assert(!*left || *left == ']' || *left == ',');
        assert(!*right || *right == ']' || *right == ',');
        if (*left == ',') {
            ++left;
        }
        if (*right == ',') {
            ++right;
        }
        if (r != NO_DECISION) {
            goto skip;
            return r;
        }
    }

    assert(r == NO_DECISION);
    bool exhausted_left = *left == ']' && *right != ']';
    bool exhausted_right = *left != ']' && *right == ']';
    if (exhausted_left) {
        indent(depth + 1);
        // puts("- Left side ran out of items, so inputs are in the right
        // order");
        r = ORDERED;
    } else if (exhausted_right) {
        indent(depth + 1);
        // puts("- Right side ran out of items, so inputs are not in the right "
        // "order");
        r = NOT_ORDERED;
    }

skip:
    if (next_left) {
        if (*left != ']') {
            discard_to_end_of_list(left, &left);
        }
        assert(*left == ']');
        *next_left = left + 1;
    }
    if (next_right) {
        if (*right != ']') {
            discard_to_end_of_list(right, &right);
        }
        assert(*right == ']');
        *next_right = right + 1;
    }
    return r;
}

static int
order_list_number(
    char *left, char *right, char **next_left, char **next_right, int depth)
{
    print_compare(left - 1, right, depth);

    int r = NO_DECISION;
    assert(*right != ']' && *right != '[');
    if (*left == ']') {
        indent(depth + 1);
        // puts("- Left side ran out of items, so inputs are in the right
        // order");
        while (*right != ']') {
            ++right;
        }
        r = ORDERED;
        goto ret;
    }
    if (*left == '[') {
        r = order_list_number(left + 1, right, &left, &right, depth + 1);
    } else {
        r = order_number_number(left, right, &left, &right, depth + 1);
    }
    if (r != NO_DECISION) {
        goto ret;
    }

    if (*left != ']') {
        indent(depth + 1);
        // puts("- Right side ran out of items, so inputs are not in the right "
        // "order");
        discard_to_end_of_list(left, &left);
        r = NOT_ORDERED;
    }

ret:
    if (next_right) {
        *next_right = right;
    }
    if (next_left) {
        *next_left = left;
    }
    return r;
}

static int
order_number_list(
    char *left, char *right, char **next_left, char **next_right, int depth)
{
    print_compare(left, right - 1, depth);
    assert(*left != ']' && *left != '[');

    int r = NO_DECISION;
    if (*right == ']') {
        indent(depth + 1);
        // puts("- Right side ran out of items, so inputs are not in the right "
        // "order");
        while (*left != ']') {
            ++left;
        }
        r = NOT_ORDERED;
        goto ret;
    }
    if (*right == '[') {
        r = order_number_list(left, right + 1, &left, &right, depth + 1);
    } else {
        r = order_number_number(left, right, &left, &right, depth + 1);
    }
    if (r != NO_DECISION) {
        goto ret;
    }

    if (*right != ']') {
        indent(depth + 1);
        // puts("- Left side ran out of items, so inputs are in the right
        // order");
        discard_to_end_of_list(right, &right);
        r = ORDERED;
    }

ret:
    if (next_left) {
        *next_left = left;
    }
    if (next_right) {
        *next_right = right;
    }
    return r;
}

static int
order_number_number(
    char *left, char *right, char **next_left, char **next_right, int depth)
{
    print_compare(left, right, depth);

    assert(*left >= '0' && *left <= '9');
    assert(*right >= '0' && *right <= '9');

    int l, r;
    l = strtol(left, next_left, 10);
    assert(errno == 0);
    r = strtol(right, next_right, 10);
    assert(errno == 0);
    if (l < r) {
        indent(depth + 1);
        // puts("- Left side is smaller, so inputs are in the right order");
        return ORDERED;
    }
    if (l > r) {
        indent(depth + 1);
        // puts("- Right side is smaller, so inputs are not in the right
        // order");
        return NOT_ORDERED;
    }
    return NO_DECISION;
}

static int
order(char *left, char *right, char **next_left, char **next_right, int depth)
{
    if (*left == '[' && *right == '[') {
        return order_list_list(
            left + 1, right + 1, next_left, next_right, depth);
    } else if (*left == '[' && *right != '[') {
        return order_list_number(left + 1, right, next_left, next_right, depth);
    } else if (*left != '[' && *right == '[') {
        return order_number_list(left, right + 1, next_left, next_right, depth);
    } else {
        return order_number_number(left, right, next_left, next_right, depth);
    }
}

static int
compare_packets(const void *left, const void *right)
{
    return order((char *)left, (char *)right, NULL, NULL, 0);
}

int
main()
{
    int packet_count = 0;

    char all_packets[512][256] = {};
    char line[256] = {};
    while (fgets(line, sizeof(line), stdin)) {
        char *p = line;
        if (*p == '\n') {
            continue;
        }
        while (*p && *p != '\n') {
            p++;
        }
        if (*p == '\n') {
            *p = '\0';
        }

        strcpy(all_packets[packet_count++], line);
    }
    assert(packet_count + 2 < 512);

    strcpy(all_packets[packet_count++], "[[2]]");
    strcpy(all_packets[packet_count++], "[[6]]");
    for (int i = 0; i < packet_count; i++) {
        // puts(all_packets[i]);
    }
    // puts("");

    qsort(all_packets, packet_count, sizeof(all_packets[0]), compare_packets);
    for (int i = 0; i < packet_count; i++) {
        // puts(all_packets[i]);
    }
    // puts("");

    int divider_2 = -1, divider_6 = -1;
    for (int i = 0; i < packet_count; ++i) {
        if (strcmp("[[2]]", all_packets[i]) == 0) {
            divider_2 = i + 1;
        }
        if (strcmp("[[6]]", all_packets[i]) == 0) {
            divider_6 = i + 1;
        }
    }

    printf("%d\n", divider_2 * divider_6);
}
