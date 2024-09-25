#include <stdint.h>
#include "serial.h"

// Defines the MU_IO address
#define MU_IO 0x3F215040

// Function to print a single character to the serial port
int putc(int data) {
    // Cast the MU_IO address to a volatile unsigned int pointer
       *((volatile unsigned int*)MU_IO) = data;
    return data;

}
