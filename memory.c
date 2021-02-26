#include "memory.h"
int8_t mem_get(memory* r, uint16_t a)  {
    return r->arr[a];
}

int16_t mem_get16(memory* r, uint16_t a) {
    return *((uint16_t *)(r->arr + a));
}

void mem_set(memory* r, uint16_t a, int8_t val) {
    r->arr[a] = val;
}

memory* create_memory() {
    return malloc(sizeof(memory));
}
