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
	{
		printf("Opening file...\r\n");
		FILE *fp = fopen("challenge.bin", "r");
		if (fp == NULL){
			printf("There was an error while opening the file.\r\n");
			return 1;
		}
		printf("Successfully openend file.\r\n");

		char currentchar;
		unsigned int pos;
		unsigned int input[0xFFFF];
		while(pos <= 0xFFFF){
			currentchar = fgetc(fp);
			if (currentchar == EOF)
				currentchar = 0; 
			input[pos] = ((unsigned int) (unsigned char) currentchar);
			pos++;
		}
		// lets fill the memory
		for(unsigned int i = 0; i < pos; i += 2){
			unsigned short int low = (unsigned short int) input[i];
			unsigned short int high = (unsigned short int) input[i+1];
			storage_memory[i >> 1] = low | (high << 8);
		}

	};
	
	highestStack = CreateStack();

	// now we can implement the opcodes
	unsigned short int programCounter = 0;
	while(true){
		// halt
		if (opCode == 0){
			break;
		}

		// set
		if (opCode == 1){
			*aParam = *bParam;
			programCounter += 3;
			continue;
		}

		// push
		if (opCode == 2){
			PushToStack(highestStack, *aParam);
			programCounter += 2;
			continue;
		}

		// pop
		if (opCode == 3){
			*aParam = highestStack->value;
			highestStack = PopFromStack(highestStack);
			programCounter += 2;
			continue;
		}

		// eq
		if (opCode == 4){
			*aParam = *bParam == *cParam;
			programCounter += 3;
			continue;
		}

		// jmp
		if (opCode == 6){
			programCounter = *aParam;
			continue;
		}

		// jt
		if (opCode == 7){
			if (*aParam != 0)
				programCounter = *bParam;
			else
				programCounter += 3;
			continue;
		}

		// jf
		if (opCode == 8){
			if (*aParam == 0)
				programCounter = *bParam;
			else
				programCounter += 3;
			continue;
		}

		// add
		if (opCode == 9){
			*aParam = *bParam + *cParam;
			programCounter += 4;
			continue;
		}

		// out
		if (opCode == 19){
			printf("%c", *aParam);
			programCounter += 2;
			continue;
		}

		// noop
		if (opCode == 21){
			programCounter += 1;
			continue;
		}

		printf("No opcode was defined for %i\r\n", opCode);
		break;
	}

	return 0;
}