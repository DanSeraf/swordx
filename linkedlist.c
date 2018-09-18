#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void headSplit(l_list *head, l_list **halve_a, l_list **halve_b);
l_list *mergeList(l_list *halve_a, l_list *halve_b);

l_list **createList() {
    l_list **h = (l_list **) malloc(sizeof(l_list *));
    *h = NULL;
    return h;
}

void push(l_list **h, char *w, int o) {
    l_list *new = (l_list *) malloc(sizeof(l_list));
    new->word = w;
    new->count = o;
    new->next = *h;
    *h = new;
}

// using merge sort to sort the linked list
void sortByOccurrences(l_list **h) {
    l_list *head = *h;
    l_list *a, *b;
    
    if (head == NULL || head->next == NULL)
        return;
    headSplit(head, &a, &b);
    sortByOccurrences(&a);
    sortByOccurrences(&b);
    *h = mergeList(a, b);
}

// split head in two parts
void headSplit(l_list *h, l_list **a, l_list **b) {
    l_list *pre;
    l_list *nex;
    pre = h;
    nex = h->next;
    
    while (nex != NULL) {
        nex = nex->next;
        if (nex != NULL) {
            pre = pre->next;
            nex = nex->next;
        }
    }
    *a = h;
    *b = pre->next;
    pre->next = NULL;
}

// merge separated parts
l_list *mergeList(l_list *a, l_list *b) {
    l_list *sorted_list = NULL;
    
    if (a == NULL) 
        return(b);
    else if (b == NULL)
        return(a);

    if (a->count >= b->count) {
        sorted_list = a;
        sorted_list->next = mergeList(a->next, b);
    } else {
        sorted_list = b;
        sorted_list->next = mergeList(b->next, a);
    }

    return sorted_list;
}

void printList(l_list *h) {
    while (h != NULL) {
        printf("Word: %s, count: %d\n", h->word, h->count);
        h = h->next;
    }
}

void destroyList(l_list *h) {
    l_list *curr = h;
    l_list *next;
    
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}
