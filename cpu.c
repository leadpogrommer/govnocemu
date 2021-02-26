#include "cpu.h"

#include "cpu.h"

ADDR_MODE(a_immediate, 2)
    return cpu->pc + 1;
}

ADDR_MODE(a_absolute, 3)
    return mem_get16(cpu->mem, cpu->pc + 1);
}

ADDR_MODE(a_zeropage, 2)
    return (uint16_t)mem_get(cpu->mem, cpu->pc + 1);
}

ADDR_MODE(a_implied, 1)
    return 0;
}

ADDR_MODE(a_absolute_x, 3)
    return mem_get16(cpu->mem, cpu->pc + 1) + cpu->x;
}

ADDR_MODE(a_absolute_y, 3)
    return mem_get16(cpu->mem, cpu->pc + 1) + cpu->y;
}

ADDR_MODE(a_zeropage_x, 2)
    return (uint16_t)mem_get(cpu->mem, cpu->pc+1) + cpu->x;
}

ADDR_MODE(a_zeropage_y, 2)
    return (uint16_t)mem_get(cpu->mem, cpu->pc+1) + cpu->y;
}

ADDR_MODE(a_indirect, 3)
    return mem_get16(cpu->mem, mem_get16(cpu->mem, cpu->pc + 1));
}

ADDR_MODE(a_indirect_x, 2)
    return  mem_get16(cpu->mem, (mem_get(cpu->mem, cpu->pc + 1) + cpu->x) & 0xFF);
}

ADDR_MODE(a_indirect_y, 2)
    return mem_get16(cpu->mem, mem_get(cpu->mem, cpu->pc + 1)) + cpu->y;
}

ADDR_MODE(a_relative, 2)
    int8_t offset = mem_get(cpu->mem, cpu->pc + 1);
    return cpu->pc + 2 + offset;
// return target position
}



// TODO: !ldx, !stx, !inx, !lda, !clc, !adc, !sta, !bcc, !jmp

INSTRUCTION(i_ldx) {
    cpu->x = mem_get(cpu->mem, operand);
    if(cpu->x == 0)SET_FLAG(FLAG_Z);
    if(((int8_t)cpu->x) < 0)SET_FLAG(FLAG_N);
    else CLEAR_FLAG(FLAG_N);
}

INSTRUCTION(i_stx) {
    mem_set(cpu->mem, operand, cpu->x);
}

INSTRUCTION(i_inx) {
    cpu->x += 1;
    if (cpu->x == 0) SET_FLAG(FLAG_Z);
    if (((int8_t)cpu->x) < 0) SET_FLAG(FLAG_N);
    else CLEAR_FLAG(FLAG_N);
}

INSTRUCTION(i_lda) {
    cpu->a = mem_get(cpu->mem, operand);
    if (cpu->a == 0) SET_FLAG(FLAG_Z);
    if (((int8_t)cpu->a) < 0) SET_FLAG(FLAG_N);
    else CLEAR_FLAG(FLAG_N);
}

INSTRUCTION(i_clc) {
    CLEAR_FLAG(FLAG_C);
}

INSTRUCTION(i_adc) {
    // TODO: make asm implementation
    // TODO: overflow flag
    uint16_t sum = cpu->a + mem_get(cpu->mem, operand) + GET_FLAG(FLAG_C);
    if (sum > 0xFF) {
        sum = (uint8_t)sum;
        SET_FLAG(FLAG_C);
    } else {
        CLEAR_FLAG(FLAG_C);
    }
    cpu->a = sum;
    if (cpu->a == 0) SET_FLAG(FLAG_Z);
    else CLEAR_FLAG(FLAG_Z);
    if (((int8_t)cpu->a) < 0) SET_FLAG(FLAG_N);
    else CLEAR_FLAG(FLAG_N);
}

INSTRUCTION(i_sta) {
    mem_set(cpu->mem, operand, cpu->a);
}

INSTRUCTION(i_bcc) {
    if (!GET_FLAG(FLAG_C)){
        cpu->was_branch = 1;
        cpu->pc = operand;
    }
}

INSTRUCTION(i_jmp) {
    cpu->was_branch = 1;
    cpu->pc = operand;
}

operation_t op(addr_f a, instr_f i, int c){
    operation_t r = {a, i, c};
    return r;
}

cpu_t* create_cpu() {
    cpu_t *ret = malloc(sizeof(cpu_t));
    ret->mem = create_memory();
    ret->sp = 0xFD;

    ret->ops[0x4C] = op(a_absolute, i_jmp, 3);
    ret->ops[0x6C] = op(a_indirect, i_jmp, 5);

    // TODO: weird bcc clocks
    ret->ops[0x90] = op(a_relative, i_bcc, -1);

    ret->ops[0x85] = op(a_zeropage, i_sta, 3);
    ret->ops[0x95] = op(a_zeropage_x, i_sta, 4);
    ret->ops[0x8D] = op(a_absolute, i_sta, 4);
    ret->ops[0x9D] = op(a_absolute_x, i_sta, 5);
    ret->ops[0x99] = op(a_absolute_y, i_sta, 5);
    ret->ops[0x81] = op(a_indirect_x, i_sta, 6);
    ret->ops[0x91] = op(a_indirect_y, i_sta, 6);

    ret->ops[0x69] = op(a_immediate, i_adc, 2);
    ret->ops[0x65] = op(a_zeropage, i_adc, 3);
    ret->ops[0x75] = op(a_zeropage_x, i_adc, 4);
    ret->ops[0x6D] = op(a_absolute, i_adc, 4);
    ret->ops[0x7D] = op(a_absolute_x, i_adc, 4);
    ret->ops[0x79] = op(a_absolute_y, i_adc, 4);
    ret->ops[0x61] = op(a_indirect_x, i_adc, 6);
    ret->ops[0x71] = op(a_indirect_y, i_adc, 5);

    ret->ops[0x18] = op(a_implied, i_clc, 2);

    ret->ops[0xA9] = op(a_immediate, i_lda, 2);
    ret->ops[0xA5] = op(a_zeropage, i_lda, 3);
    ret->ops[0xB5] = op(a_zeropage_x, i_lda, 4);
    ret->ops[0xAD] = op(a_absolute, i_lda, 4);
    ret->ops[0xBD] = op(a_absolute_x, i_lda, 4);
    ret->ops[0xB9] = op(a_absolute_y, i_lda, 4);
    ret->ops[0xA1] = op(a_indirect_x, i_lda, 6);
    ret->ops[0xB1] = op(a_indirect_y, i_lda, 5);

    ret->ops[0xE8] = op(a_implied, i_inx, 2);

    ret->ops[0x86] = op(a_zeropage, i_stx, 3);
    ret->ops[0x96] = op(a_zeropage_y, i_stx, 4);
    ret->ops[0x8E] = op(a_absolute, i_stx, 4);

    ret->ops[0xA2] = op(a_immediate, i_ldx, 2);
    ret->ops[0xA6] = op(a_zeropage, i_ldx, 3);
    ret->ops[0xB6] = op(a_zeropage_y, i_ldx, 4);
    ret->ops[0xAE] = op(a_absolute, i_ldx, 4);
    ret->ops[0xBE] = op(a_absolute_y, i_ldx, 4);

    return ret;
}

void tick(cpu_t* cpu){
    uint8_t opcode = mem_get(cpu->mem, cpu->pc);
    cpu->was_branch = 0;
    cpu->ops[opcode].instruction(cpu, cpu->ops[opcode].addressing_mode(cpu));
    if(!cpu->was_branch){
        cpu->pc += cpu->pc_delta;
    }
}