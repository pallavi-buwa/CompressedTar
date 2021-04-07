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

int padding;
int byteptrs;
int bytes[100];
int merge_count;
char* merge_nme[100];
int merge_pos;

//Huffman coding functions
void init(list *l);
void append(list *l, char ch);
node* search(list l, char ch);
void make_tree(list *l);
void insert_sorted(list *l, node* p);
void get_code(list l, int a[], int n, map* m);
void traverse(list l);
void encode(FILE *f, map m, FILE* f1);
FILE* decode(FILE* f, list l, char* num);
void writeBit(int b,FILE *f);
void write_table(char* s[], map m, int n, char name[]);
char *int2string(int n);
int* trying(char ch);
void read_header(FILE* fp, list *l);
node *make_blank_node();


void init_map(map *m);
void append_map(map *m, char ch, int b[], int n);
void traverse_map(map m);
code_map* search_map(map l, char ch);


void check_file();
