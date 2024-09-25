#include <stdint.h>
#include "serial.h"

// Define the memory-mapped IO register address
#define MU_IO 0x3F215040

// Function to print a single character to the serial port
int putc(int data) {
    // Cast the MU_IO address to a volatile unsigned int pointer
   // volatile unsigned int* mu_io = (volatile unsigned int*)MU_IO;
       *((volatile unsigned int*)MU_IO) = data;
    return data;

    // Write the character to the MU_IO register
   // *mu_io = (unsigned int)data;
}
