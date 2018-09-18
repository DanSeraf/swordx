#ifndef BINTREE_H
#define BINTREE_H

#include "linkedlist.h"

typedef struct t_node {
    char *word;
    unsigned int count; /* occurrences */
    struct t_node *left;
    struct t_node *right;
} t_node;

t_node** createTree();
void addToTree(t_node **root, char *word);    
void addToList(t_node *root, l_list **head);
void treePrint(t_node *root);
void destroyTree(t_node *root);

#endif
