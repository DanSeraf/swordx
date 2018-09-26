#include <stdio.h>
#include <stdlib.h>
#include "linkedstack.h"

stack ** createStack(){
	stack ** head = (stack **) malloc(sizeof(stack));
	*head = NULL;
	return head;
}

void print_stack(stack * head){
	stack * current = head;

	while(current != NULL){
		printf("%s\n", current->val);
		current=current->next;

	}
}

void push(stack ** head, char * val) {
    stack * new_node;
    new_node = (stack *)malloc(sizeof(stack));
    new_node->val = val;
    new_node->next = (*head);
    (*head) = new_node;
}

char *pop(stack ** head) {
    char *retval;
    stack * next_node = NULL;
    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}