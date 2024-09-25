#include <stdint.h>
#include "rprintf.h"
#include "serial.h"

// Function to clear the BSS segment
void clear_bss(void) {
    extern uint8_t __bss_start;
    extern uint8_t __bss_end;
    
    uint8_t *bss = &__bss_start;
    uint8_t *bss_end = &__bss_end;

    while (bss < bss_end) {
        *bss = 0;
        bss++;
    }
}

int getEL(void) {
    int el;
    asm volatile ("mrs %0, CurrentEL" : "=r" (el));
    return el >> 2;
}

void kernel_main(void) {

    esp_printf(putc, "Current Execution Level is %d\r\n", getEL());

    while(1) {
        
    }
}
