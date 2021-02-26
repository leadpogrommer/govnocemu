#pragma once

#include "stdint.h"
#include "memory.h"

#define ADDR_MODE(name, size) uint16_t (name)(cpu_t* cpu) { cpu->pc_delta = (size);
#define INSTRUCTION(name) void (name)(cpu_t* cpu, uint16_t operand)

#define FLAG_N 0b10000000
#define FLAG_V 0b01000000
#define FLAG_D 0b00001000
#define FLAG_I 0b00000100
#define FLAG_Z 0b00000010
#define FLAG_C 0b00000001

#define GET_FLAG(flag) ((cpu->ps & (flag)) != 0)
#define SET_FLAG(flag) (cpu->ps |= (flag))
#define CLEAR_FLAG(flag) (cpu->ps &= (~(flag)))

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