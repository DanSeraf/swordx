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
void addToTree(t_node **p, char *w, int o) {
    int cond;

    if (*p == NULL) {
        *p = (t_node *) malloc(sizeof(t_node));
        (*p)->word = strdup(w);
        (*p)->count = 1;
        (*p)->left = (*p)->right = NULL;
    } 
	else if ((cond = strcmp(w, (*p)->word)) == 0)
        (*p)->count++;
    else if (cond < 0)
        addToTree(&(*p)->left, w);
    else 
        addToTree(&(*p)->right, w);
}

void writeTree(t_node *p, FILE *f) {
    if (p != NULL) {
        writeTree(p->left, f);
        fprintf(f, "%s %d\n", p->word, p->count);
        writeTree(p->right, f);
    }
    fclose(f);
}

void treePrint(t_node *p) {
    if (p != NULL) {
        treePrint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treePrint(p->right);
    }
}

/* Free memory of binary tree */
void destroyTree(t_node *p) {
	if (p) {
		free(p->word);
		destroyTree(p->left);
		destroyTree(p->right);
		free(p);
	}	
}
