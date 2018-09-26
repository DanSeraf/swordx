#ifndef BINTREE_H
#define BINTREE_H

typedef struct t_node {
    char *word;
    unsigned int count; /* occurrences */
    struct t_node *left;
    struct t_node *right;
} t_node;

t_node** createTree();
void addToTree(t_node **root, char *word);    
void writeTree(t_node *root, FILE *outfile);
void treePrint(t_node *root);
void destroyTree(t_node *root);

#endif
