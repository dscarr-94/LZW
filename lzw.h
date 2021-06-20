#ifndef LZW_H
#define LZW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <libgen.h>

#define NUMBER_OF_SYMBOLS 256
#define MAX_NODES 32768
#define MAX_CODES 32768

typedef unsigned short Code;

typedef struct trieNode {
   Code code;
   short children[NUMBER_OF_SYMBOLS];
} TrieNode;

typedef struct trie {
    TrieNode* nodes;
    short num_entries;
    short nodes_size;
} Trie;

void insert(Trie *trie, int cur_pos_index, int letter, short next_code);
void initTrie(Trie *trie);
short lookup(Trie *trie, short cur_pos_index, short letter);
short resize(Trie *trie);
void compress(FILE *in, FILE *out);
FILE* fileOpen(const char *fname);

#endif
