#ifndef TRIE_H
#define TRIE_H

#define WORD_SIZE 512
// 26 english alphabetic characters plus 10 for numbers
#define ALPHA_SIZE 36

typedef struct trie {
    int count;
    struct trie *child[ALPHA_SIZE];
} trie;

trie *getNode(void);
void addToTrie(trie *root, const char *word);
void printTrie(trie *root, char word[WORD_SIZE], int level);
void writeTrie(trie *root, char wrod[WORD_SIZE], int level, FILE *file_name);
void destroyTrie(trie *root);

#endif
