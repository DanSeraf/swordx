#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bintree.h"

struct t_node *addtree(struct t_node *p, char *w) {
    int cond;

    if (p == NULL) {
        p = (struct t_node *) malloc(sizeof(struct t_node));
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;
    else if (cond < 0)
        p->left = addtree(p->left, w);
    else 
        p->right = addtree(p->right, w);
    return p;
}

void treePrint(struct t_node *p) {
    if (p != NULL) {
        treePrint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treePrint(p->right);
    }
}
