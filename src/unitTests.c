#include "unitTests.h"

#include "stack.h"
#include "stdio.h"

#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KWHT "\x1B[37m"

bool testStack(){
	bool noTestFailed = true;

	// create bottom of stack
	struct StackNode* stack = CreateStack();
	if (stack->value == 0)
		printf("%sSuccessfully created an empty stack.\r\n", KGRN);
	else {
		printf("%sError while creating a stack...\r\n", KRED);
		noTestFailed = false;
	}

	// change value of top of stack
	stack->value = 10;
	if (stack->value == 10)
		printf("%sSuccessfully changed value of stack.\r\n", KGRN);
	else {
		printf("%sError while changing the value of the stack...\r\n", KRED);
		noTestFailed = false;
	}

	// push to stack
	stack = PushToStack(stack, 40);
	if (stack->value == 40)
		printf("%sSuccessfully pushed to stack.\r\n", KGRN);
	else {
		printf("%sError while pushing to stack...\r\n", KRED);
		noTestFailed = false;
	}

	// push to stack
	stack = PushToStack(stack, 60);
	if (stack->value == 60)
		printf("%sSuccessfully pushed to stack.\r\n", KGRN);
	else {
		printf("%sError while pushing to stack...\r\n", KRED);
		noTestFailed = false;
	}

	// change value of top of stack
	stack->value = 10;
	if (stack->value == 10)
		printf("%sSuccessfully changed value of stack.\r\n", KGRN);
	else {
		printf("%sError while changing the value of the stack...\r\n", KRED);
		noTestFailed = false;
	}

	// remove top of stack
	stack = PopFromStack(stack);
	if(stack->value == 40)
		printf("%sSuccessfully popped from stack.\r\n", KGRN);
	else {
		printf("%sError while popping from stack...\r\n", KRED);
		noTestFailed = false;
	}

	// remove top of stack
	stack = PopFromStack(stack);
	if(stack->value == 10)
		printf("%sSuccessfully popped from stack.\r\n", KGRN);
	else {
		printf("%sError while popping from stack...\r\n", KRED);
		noTestFailed = false;
	}

	// change value of top of stack
	stack->value = 90;
	if (stack->value == 90)
		printf("%sSuccessfully changed value of stack.\r\n", KGRN);
	else {
		printf("%sError while changing the value of the stack...\r\n", KRED);
		noTestFailed = false;
	}

	// push to stack
	stack = PushToStack(stack, 30);
	if (stack->value == 30)
		printf("%sSuccessfully pushed to stack.\r\n", KGRN);
	else {
		printf("%sError while pushing to stack...\r\n", KRED);
		noTestFailed = false;
	}

	// remove top of stack
	stack = PopFromStack(stack);
	if(stack->value == 90)
		printf("%sSuccessfully popped from stack.\r\n", KGRN);
	else {
		printf("%sError while popping from stack...\r\n", KRED);
		noTestFailed = false;
	}

	return noTestFailed;
}

bool testMemory(){
	bool noTestFailed = true;

	return noTestFailed;
}

bool testRegister(){
	bool noTestFailed = true;

	return noTestFailed;
}

bool testOpCodes(){
	bool noTestFailed = true;

	return noTestFailed;
}

bool runTests(){
	printf("%sStart tests now.\r\n", KWHT);

	// Test of the stack
	printf("\r\n");
	printf("%sStart test of stack...\r\n", KWHT);
	bool testStackPassed = testStack();
	if (testStackPassed)
		printf("%sStack test passed.\r\n", KGRN);
	else
		printf("%sStack test failed...\r\n", KRED);

	// Test of the memory
	printf("\r\n");
	printf("%sStart test of memory...\r\n", KWHT);
	bool testMemoryPassed = testMemory();
	if (testMemoryPassed)
		printf("%sMemory test passed.\r\n", KGRN);
	else
		printf("%sMemory test failed...\r\n", KRED);

	// Test of the registers
	printf("\r\n");
	printf("%sStart test of the register...\r\n", KWHT);
	bool testRegisterPassed = testRegister();
	if (testRegisterPassed)
		printf("%sRegister test passed.\r\n", KGRN);
	else
		printf("%sRegister test failed...\r\n", KRED);

	// Test of the opcodes
	printf("\r\n");
	printf("%sStart test of opcodes...\r\n", KWHT);
	bool testOpCodesPassed = testOpCodes();
	if (testOpCodesPassed)
		printf("%sOpCodes test passed.\r\n", KGRN);
	else
		printf("%sOpCodes test failed...\r\n", KRED);

	printf("\r\n");
	printf("%sFinished all tests.\r\n", KWHT);
	if (testMemoryPassed && testMemoryPassed && testRegisterPassed && testOpCodesPassed)
		printf("%sAll tests passed.\r\n", KGRN);
	else
		printf("%sAt least one test failed...\r\n", KRED);
	printf("%s\r\n", KWHT);

	return testMemoryPassed && testMemoryPassed && testRegisterPassed && testOpCodesPassed;
}