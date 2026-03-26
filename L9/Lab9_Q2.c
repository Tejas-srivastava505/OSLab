#include <stdio.h>

void translate_address(int logical_address) {
    if (logical_address < 0 || logical_address >= 256) {
        printf("Address %d is out of bounds (0-255).\n", logical_address);
        return;
    }

    int page_number = logical_address / 32;
    int offset = logical_address % 32;

    printf("Logical Address: %d -> Page Number: %d, Offset: %d\n", 
            logical_address, page_number, offset);
}

int main() {
    int addr1 = 204;
    int addr2 = 56;

    printf("Memory Mapping Simulation (Page Size: 32 bytes)\n");
    printf("----------------------------------------------\n");
    
    translate_address(addr1);
    translate_address(addr2);

    return 0;
}