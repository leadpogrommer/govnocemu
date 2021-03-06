#pragma once

#include <stdint.h>
#include <stdio.h>
#include "memory.h"



typedef struct _cpu_t cpu_t;

typedef uint16_t (*addr_f)(cpu_t *);
typedef void (*instr_f)(cpu_t *, uint16_t);


typedef struct {
    addr_f addressing_mode;
    instr_f instruction;
    int clocks;
}operation_t;

typedef struct _cpu_t{
    uint8_t x, y, a, ps, sp;
    uint16_t pc;
    memory* mem;
    uint8_t pc_delta;
    int was_branch;
    operation_t ops[256];
} cpu_t;





cpu_t* create_cpu();

void tick(cpu_t* cpu);