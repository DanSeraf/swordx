#ifndef BINTREE_H
#define BINTREE_H

typedef struct t_node {
    char *word;
    unsigned int count; /* occurrences */
    struct t_node *left;
    struct t_node *right;
} t_node;

t_node** createTree();
void addToTree(t_node **tree_root, char *word, unsigned int o);    
void writeTree(t_node *tree_root, FILE *outfile);
void destroyTree(t_node *tree_root);

#endif
