#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trie.h"

static inline int getIndex(int character);
static inline int getChar(int index);

trie *getNode(void) {
    trie *new_node = NULL;
    new_node = (trie *) malloc(sizeof(trie));
    if (new_node) {
        int i;
        new_node->count = 0;
        for (i = 0; i < ALPHA_SIZE; i++)
            new_node->child[i] = NULL;
    }

    return new_node;
}

// Get index from an int according to ascii table
static inline int getIndex(int c) {
    if (c >= 97 && c <= 122) 
        return c - 'a';
    else 
        return c - 21;
}

// Get char from index according to ascii table
static inline int getChar(int i) {
    if (i >= 0 && i <= 27) 
        return i + 'a';
    else 
        return i + 21;
}

void addToTrie(trie *p, const char *w) {
    int level, index;
    int word_len = strlen(w);
    trie *next = p;
    
    for (level = 0; level < word_len; level++) {
        index = getIndex(w[level]);
        if (!next->child[index])
            next->child[index] = getNode();
        next = next->child[index];

    }
    next->count += 1;
}   

void printTrie(trie *p, char word[WORD_SIZE], int level) {
    assert(p != NULL);

    if (p->count != 0) {
        word[level] = '\0';
        printf("%s %d\n", word, p->count);
    }

    int i;
    for (i = 0; i < ALPHA_SIZE; i++) {
        if (p->child[i] != NULL) {
            word[level] = getChar(i);
            printTrie(p->child[i], word, level + 1);
        }
    }
}

void sboTrie(trie *p, t_node **tr, char word[WORD_SIZE], int level) {
    assert(p != NULL);
    
    if (p->count != 0) {
        word[level] = '\0';
        addToTree(tr, word, p->count);
    }

    int i;
    for (i = 0; i < ALPHA_SIZE; i++) {
        if (p->child[i] != NULL) {
            word[level] = getChar(i);
            sboTrie(p->child[i], tr, word, level + 1);
        }
    }
}

void writeTrie(trie *p, char word[WORD_SIZE], int level, FILE *f) {
    assert(p != NULL);

    if (p->count != 0) {
        word[level] = '\0';
        fprintf(f, "%s %d\n", word, p->count);
    }

    int i;
    for (i = 0; i < ALPHA_SIZE; i++) {
        if (p->child[i] != NULL) {
            word[level] = getChar(i);
            writeTrie(p->child[i], word, level + 1, f);
        }
    }
}

void destroyTrie(trie *p) {
    int i;

    if (!p) return;

    for (i = 0; i < ALPHA_SIZE; i++)
        destroyTrie(p->child[i]);

    free(p);
}
