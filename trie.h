#ifndef TRIE_H
#define TRIE_H

#include "bintree.h"

#define WORD_SIZE 512
#define ALPHA_SIZE 36

typedef struct trie {
    unsigned int count; // occurrences
    struct trie *child[ALPHA_SIZE];
} trie;

trie *getNode(void);
void addToTrie(trie *root, const char *word);
void sboTrie(trie *root, t_node **tree_root, char word[WORD_SIZE], int level);
bool searchTrie(trie *root, const char *word);
void writeTrie(trie *root, char word[WORD_SIZE], int level, FILE *file_name);
void destroyTrie(trie *root);

#endif
