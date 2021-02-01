#include <stdio.h>
#include <stdlib.h>
#include "tar.h"

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
