#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int8_t arr[65536];
} memory;

int8_t mem_get(memory* r, uint16_t a);

int16_t mem_get16(memory* r, uint16_t a);

void mem_set(memory* r, uint16_t a, int8_t val);

memory* create_memory();