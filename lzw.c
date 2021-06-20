#define NUMBER_OF_SYMBOLS 256
#define MAX_NODES 32768
#define MAX_CODES 32768

typdef unsigned short Code;

typedef struct trie {
    trieNode** nodes;
    short num_entries;
    short nodes_size;
} Trie;

typedef struct trieNode {
   Code code;
   short children[NUMBER_OF_SYMBOLS];
} TrieNode;

// sets intiail 256 nodes codes to all single possible characters
// sets root nodes children to correct index values into nodes array
void initTrie(Trie *trie) {
   int i;
   for(i = 0; i < NUMBER_OF_SYMBOLS; i++) {
      trie->nodes[i].code = i;
      trie->nodes[0].children[i] = i+1;
   }
}

short lookup(Trie *trie, short cur_pos_index, short letter) {
   short index = trie->nodes[cur_pos_index].children[letter];
   if(index == NULL)
      return index;
   else
      return NULL; // signal that it found another letter in sequence
}

// reallocs and memsets new array of nodes_size to set all to NULL
// returns new size of array
short resize(Trie *trie) {
   short prev_size = trie->nodes_size;
   // double current size
   trie->nodes = realloc(trie->nodes, sizeof(TrieNode) * trie->nodes_size * 2);
   if(trie->nodes == NULL) {
      fprintf(stderr, "realloc failure in %s at %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
   trie->nodes_size = trie->nodes_size * 2;
   memset(trie->nodes[prev_size], 0, prev_size);
   // memset new memory to 0 starting at previous size index
   // use prev_size for number to fill because doubled

   return 0;
}

void compress(FILE *in, FILE *out ) {
   unsigned short next_code, cur_pos_index;
   int i, c;
   Trie *trie;

   trie = calloc(sizeof(Trie));
   if(trie == NULL) {
      fprintf(stderr, "calloc failure in %s at %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
   // initialize to 512 nodes to start?
   trie->nodes = calloc(sizeof(TrieNode) * NUMBER_OF_SYMBOLS * 2);
   if(trie->nodes == NULL) {
      fprintf(stderr, "calloc failure in %s at %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
   trie->nodes_size = NUMBER_OF_SYMBOLS * 2;
   // Allocate the nodes array and roots
   initTrie(trie);
   next_code = NUMBER_OF_SYMBOLS + 1; // 257
   trie->num_entries = NUMBER_OF_SYMBOLS;
   // After this point that next_code == trie->num_entries + 1
   cur_pos_index = 0; // <- Root

   while ((c = fgetc(in)) != EOF) {
     // lookup should never return NULL on first letter in a sequence
     if(lookup(trie, cur_pos_index, c) == NULL) {
        // children array was NULL -> new sequence found! add it to the Trie
        if(next_code <= MAX_CODES)
           insert(trie, cur_pos_index, c, next_code);
        else {
           RECYCLE DICTIONARY
           break;
        }
        // output code of current node, not new code
        fputc(trie->nodes[cur_pos_index].code, out);
        next_code++;
        // Adjust your current index to root[c].
        cur_pos_index = trie->nodes[0].children[c];
     }
     else {
        // set cur_pos_index to index of next letter in sequence/path
        cur_pos_index = trie->nodes[cur_pos_index].children[c];
     }
   }
   // Write out the newest code.
   fputc(trie->nodes[cur_pos_index].code, out);
}

void insert(Trie *trie, int cur_pos_index, int letter, short next_code) {
   short next_index;
   if(trie->num_entries >= trie->nodes_size) {
      trie->nodes_size = resize(trie);
   }
   next_index = trie->num_entries;
   trie->nodes[cur_pos_index].children[letter] = next_index;
   trie->nodes[next_index].code = next_code;
   trie->num_entries++;
}

void decompress(FILE *in, FILE *out) {
   unsigned short next_code, cur_pos_index;
   int i, c;
   Trie *trie;

   trie = calloc(sizeof(Trie));
   if(trie == NULL) {
      fprintf(stderr, "calloc failure in %s at %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
   // initialize to 512 nodes to start?
   trie->nodes = calloc(sizeof(TrieNode) * NUMBER_OF_SYMBOLS * 2);
   if(trie->nodes == NULL) {
      fprintf(stderr, "calloc failure in %s at %d\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
   }
   trie->nodes_size = NUMBER_OF_SYMBOLS * 2;
   // Allocate the nodes array and roots
   initTrie(trie);
   next_code = NUMBER_OF_SYMBOLS + 1; // 257
   trie->num_entries = NUMBER_OF_SYMBOLS;
   // After this point that next_code == trie->num_entries + 1
   cur_pos_index = 0; // <- Root

   while ((c = fgetc(in)) != EOF) {

   }

}

FILE* fileOpen(const char *fname) {

   FILE *fp = fopen(fname, "r");
   if(fp == NULL) {
      fprintf(stderr, "lzw %s: ", fname);
      perror("");
   }
   return fp;
}

int main(int argc, char *argv[]) {
   FILE *in, *out;
   in = fileOpen(argv[1]);
   out = argv[2];
   if(in == NULL)
      return EXIT_FAILURE;
   else {
      compress(in, out);
   }
}
