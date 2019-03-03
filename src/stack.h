#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct StackNode {
	unsigned short int value;
	struct StackNode* previous;
};

static inline struct StackNode* CreateStack(){
	struct StackNode* stack = (struct StackNode*) malloc(sizeof(struct StackNode));
	stack->previous = 0;
	stack->value = 0;
	return stack;
};

static inline struct StackNode* PushToStack(struct StackNode* stack_top, unsigned short int value){
	struct StackNode* stack = (struct StackNode*) malloc(sizeof(struct StackNode));
	stack->previous = stack_top;
	stack->value = value;
	return stack;
};

static inline struct StackNode* PopFromStack(struct StackNode* stack_top){
	struct StackNode* stack_prev = stack_top->previous;
	free(stack_top);
	return stack_prev;
};

#endif