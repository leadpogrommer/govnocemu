#include <stdio.h>
#include "cpu.h"

int main() {
    cpu_t* cpu = create_cpu();
    FILE *cart = fopen("../max.bin", "rb");
    fread(cpu->mem->arr + 0xF000, 1, 0x1000, cart);
    fclose(cart);

    cpu->pc = 0xF000;

    for(int i = 0; i < 1000; i++){
        tick(cpu);
    }

    FILE *dump = fopen("mem.bin", "wb");
    fwrite(cpu->mem->arr, 1, 0xFFFF, dump);
    fclose(dump);



    return 0;
}
