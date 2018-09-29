#ifndef LINKEDSTACK_H
#define STACK_H

typedef struct stack{
	char *val; // file name
	struct stack * next;
}stack;

stack **createStack();
bool searchStack(stack *head, char *file_name);
void push(stack **head, char *val);
char *pop(stack **head);

#endif
