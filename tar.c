#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

//initialize list(tree)
void init(list *l) {
    *l = NULL;
    return;
}


//initialize list(map)
void init_map(map *m) {
    *m = NULL;
    return;
}


//adding the obtained code of a character to the map
void append_map(map *m, char ch, int b[], int n) {
    code_map *nn = (code_map*)malloc(sizeof(code_map));
    if(nn) {
        nn->ch = ch;
        nn->next = NULL;
        nn->f = n;
        nn->code = (int*)malloc(sizeof(int)*n);
        for(int i = 0; i < n; i++) {
            nn->code[i] = b[i];
        }
    }
    else
        return;
    if(!*m) {
        *m = nn;
        return;
    }
    code_map *ptr = *m;
    while(ptr->next) {
        ptr = ptr->next;
    }
    ptr->next = nn;
    return;
}


//function that transforms the link list to a Huffman Tree
void make_tree(list *l) {
    node *p = *l;
    node* nn;
    while(p) {
        nn = (node*)malloc(sizeof(node));
        nn->ch = '#';
        nn->l = p;
        if(p->next) {
            p = p->next;
            nn->r = p;
            nn->f = nn->l->f + nn->r->f;
        }
        p = p->next;
        nn->l->next = NULL;
        nn->r->next = NULL;
        if(!p)
            break;
        if(nn->f <= p->f) {
            nn->next = p;
            p = nn;
        }
        else {
            insert_sorted(&p, nn);
        }
    }
    *l = nn;
    return;
}


//adding a character to the link list if it exists
//updating frequency and sorting for those characters that already exit
void append(list *l, char ch){
    node *ptr = search(*l, ch);

    //for a new character
    if(!ptr) {
        node *nn = (node*)malloc(sizeof(node));
        if(nn){
            nn->l = NULL;
            nn->r = NULL;
            nn->f = 1;
            nn->ch = ch;
        }
        else return;

        nn->next = *l;
        *l = nn;
        return;
    }

    //if the character exists, update its frequency
    (ptr->f)++;

    if(!ptr->next)
        return;
    char t;
    int f1;
    //sorting according to frequency
    while(ptr->next && (ptr->f > ptr->next->f)) {
        t = ptr->ch;
        f1 = ptr->f;
        ptr->ch = ptr->next->ch;
        ptr->f = ptr->next->f;
        ptr->next->ch = t;
        ptr->next->f = f1;
        ptr = ptr->next;
    }
    return;
}


//inserts a node in sorted order(according to frequency)
void insert_sorted(list *l, node* p) {
    node *ptr = *l;
    node *q = NULL;
    if(!p) {
        return;
    }
    while(ptr && ptr->f < p->f) {
        q = ptr;
        ptr = ptr->next;
    }
    p->next = ptr;
    q->next = p;
    return;
}


//searches for a character in a list and returns a pointer to it
node* search(list l, char ch) {
    node *p = l;

    while(p) {
        if(p->ch == ch)
            return p;
        p = p->next;
    }
    return NULL;
}


//generate Huffman Codes for characters and store in a map
void get_code(list l, int a[], int n, map* m) {
    if(l->l) {
        a[n] = 0;
        get_code(l->l, a, n+1, m);

    }
    if(l->r) {
        a[n] = 1;
        get_code(l->r, a, n+1, m);
    }
    if(!l->l && !l->r) {
        append_map(m, l->ch, a, n);
    }
}


//display the codes of all characters
void traverse_map(map m) {
    code_map *p = m;
    while(p) {
        printf("%c ", p->ch);
        for(int i = 0; i < p->f; i++) {
            printf("%d", p->code[i]);
        }
        p = p->next;
        printf("\n");
    }
    return;
}


//display the codes of all characters
void traverse(list l){
    node *p;
    p = l;
    while(p){
        printf("%c ", p->ch);
        printf("%d \n", p->f);
        p = p->next;
    }
    return;
}


//writing the header(Huffman Codes) in the archive
void write_table(char* s[], map m, int n, char name[]) {
    code_map *p = m;
    int count = 0;
    while(p) {
        count++;
        p = p->next;
    }
    p = m;

    FILE *f1 = fopen(name, "w");
    if(!f1) {
        printf("Error\n");
        return;
    }
    fprintf(f1, "%d", count);       //number of distinct characters
    fprintf(f1, "%c", 'c');
    while(p) {
        fprintf(f1, "%d", p->f);
        fprintf(f1, "%c", p->ch);
        for(int i = 0; i < p->f; i++) {
            fprintf(f1, "%d", p->code[i]);
        }
        p = p->next;
    }
    FILE* f;
    for(int i = 0; i < n; i++) {

        f = fopen(s[i], "r");
        encode(f, m, f1);
        fclose(f);
    }
    int off = 0;
    for(int i = 0; i < n; i++) {
        off = off + strlen(s[i]);
    }
    off = off + n;

    for(int i = 0; i < n; i++) {
        fprintf(f1, "%s", s[i]);
        fprintf(f1, "%d", bytes[i]);

    }
    fprintf(f1, "%d", off);
    int c_off = 0;
    while(off > 0) {
        c_off++;
        off = off / 10;
    }
    fprintf(f1, "%d", c_off);
    fclose(f1);
    return;
}


//performs data encryption and writes data into archive
void encode(FILE* f, map m, FILE* f1) {

    char ch;
    code_map *p = NULL;
    int data;
    int count = 0;

    fscanf(f, "%c", &ch);
    while(!feof(f)) {
        p = search_map(m, ch);
        for(int i = 0; i < p->f; i++) {
            data = p->code[i]; //got one digit in 4 byte long variable data
            writeBit(data, f1);
            count++;
            if(count == 8) {
                count = 0;
            }
        }
        fscanf(f, "%c", &ch);
    }
    padding = 8 - count;
    bytes[byteptrs++] = padding;
    for(int k = 0; k < 8 - count; k++) {
        writeBit(0, f1);
    }

    //mark to denote end of contents of one file
    char terminating[] = {'0', '0', '0', '0', '0', '0', '0', '0'};

    fwrite(&terminating,sizeof(char),8,f1);
    return;
}


//makes use of a bit buffer to write data into the archive
void writeBit(int b, FILE *f) {
	static char byte;
	static int cnt = 0;
	char temp;
	if(b == 1) {
        temp = 1;
		temp = temp<<(7-cnt);
		byte = byte | temp;
	}
    cnt++;
	if(cnt==8)	//buffer full
	{
	    if((int)byte == 26) {
            char temp[] = {'0', '0', '0', '1', '1', '0', '1', '0'};
            fwrite(&temp,sizeof(char),8,f);
	    }
	    else {
            fwrite(&byte,sizeof(char),1,f);
	    }
		cnt=0;
		byte=0;
		return;
	}
	return;
}


//reads a bit from the archive and returns it as a byte
int* trying(char ch) {

    static int ans[8] = {};
    int n;
    char one = 1 << 7;
    for(int i = 0; i < 8; i++) {
        n = one & ch;
        if(n == 0) {
            ans[i] = 0;
        }
        else {
            ans[i] = 1;
        }
        ch = ch << 1;
    }
    return ans;
}


//returns a pointer to a character in the map
code_map* search_map(map m, char ch) {
    code_map *p = m;

    while(p) {
        if(p->ch == ch)
            return p;
        p = p->next;
    }
    return NULL;
}


//reads the header of the archive and makes a Huffman Tree
void read_header(FILE* fp, list *l) {

    node *p = *l;
    node* nn = make_blank_node();
    *l = nn;
    p = nn;
    //reading the names of the files to be decoded
    fseek(fp, -1, SEEK_END);
    int d;

    fscanf(fp, "%d", &d);
    fseek(fp, -(1 + d), SEEK_END);
    int offset1 = 0;
    fscanf(fp, "%d", &offset1);
    offset1 = offset1 / 10;

    fseek(fp, -(offset1 + 1 + d), SEEK_END);
    char buffer[500] = {};
    int counter = 0;
    while(!feof(fp)) {
        fscanf(fp, "%c", &buffer[counter]);
        counter++;
    }
    fseek(fp, 0, SEEK_SET);

    int count_files = 0;
    int nme = 0, bt = 0;
    int flag = 0, ptr_nme = 0;

    char name[50]  = {};
    char ch1;
    char* s[100];
    //getting file names that are written at the end of the archive
    for(int i = 0; i < counter; i++) {
        ch1 = buffer[i];
        if((ch1 >= 'A' && ch1 <= 'Z') || (ch1 >= 'a' && ch1 <= 'z') || ch1 == '.') {
            name[nme++] = ch1;
            flag = 1;
        }
        else {
            if(flag) {
                bytes[bt++] = ch1 - '0';
                s[ptr_nme] = (char*)malloc(sizeof(char) * 50);
                strcpy(s[ptr_nme], name);
                merge_nme[merge_pos] = (char*)malloc(sizeof(char) * 20);
                strcpy(merge_nme[merge_pos], name);
                merge_pos++;
                ptr_nme++;
                for(int tempp = 0; tempp < nme; tempp++) {
                    name[tempp] = '\0';
                }
                nme = 0;
                strcpy(name, "");
                flag = 0;
                count_files++;
            }
            else {
                break;
            }
        }
    }
    merge_count += count_files;

    char ch, discard;
    int a[20] = {};
    int i = 0, count = 0;
    char t;
    int  f;
    fscanf(fp, "%d", &count); //number of distinct characters
    fread(&discard, 1, 1, fp); //reading the character c

    while(count) {
        count--;
        i = 0;
        fscanf(fp, "%d", &f);
        fread(&ch, 1, 1, fp);
        for(i = 0; i < f; i++) {

            fread(&t, 1, 1, fp);
            if(t == '0')
                a[i] = 0;
            else
                a[i] = 1;
        }

        node *n = (node*)malloc(sizeof(node));

        n->ch = ch;
        n->l = n->r = NULL;

        int x;
        p = *l;
        //constructing Huffman tree
        for(int j = 0; j < i; j++) {
            x = a[j];
            if(x == 0) {
                if(p->l) {
                    p = p->l;
                }
                else {

                    if(j < i - 1) {
                        p->l = make_blank_node();
                        p = p->l;
                    }
                    else {
                        p->l = n;
                    }
                }
            }
            else {
                if(p->r) {
                    p = p->r;
                }
                else {
                    if(j < i - 1) {
                        p->r = make_blank_node();
                        p = p->r;
                    }
                    else {
                        p->r = n;
                    }
                }
            }
        }
    }

    int pos = 0, val = 0;

    while(val < count_files) {
        fp = decode(fp, *l, s[pos]);
        pos++;
        val++;

    }
    return;
}


//decrypts the file by reading individual bytes and extracting bit information
FILE* decode(FILE* f, list l, char* num) {
    char* name = (char*)malloc(sizeof(char) * 100);
    strcpy(name, "DIR/");
    strcat(name, num);
    FILE *fp = fopen(name, "w");
    if(fp == NULL) {
        printf("Error\n");
        return EOF;
    }

    int count = 0, set_pad = 0;

    int n;
    static char val;
    static char val1;

    node *p = l;
    int* code;
    fread(&val, 1, 1, f);

    //EOF character may occur within a file(encrypted)
    int eof[] = {0,0,0,1,1,0,1,0};

    //file terminating marker
    int terminate[] = {0,0,0,0,0,0,0,0};
    int term = 0;
    while(!term) {
        fread(&val1, 1, 1, f);
        if(val1 == '0') {
            code[0] = val1 - '0';
            for(int j = 1; j < 8; j++) {
                fread(&val1, 1, 1, f);
                code[j] = val1 - '0';
                if(code[j] == terminate[j]) {
                    if(j == 7) {
                        set_pad = bytes[count++];
                        code = trying(val);
                        term = 1;
                        fseek(f, -1, SEEK_CUR);
                        goto last;
                    }
                }
                else {
                    fseek(f, -(j), SEEK_CUR);
                    break;
                }
            }
        }
        fseek(f, -1, SEEK_CUR);

        if(val == '0') {
            code[0] = val - '0';
            for(int j = 1; j < 8; j++) {
                fread(&val, 1, 1, f);
                code[j] = val - '0';
                if(code[j] == eof[j] || code[j] == terminate[j]) {
                    if(j == 7) {
                        int testing;
                        for(testing = 0; testing < 8; testing++) {
                            if(code[testing] != terminate[testing]) {
                                break;
                            }
                        }
                        if(testing == 8) {
                            term = 1;
                            break;
                        }
                    }
                    continue;
                }
                else {
                    fseek(f, -j, SEEK_CUR);
                    code = trying('0');
                    break;
                }
            }
        }
        else {
            code = trying(val);
        }
        if(term == 1) {
            break;
        }
        last:
        for(int i = 0; i < 8 - set_pad; i++) {
            n = *(code + i);
            if(n == 0) {
                p = p->l;
            }
            else {
                p = p->r;
            }
            if(!p->l && !p->r) {
                fprintf(fp, "%c", p->ch);
                p = l;
            }

        }
        fread(&val, 1, 1, f);

    }
    fclose(fp);
    return f;
}


//makes a blank node for a list
node *make_blank_node() {
    node* nn = (node*)malloc(sizeof(node));
    nn->ch = '#';
    nn->l = nn->r = NULL;
    return nn;
}


//to check if the archive is written correctly(test function)
void check_file() {
    printf("\n Checking file\n");
    FILE *deletethis = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");
    static char ch;


    fread(&ch, 1, 1, deletethis);
    while(!feof(deletethis)) {
        printf("%d\n", ch);
        fread(&ch, 1, 1, deletethis);
    }
    fclose(deletethis);
}




