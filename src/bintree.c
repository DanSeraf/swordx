#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "bintree.h"

t_node **createTree(){
	t_node **p = (t_node**) malloc(sizeof(t_node*));
	*p = NULL;
	return p;
}

/* Add word inside the binary tree */ 
void addToTree(t_node **p, char *w, unsigned int o) {
    if (*p == NULL) {
        *p = (t_node *) malloc(sizeof(t_node));
        (*p)->word = strdup(w);
        (*p)->count = o;
        (*p)->left = (*p)->right = NULL;
    } 
    else if ((*p)->count < o)
        addToTree(&(*p)->left, w, o);
    else 
        addToTree(&(*p)->right, w, o);
}

void writeTree(t_node *p, FILE *f) {
    if (p != NULL) {
        writeTree(p->left, f);
        fprintf(f, "%s %d\n", p->word, p->count);
        writeTree(p->right, f);
    }
}

/* Free memory of binary tree */
void destroyTree(t_node *p) {
	if (p != NULL) {
		free(p->word);
		destroyTree(p->left);
		destroyTree(p->right);
		free(p);
	}
}
