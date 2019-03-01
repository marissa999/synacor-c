#include "main.h"
#include "unitTests.h"

#include <stdio.h>

int main(const int argc, const char* argv[]){
	// Small unit tests
	if (!runTests()){
		// At least one test failed
		return 1;
	} else {
		// All tests passed		
	}

	// first we have to read the file and store it in the programs memory

	return 0;
}