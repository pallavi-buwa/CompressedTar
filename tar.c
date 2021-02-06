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

    fscanf(f, "%c", &ch);
    while(!feof(f)) {
        p = search_map(m, ch);
        for(int i = 0; i < p->f; i++) {
            data = p->code[i]; //got one digit in 4 byte long variable data
            writeBit(data, f1, 0);
        }
        fscanf(f, "%c", &ch);
    }
    writeBit(0, f1, 1);
    //fclose(f1);
    return;
}

void writeBit(int b, FILE *f, int pass) {
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
		fwrite(&byte,sizeof(char),1,f);
		cnt=0;
		byte=0;
		return;
	}
	if(pass == 1) {     //buffer might be partially filled and eof was reached
        fwrite(&byte,sizeof(char),1,f);
        cnt=0;
		byte=0;
        return;
	}
	return;
}

void decode(FILE* f, list l) {

    FILE *fp = fopen("D:/Semester3/DSA/Programs/HuffmanDecoded.txt", "w");
    if(fp == NULL) {
        printf("Error\n");
        return 1;
    }

    int n, num;
    char val;
    node *p = l;
    int* code;
    int count =0 ;

    //fscanf(f, "%c", &val);
     fread(&val, 1, 1, f);

    while(!feof(f)) {
        //num = (int)val;
        //temp = int2string(num);
        code = trying(val);
        for(int i = 0; i < 8; i++) {
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
        //fscanf(f, "%c", &val);
        fread(&val, 1, 1, f);

        //printf("%d", count++);
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

    FILE *f1 = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "w");
    if(!f1) {
        printf("Error\n");
        return;
    }

    while(p) {
        fprintf(f1, "%c", p->ch);
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



