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


To do: Really need a way to encode the codes

Okay...weird thing...storing codes as either characters or integers makes no difference.
Yup...double checked that.

So..lets skip this for now shall we?
(Hopefully it'll get balanced for super big files Kb type)

Lets try to include other characters now....
Whoa..done..that was quick...
Turns out I had not included a lowercase condition...lol...

OKAY
Next step: Check the program on a big file.
It works!!!!

Bad news: The content generated with and without the header is different.
            This is a serious problem...

            Lets try this - decode the file without header and see if you get the output.
            This is exhausting.
            So...it works...kind of
            The last character does not get encoded...probably something to do with the padding.
            Yup...padding..that's the problem.

Okay so here is what I have figured out after my tantrum:
        something something... 11001010100111 so that's 14 bits...padding needed - 2 bits.
        Solution: Insert a padding variable to store this count and while writing in the file
        11001010100111002
        So, while decoding, read the last byte which gives you number of bits of paddind

Done....the file is getting compressed and decompressed. Padding bits are not an issue because there will be
no corresponding path in the tree.

Now...all of this is without writing the map in the header.

Next step: Write the map in the header
           Rewrite code for decompression by dynamically generating the tree

Ohhh..wait...padding is an issue...
Damn this is messy
So I have the padding value and now we have got to make a function to skip padding  bits

Done...for now I'm storing padding in a global variable.

Time to write the header
Format : letter code letter code......

Problem: Gotta store padding as the last byte of the file

Okay..we're making good progress...now we gotta store number of characters as the first byte of the file

Big files dont work..small ones do..no clue what the problem is

Okay Okay...the header is correct..reading is the issue...fix that


Whoa..lots of changes

Finally : 64c(total number of characters)numofdigitscharacter....

READ the last bit of the file!!!!! you've assigned it to padding

THIS IS MAD

SO....ASCII for EOF is 26..which is cropping up in your program
So in decode() you have to find a way to continue the loop if EOF is encountered.
And that double pointer business...check that...

remove trail
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

int padding, bytes;

//Huffman coding functions
void init(list *l);
void append(list *l, char ch);
node* search(list l, char ch);
void make_tree(list *l);
void insert_sorted(list *l, node* p);
void get_code(list l, int a[], int n, map* m);
void traverse(list l);
void encode(FILE *f, map m, FILE* f1);
void decode(FILE* f, list l, FILE* f1);
void writeBit(int b,FILE *f);
void write_table(FILE* f, map m);
char *int2string(int n);
int* trying(char ch);
void read_header(FILE* fp, list *l, FILE* trail);
node *make_blank_node();
int count_leaf(list l); //mostly not needed

void init_map(map *m);
void append_map(map *m, char ch, int b[], int n);
void traverse_map(map m);
code_map* search_map(map l, char ch);


void check_file();
