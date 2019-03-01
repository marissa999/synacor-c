#ifndef MAIN_H
#define MAIN_H

#define SIZE_REGISTER 8
#define SIZE_MEMORY 32768

unsigned short int* storage_memory[SIZE_MEMORY] = {};
unsigned short int* storage_register[SIZE_REGISTER] = {};

struct StackNode* highestStack;

int main(const int argc, const char* argv[]);

#endif