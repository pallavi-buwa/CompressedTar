/*
Stuff I need to do:

1. To make an archive
2. Append files to an existing archive  //OPTIONAL ONLY IF TIME PERMITS
3. Extract files from an archive
4. Compress an archive
5. Merge compressed archives

Possible functions:

make_archive();
extract();
compress();
merge_compressed();
append();                   //OPTIONAL ONLY IF TIME PERMITS



Worst case scenario:
Only implement compression so that's zip and unzip

Target:
Compress files and make an archive. Extract the same

Best case:
All the above features


STAGE 1

Implement Huffman algorithm to compress and decompress a string
Extend to files

*/


typedef struct node{
    char ch;
    int f;
    struct node *next, *l, *r;
}node;

typedef node* list;

typedef struct code_map {
    char ch;
    int f;
    int *code;
    struct code_map* next;
}code_map;

typedef code_map* map;

//Huffman coding functions
void init(list *l);
void append(list *l, char ch);
node* search(list l, char ch);
void make_tree(list *l);
void insert_sorted(list *l, node* p);
void get_code(list l, int a[], int n, map* m);
void traverse(list l);

void init_map(map *m);
void append_map(map *m, char ch, int b[], int n);
void traverse_map(map m);
