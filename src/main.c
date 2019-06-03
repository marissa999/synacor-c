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

        printf( "\npc: %d, current opcode: %d\n"
                "Memory address a: %p, Real Value: %d, Register: %s, value: %d\n"
                "Memory address b: %p, Real Value: %d, Register: %s, value: %d\n"
                "Memory address c: %p, Real Value: %d, Register: %s, value: %d\n",
                programCounter, opCode,
                aParam, storage_memory[programCounter + 1], storage_memory[programCounter + 1] < 32567 ? "No" : "Yes", *aParam,
                bParam, storage_memory[programCounter + 2], storage_memory[programCounter + 2] < 32567 ? "No" : "Yes", *bParam,
                cParam, storage_memory[programCounter + 3], storage_memory[programCounter + 3] < 32567 ? "No" : "Yes", *cParam);

        // halt: 0 - stop execution and terminate the program
        if (opCode == 0){
            printf("halt: 0 - stop execution and terminate the program\n");
            break;
        }

        // set: 1 a b - set register <a> to the value of <b>
        if (opCode == 1){
            printf("set: 1 a b - set register <a> to the value of <b>\n");
            *aParam = *bParam;
            programCounter += 3;
            continue;
        }

        // push: 2 a - push <a> onto the stack
        if (opCode == 2){
            printf("push: 2 a - push <a> onto the stack\n");
            highestStack = PushToStack(highestStack, *aParam);
            programCounter += 2;
            continue;
        }

        // pop: 3 a - remove the top element from the stack and write it into <a>; empty stack = error
        if (opCode == 3){
            printf("pop: 3 a - remove the top element from the stack and write it into <a>; empty stack = error\n");
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
            printf("eq: 4 a b c - set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise\n");
            *aParam = *bParam == *cParam;
            programCounter += 4;
            continue;
        }

        // gt: 5 a b c - set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
        if (opCode == 5){
            printf("gt: 5 a b c - set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise\n");
            *aParam = *bParam > *cParam;
            programCounter += 4;
            continue;
        }

        // jmp: 6 a - jump to <a>
        if (opCode == 6){
            printf("jmp: 6 a - jump to <a>\n");
            programCounter = *aParam;
            continue;
        }

        // jt: 7 a b - if <a> is nonzero, jump to <b>
        if (opCode == 7){
            printf("jt: 7 a b - if <a> is nonzero, jump to <b>\n");
            if (*aParam != 0)
                programCounter = *bParam;
            else
                programCounter += 3;
            continue;
        }

        // jf: 8 a b - if <a> is zero, jump to <b>
        if (opCode == 8){
            printf("jf: 8 a b - if <a> is zero, jump to <b>\n");
            if (*aParam == 0)
                programCounter = *bParam;
            else
                programCounter += 3;
            continue;
        }

        // add: 9 a b c - assign into <a> the sum of <b> and <c> (modulo 32768)
        if (opCode == 9){
            printf("add: 9 a b c - assign into <a> the sum of <b> and <c> (modulo 32768)\n");
            *aParam = (*bParam + *cParam) % 32768;
            programCounter += 4;
            continue;
        }

        // mult: 10 a b c - store into <a> the product of <b> and <c> (modulo 32768)
        if (opCode == 10){
            printf("mult: 10 a b c - store into <a> the product of <b> and <c> (modulo 32768)\n");
            *aParam = (*bParam * *cParam) % 32768;
            programCounter += 4;
            continue;
        }

        // mod: 11 a b c - store into <a> the remainder of <b> divided by <c>
        if (opCode == 11){
            printf("mod: 11 a b c - store into <a> the remainder of <b> divided by <c>\n");
            *aParam = *bParam % *cParam;
            programCounter += 4;
            continue;
        }

        // and: 12 a b c - stores into <a> the bitwise and of <b> and <c>
        if (opCode == 12){
            printf("and: 12 a b c - stores into <a> the bitwise and of <b> and <c>\n");
            *aParam = *bParam & *cParam;
            programCounter += 4;
            continue;
        }

        // or: 13 a b c - stores into <a> the bitwise or of <b> and <c>
        if (opCode == 13){
            printf("or: 13 a b c - stores into <a> the bitwise or of <b> and <c>\n");
            *aParam = *bParam | *cParam;
            programCounter += 4;
            continue;
        }

        // not: 14 a b - stores 15-bit bitwise inverse of <b> in <a>
        if (opCode == 14){
            printf("not: 14 a b - stores 15-bit bitwise inverse of <b> in <a>\n");
            *aParam = ~*bParam & 0x7FFF;
            printf("%d\n", *aParam);
            programCounter += 3;
            continue;
        }

        // rmem: 15 a b - read memory at address <b> and write it to <a>
        if (opCode == 15){
            printf("rmem: 15 a b - read memory at address <b> and write it to <a>\n");
            *aParam = storage_memory[*bParam];
            programCounter += 3;
            continue;
        }

        // wmem: 16 a b - write the value from <b> into memory at address <a>
        if (opCode == 16){
            printf("wmem: 16 a b - write the value from <b> into memory at address <a>\n");
            storage_memory[*aParam] = *bParam;
            programCounter += 3;
            continue;
        }

        // call: 17 a - write the address of the next instruction to the stack and jump to <a>
        if (opCode == 17){
            printf("call: 17 a - write the address of the next instruction to the stack and jump to <a>\n");
            highestStack = PushToStack(highestStack, programCounter += 2);
            programCounter = *aParam;
            continue;
        }

        // ret: 18 - remove the top element from the stack and jump to it; empty stack = halt
        if (opCode == 18){
            printf("ret: 18 - remove the top element from the stack and jump to it; empty stack = halt\n");
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
            printf("out: 19 a - write the character represented by ascii code <a> to the terminal\n");
            printf("%c", *aParam);
            programCounter += 2;
            continue;
        }

        // in: 20 a - read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read
        if (opCode == 20){
            printf("in: 20 a - read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read\n");
            programCounter += 2;
            continue;
        }

        // noop: 21 - no operation
        if (opCode == 21){
            printf("noop: 21 - no operation\n");
            programCounter += 1;
            continue;
        }

        printf("No opcode was defined for %i\r\n", opCode);
        break;
    }

    return 0;
}