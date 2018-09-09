#ifndef BINTREE_H
#define BINTREE_H

struct t_node {
    char *word;
    int count; /* occurrences */
    struct t_node *left;
    struct t_node *right;
};

struct t_node *addToTree(struct t_node *root, char *word);    
struct t_node *scanFile(FILE *file, struct t_node *root);
void treePrint(struct t_node *);

#endif
