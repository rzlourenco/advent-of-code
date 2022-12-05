#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct String {
    char *data;
    int count;
    size_t size;
};

static void
string_ensure_space(struct String *s, int extra)
{
    size_t required = s->count + extra;

    if (s->size == 0) {
        s->size = 8;
    }
    while (required >= s->size) {
        s->size *= 2;
    }
    s->data = realloc(s->data, s->size);
}

static void
string_insert_at_beginning(struct String *s, char c)
{
    string_ensure_space(s, 1);
    memmove(s->data + 1, s->data, s->count);
    s->data[0] = c;
    s->count++;
}

static char
string_pop_back(struct String *s)
{
    assert(s->count > 0 || !"fix your code");
    return s->data[--s->count];
}

static void
string_push_back(struct String *s, char c)
{
    string_ensure_space(s, 1);
    s->data[s->count++] = c;
}

static char
string_peek_back(struct String *s)
{
    assert(s->count > 0 || !"fix your code");
    return s->data[s->count - 1];
}

static void
string_move_tail(struct String *to, struct String *from, int count)
{
    assert(from->count >= count || !"fix your code");
    string_ensure_space(to, count);
    memcpy(to->data + to->count, from->data + from->count - count, count);
    from->count -= count;
    to->count += count;
}
