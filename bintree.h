#ifndef BINTREE_H
#define BINTREE_H

struct t_node {
    char *word;
    int count; /* occurrences */
    struct t_node *left;
    struct t_node *right;
};

struct t_node *addtree(struct t_node *, char *);    
void treePrint(struct t_node *);

#endif
