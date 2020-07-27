#include "main.h"
#include "unitTests.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

#define opCode storage_memory[programCounter]
#define aParam (storage_memory[programCounter + 1] < 32567 ? storage_memory + programCounter + 1 : storage_register + storage_memory[programCounter + 1] - 32567)
#define bParam (storage_memory[programCounter + 2] < 32567 ? storage_memory + programCounter + 2 : storage_register + storage_memory[programCounter + 2] - 32567)
#define cParam (storage_memory[programCounter + 3] < 32567 ? storage_memory + programCounter + 3 : storage_register + storage_memory[programCounter + 3] - 32567)

int main(const int argc, const char* argv[]){
    // Small unit tests
    if (!runTests()){
        // At least one test failed
        printf("One test failed. Stopping program now.\r\n");
        return 1;
    }

    // first we have to read the file and store it in the programs memory
    printf("Opening file...\r\n");
    FILE *fp = fopen("../files/challenge.bin", "r");
    if (fp == NULL){
        printf("There was an error while opening the file.\r\n");
        return 1;
    }
    printf("Successfully openend file.\r\n");

    char currentChar;
    unsigned int pos = 0;
    unsigned int input[0xFFFF];
    while(pos <= 0xFFFF){
        currentChar = fgetc(fp);
        if (currentChar == EOF)
            currentChar = 0;
        input[pos] = ((unsigned int) (unsigned char) currentChar);
        pos++;
    }
    // lets fill the memory
    for(unsigned int i = 0; i < pos; i += 2){
        unsigned short int low = (unsigned short int) input[i];
        unsigned short int high = (unsigned short int) input[i+1];
        storage_memory[i >> 1] = low | (high << 8);
    }

    // init empty stack
    highestStack = CreateStack();

    // now we can implement the opcodes
    unsigned short int programCounter = 0;

    while(true){

        // halt: 0 - stop execution and terminate the program
        if (opCode == 0){
            break;
        }

        // set: 1 a b - set register <a> to the value of <b>
        if (opCode == 1){
            *aParam = *bParam;
            programCounter += 3;
            continue;
        }

        // push: 2 a - push <a> onto the stack
        if (opCode == 2){
            highestStack = PushToStack(highestStack, *aParam);
            programCounter += 2;
            continue;
        }

        // pop: 3 a - remove the top element from the stack and write it into <a>; empty stack = error
        if (opCode == 3){
            if (highestStack == NULL){
                printf("Stack is empty. Error...\r\n");
                break;
            }
            *aParam = highestStack->value;
            highestStack = PopFromStack(highestStack);
            programCounter += 2;
            continue;
        }

        // eq: 4 a b c - set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
        if (opCode == 4){
            *aParam = *bParam == *cParam;
            programCounter += 4;
            continue;
        }

        // gt: 5 a b c - set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
        if (opCode == 5){
            *aParam = *bParam > *cParam;
            programCounter += 4;
            continue;
        }

        // jmp: 6 a - jump to <a>
        if (opCode == 6){
            programCounter = *aParam;
            continue;
        }

        // jt: 7 a b - if <a> is nonzero, jump to <b>
        if (opCode == 7){
            if (*aParam != 0)
                programCounter = *bParam;
            else
                programCounter += 3;
            continue;
        }

        // jf: 8 a b - if <a> is zero, jump to <b>
        if (opCode == 8){
            if (*aParam == 0)
                programCounter = *bParam;
            else
                programCounter += 3;
            continue;
        }

        // add: 9 a b c - assign into <a> the sum of <b> and <c> (modulo 32768)
        if (opCode == 9){
            *aParam = (*bParam + *cParam) % 32768;
            programCounter += 4;
            continue;
        }

        // mult: 10 a b c - store into <a> the product of <b> and <c> (modulo 32768)
        if (opCode == 10){
            *aParam = (*bParam * *cParam) % 32768;
            programCounter += 4;
            continue;
        }

        // mod: 11 a b c - store into <a> the remainder of <b> divided by <c>
        if (opCode == 11){
            *aParam = *bParam % *cParam;
            programCounter += 4;
            continue;
        }

        // and: 12 a b c - stores into <a> the bitwise and of <b> and <c>
        if (opCode == 12){
            *aParam = *bParam & *cParam;
            programCounter += 4;
            continue;
        }

        // or: 13 a b c - stores into <a> the bitwise or of <b> and <c>
        if (opCode == 13){
            *aParam = *bParam | *cParam;
            programCounter += 4;
            continue;
        }

        // not: 14 a b - stores 15-bit bitwise inverse of <b> in <a>
        if (opCode == 14){
            *aParam = ~*bParam & 0x7FFF;
            printf("%d\n", *aParam);
            programCounter += 3;
            continue;
        }

        // rmem: 15 a b - read memory at address <b> and write it to <a>
        if (opCode == 15){
            *aParam = storage_memory[*bParam];
            programCounter += 3;
            continue;
        }

        // wmem: 16 a b - write the value from <b> into memory at address <a>
        if (opCode == 16){
            storage_memory[*aParam] = *bParam;
            programCounter += 3;
            continue;
        }

        // call: 17 a - write the address of the next instruction to the stack and jump to <a>
        if (opCode == 17){
            highestStack = PushToStack(highestStack, programCounter += 2);
            programCounter = *aParam;
            continue;
        }

        // ret: 18 - remove the top element from the stack and jump to it; empty stack = halt
        if (opCode == 18){
            if (highestStack == NULL){
                printf("Stack is empty. Error...\r\n");
                break;
            }
            programCounter = highestStack->value;
            highestStack = PopFromStack(highestStack);
            continue;
        }

        // out: 19 a - write the character represented by ascii code <a> to the terminal
        if (opCode == 19){
            printf("%c", *aParam);
            programCounter += 2;
            continue;
        }

        // in: 20 a - read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read
        if (opCode == 20){
            programCounter += 2;
            continue;
        }

        // noop: 21 - no operation
        if (opCode == 21){
            programCounter += 1;
            continue;
        }

        printf("No opcode was defined for %i\r\n", opCode);
        break;
    }

    return 0;
}