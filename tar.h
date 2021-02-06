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

Done yaaayyyyyyy!!!!! Implement Huffman algorithm to compress and decompress a string

Extend to files :
So....let's do this!
For now, lets keep a file fixed.

Okay, so the file gets compressed...great!
Now, the thing is, we need to add the mapping table as a header in the file itself.
So when you start to decompress, you first read that table and then decode the whole file.
Time to do that!

Evening status update: Added the header as char ch and int* code.
                        So if the file size is really small, the final one ends up being larger than
                        the original file cause of the int codes.
                        So...what to do now?
                        Could try to encode the codes in some way...that seems tricky cause we will have to store the
                        size of the code to read only those many numbers...rest will have to be 0 in the
                        buffer..and the size may go beyond 8 bits so that's another headache.
                        Hmmmm...
                        Also.....we still have to add capital letters, numbers, spaces and special characters.
                        Damn.
                        Really need some form of encoding for codes.
                        Tried storing as a char array...recheck that.
Okay lets do the math:
                        a-z : 26
                        A-Z : 26
                        1-9 : 10
                        Lets keep only . for now
                        So..that is..63 characters!!!

Conclusion: Really need a way to encode the codes

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
void encode(FILE *f, map m);
void decode(int s[], list l, int size);
void writeBit(int b,FILE *f);
void write_table(FILE **fp, map m);

void init_map(map *m);
void append_map(map *m, char ch, int b[], int n);
void traverse_map(map m);
code_map* search_map(map l, char ch);
