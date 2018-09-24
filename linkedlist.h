#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct l_list {
    char *word;
    int count;
    struct l_list *next;
} l_list;

l_list **createList();
void push(l_list **head, char *word, int occurrences);
void sortByOccurrences(l_list **head);
void writeList(l_list *head, FILE *f);
void printList(l_list *head);
void destroyList(l_list *head);

#endif
