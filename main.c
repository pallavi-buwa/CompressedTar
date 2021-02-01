#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

int main()
{
    char st[20];
    strcpy(st, "");
    printf("Enter a string: ");
    gets(st);
    char ch;
    list l;
    init(&l);
    for(int i = 0; i < 20; i++) {
        ch = st[i];
        if(ch >= 'a' && ch <= 'z') {
            append(&l, ch);
        }
        else
            break;
    }
    make_tree(&l);
    int a[30];
    map m;
    init_map(&m);
    get_code(l, a, 0, &m);
    traverse_map(m);
    encode(st, m);
    //int arr[] = {1,0,1,1,1,1,0,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0};
    //decode(arr, l, 29);
    return 0;
}
