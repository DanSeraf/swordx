typedef struct stack{
	char *val;
	struct stack * next;
}stack;

void print_stack(stack * head);
void push(stack ** head, char * val);
stack ** createStack();
char *pop(stack ** head);