#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedstack.h"

stack **createStack(){
	stack ** head = (stack **) malloc(sizeof(stack));
	*head = NULL;
	return head;
}

bool searchStack(stack *head, char *fn) {
    stack *current = head;

    while(current != NULL) {
        if (!strcmp(current->val, fn)) 
            return true;
        current = current->next;
    }
    return false;
}

void push(stack **head, char *val) {
    stack * new_node;
    new_node = (stack *)malloc(sizeof(stack));
    new_node->val = strdup(val);
    new_node->next = (*head);
    (*head) = new_node;
}
