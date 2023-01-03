#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 100

typedef struct {
    char **items;
    int capacity;
    int size;
} Vector;

void vector_init(Vector *v) {
    v->capacity = VECTOR_INITIAL_CAPACITY;
    v->size = 0;
    v->items = malloc(sizeof(char *) * v->capacity);
}

int vector_size(Vector *v) {
    return v->size;
}

char *vector_get(Vector *v, int index) {
    if (index >= 0 && index < v->size)
        return v->items[index];
    return NULL;
}

void vector_set(Vector *v, int index, char *item) {
    if (index >= 0 && index < v->size)
        v->items[index] = item;
}

void vector_push(Vector *v, char *item) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->items = realloc(v->items, sizeof(char *) * v->capacity);
    }
    v->items[v->size] = item;
    v->size++;
}

void vector_pop(Vector *v) {
    if (v->size > 0)
        v->size--;
}

void vector_reset(Vector *v) {
    v->size = 0;
}
