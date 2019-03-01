#ifndef MAIN_H
#define MAIN_H

#define SIZE_REGISTER 8
#define SIZE_MEMORY 32768

/* three storage regions
  - memory with 15-bit address space storing 16-bit values
  - eight registers
  - an unbounded stack which holds individual 16-bit values */
unsigned short int storage_memory[SIZE_MEMORY] = {};
unsigned short int storage_register[SIZE_REGISTER] = {};
struct StackNode* highestStack;

int main(const int argc, const char* argv[]);

#endif