#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "bintree.h"

// Add word inside the struct using the 
struct t_node *addToTree(struct t_node *p, char *w) {
    int cond;

    if (p == NULL) {
        p = (struct t_node *) malloc(sizeof(struct t_node));
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;
    else if (cond < 0)
        p->left = addToTree(p->left, w);
    else 
        p->right = addToTree(p->right, w);
    return p;
}

// Scan file to add word inside the binary tree 
struct t_node *scanFile(FILE *f, struct t_node *root) {
    char *word;
    int n;
    errno = 0;
    do {
        n = fscanf(f, "%ms", &word);
        if (n == 1) {
            printf("Word: %s\n", word);
            root = addToTree(root, word);
            free(word);
        } else if (errno != 0)
            perror("Error in scanf");
    } while (n != EOF);
    fclose(f);
    return root;
}

void treePrint(struct t_node *p) {
    if (p != NULL) {
        treePrint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treePrint(p->right);
    }
}
