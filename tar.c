#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

void init(list *l) {
    *l = NULL;
    return;
}

void init_map(map *m) {
    *m = NULL;
    return;
}

void append_map(map *m, char ch, int b[], int n) { //add top later to insert in O(1)
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
        nn->l->next = NULL; //check these two
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

void append(list *l, char ch){
    node *ptr = search(*l, ch);
    if(!ptr) {          //if not found
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

    (ptr->f)++;

    if(!ptr->next)
        return;
    char t;
    int f1;
    while(ptr->next && (ptr->f > ptr->next->f)) { //change this- swapping may not be needed
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
    p->next = ptr;          //q could become NULL- check this in dry run
    q->next = p;
    return;
}

node* search(list l, char ch) {
    node *p = l;

    while(p) {
        if(p->ch == ch)
            return p;
        p = p->next;
    }
    return NULL;
}

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
        /*printf("%c ", l->ch);
        for(int i = 0; i < n; i++) {
            printf("%d", a[i]);
        }
        printf("\n");*/
    }
}

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

void encode(FILE* f, map m, FILE* f1) {
    /*FILE *f1 = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "a");
    if(!f1) {
        printf("Error\n");
        return;
    }*/

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
        printf("%c", ch);
    }
    padding = 8 - count;
    for(int k = 0; k < 8 - count; k++) {
        writeBit(0, f1);
    }
    fprintf(f1, "%d", padding);

    //printf("Padding:%d", padding);
    fclose(f1);
    return;
}

void writeBit(int b, FILE *f) {
	static char byte;
	static int cnt = 0;
	char temp;
	//printf("\nSetting %dth bit = %d of %d ",cnt,b,byte);
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
		//fflush(f);
		printf("\nWritten byte %d ",byte);
		bytes++;
		cnt=0;
		byte=0;
		return;
	}
	return;
}

void decode(FILE* f, list l, FILE* f1) {

    FILE *fp = fopen("D:/Semester3/DSA/Programs/HuffmanDecoded.txt", "w");
    if(fp == NULL) {
        printf("Error\n");
        return;
    }
    //FILE* f1 = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");
    FILE* f2 = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");
    fseek(f2, -1, SEEK_END);

    int offset;
    fscanf(f2, "%d", &offset);
    fseek(f2, 0, SEEK_SET);
    int count = 0;
    char test, test1;

   /* while(true) {
        fread(&test, 1, 1, f2);
        fread(&test1, 1, 1, f2);
    }*/


    //int offset = padding;
    //fseek(f, -1, SEEK_END);
    //fscanf(f, "%d", &offset);
    //fseek(f, 0, SEEK_SET);
    //char final_byte;
    //fscanf(f, "%c", &final_byte); //reaches end of file
    //printf("Obtained value of padding: %d\n", offset);
    //fseek(f, 0, SEEK_SET); //set to beginning
    //fseek(f, 0, SEEK_END);
    //int len = ftell(f);

    //fprintf(fp, "%d", len);
    //fseek(f, 0, SEEK_SET);

    int n, num;
    static char val;
    static char check;
    static char check1;
    static char check2;

    fread(&check2, 1, 1, f1);
    node *p = l;
    int* code;
    int final_bits = 0, stop = 0;
    fread(&val, 1, 1, f);
    //fread(&check2, 1, 1, f1);
    //fread(&check2, 1, 1, f1);


    int eof[] = {0,0,0,1,1,0,1,0};
    while(count < bytes) {
        count++;
        /*if(feof(f)) {
            count -- ;
        }*/
        //num = (int)val;
        //temp = int2string(num);
        if(val == '0') {
            code[0] = val - '0';
            for(int j = 1; j < 8; j++) {
                fread(&val, 1, 1, f);
                code[j] = val - '0';
                if(code[j] == eof[j]) {
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
        for(int i = 0; i < 8 - final_bits; i++) {
            //n = (int)temp[i];
            n = *(code + i);
            if(n == 0) {
                p = p->l;
            }
            else {
                p = p->r;
            }
            if(!p->l && !p->r) {
                printf("%c", p->ch);
                fprintf(fp, "%c", p->ch);
                p = l;
            }

        }
        /*if(stop) {
            fclose(fp);
            return;
        }*/

        //fread(&check2, 1, 1, f1);
        fread(&val, 1, 1, f);

        //PUT PRINTF HERE TO CHECK VALUES BEING READ
        //printf("VAl value:%d\n", val);
        //fread(&check, 1, 1, f);
        //fread(&check1, 1, 1, f);

        /*if(feof(f)) {
            final_bits = offset;
            stop = 1;
        }*/
        //fseek(f, -2, SEEK_CUR);

    }
    fclose(fp);
    return;
}


char *int2string(int n) {
    int i,k,and,j;
    char *temp=(char *)malloc(16*sizeof(char));
    j=0;

    for(i=15;i>=0;i--)
    {
        and=1<<i;
        k=n & and;
        if(k==0) temp[j++]='0'; else temp[j++]='1';
    }
    //temp[j]='\0';
    return temp;
}

int* trying(char ch) {

    printf("Obtained byte: %d\n", ch);
    static int ans[8] = {};
    int n;
    char one = 1 << 7;
    for(int i = 0; i < 8; i++) {
        n = one & ch;
        if(n == 0) {
            ans[i] = 0;
            //printf("%d", ans[i]);
        }
        else {
            ans[i] = 1;
            //printf("%d", ans[i]);
        }
        ch = ch << 1;
    }
    /*for(int i = 0; i < 8; i++) {
    printf("%d ", ans[i]);
    }*/
    return ans;
}


code_map* search_map(map m, char ch) {
    code_map *p = m;

    while(p) {
        if(p->ch == ch)
            return p;
        p = p->next;
    }
    return NULL;
}


void write_table(FILE* f, map m) {

    code_map *p = m;
    int count = 0;
    while(p) {
        count++;
        p = p->next;
    }
    p = m;

    FILE *f1 = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "w");
    if(!f1) {
        printf("Error\n");
        return;
    }
    fprintf(f1, "%d", count);       //number of distinct characters
    fprintf(f1, "%c", 'c');
    while(p) {
        fprintf(f1, "%d", p->f);
        fprintf(f1, "%c", p->ch);
        //printf("%c: %d\n", p->ch, p->f);
        for(int i = 0; i < p->f; i++) {
            fprintf(f1, "%d", p->code[i]);
            /*if(p->code[i]) {
                fprintf(f1, "%c", '1');
            }
            else {
                fprintf(f1, "%c", '0');
            }*/
        }
        p = p->next;
    }
    encode(f, m, f1);
    fclose(f1);
    return;
}


void read_header(FILE* fp, list *l, FILE* trail) {
    node *p = *l;
    node* nn = make_blank_node();
    *l = nn;
    p = nn;

    char ch, discard, discard1;
    int a[20] = {};
    int i = 0, count = 0, count1;
    char t;
    int  f;
    fscanf(fp, "%d", &count); //number of distinct characters
    fread(&discard, 1, 1, fp);

    fscanf(trail, "%d", &count1);
    fread(&discard1, 1, 1, trail);

    while(count) {
        count--;
        i = 0;
        //fread(&f, 1, 1, fp);
        fscanf(fp, "%d", &f);
        fscanf(trail, "%d", &count1);

        fread(&ch, 1, 1, fp);
        fread(&discard1, 1, 1, trail);

        printf("%c ", ch);

        printf(" %d \n", f);


        for(i = 0; i < f; i++) {

            fread(&t, 1, 1, fp);
            fread(&discard1, 1, 1, trail);
            if(t == '0')
                a[i] = 0;
            else
                a[i] = 1;
            //printf("%d ", a[i]);
        }
        //printf("\n");

        /*while(a[i] == 0 || a[i] == 1) {

            printf("%d ", a[i]);
            i++;
            fread(&t, 1, 1, fp);

            if(t != '1' && t != '0') {
                break;
            }
            if(t == '0')
                a[i] = 0;
            else
                a[i] = 1;

        }

        printf("\n");
        fseek(fp, -1, SEEK_CUR);*/

        node *n = (node*)malloc(sizeof(node));

        n->ch = ch;
        //printf("character in node: %c\n", n->ch);
        n->l = n->r = NULL;

        int x;
        p = *l;
        for(int j = 0; j < i; j++) {
            x = a[j];
            //printf("x: %d ", x);
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
                        printf("inserted leaf %c\n", p->l->ch);
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
                        printf("inserted leaf %c\n", p->r->ch);
                    }
                }
            }
        }
    }

    decode(fp, *l, trail);

    return;
}


node *make_blank_node() {
    node* nn = (node*)malloc(sizeof(node));
    nn->ch = '#';
    nn->l = nn->r = NULL;
    return nn;
}

int count_leaf(list l) {
    node *p = l;
    if(l == NULL) {
        return 0;
    }
    if(!p->l && !p->r) {
        return 1;
    }
    return (count_leaf(p->l) + count_leaf(p->r));
}

void check_file() {
    printf("\n Checking file\n");
    FILE *deletethis = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");
    static char ch;

    //fscanf(deletethis, "%c", &ch);
    fread(&ch, 1, 1, deletethis);
    while(!feof(deletethis)) {
        printf("%d\n", ch);
        //fscanf(deletethis, "%c", &ch);
        fread(&ch, 1, 1, deletethis);
    }
    fclose(deletethis);
}


void decode_try(list l) {

    FILE* f;
    char* buffer;
    long numbytes;

    f = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "rb");


    if(f == NULL)
        return 1;


    fseek(f, 0L, SEEK_END);
    numbytes = ftell(f);


    fseek(f, 0L, SEEK_SET);

    buffer = (char*)calloc(numbytes, sizeof(char));

    if(buffer == NULL)
        printf("empty buffer\n");

    fread(buffer, sizeof(char), numbytes, f);
    fclose(f);
    node *p = l;
    int* code;
    int n, stop = 0, final_bits = 0;
    printf("buffer\n");
    for(int i = 0; i < numbytes; i++) {
        printf("%c", buffer[i]);
    }

    /*for(int i = 0; i < numbytes; i++) {
            code = trying(buffer[i]);
        for(int j = 0; j < 8 - final_bits; j++) {
            n = *(code + i);
            if(n == 0) {
                p = p->l;
            }
            else {
                p = p->r;
            }
            if(!p->l && !p->r) {
                printf("%c", p->ch);
                fprintf(fp, "%c", p->ch);
                p = l;
            }

        }
        if(stop) {
            fclose(fp);
            return;
        }

        if(feof(f)) {
            final_bits = offset;
            stop = 1;
        }
        //fseek(f, -2, SEEK_CUR);

    }
    }

     free the memory we used for the buffer */
    free(buffer);

}

