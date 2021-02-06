#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

int main()
{

    FILE *fp;
    fp = fopen("D:/Semester3/DSA/Programs/HuffmanTest.txt", "r");
    if(!fp) {
        printf("Error\n");
        return 1;
    }

    char ch;
    list l;
    init(&l);

    fscanf(fp, "%c", &ch);
    while(!feof(fp)) {
        append(&l, ch);
        fscanf(fp, "%c", &ch);
    }
    fclose(fp);

    make_tree(&l);
    int a[30];
    map m;
    init_map(&m);
    get_code(l, a, 0, &m);
    traverse_map(m); //map aka linked list of codes obtained

    fp = fopen("D:/Semester3/DSA/Programs/HuffmanTest.txt", "r");
    FILE* deletethis = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "w");
    //write_table(fp, m);
    encode(fp, m, deletethis);

    fclose(fp);
    fclose(deletethis);
    deletethis = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");
    decode(deletethis, l);
    return 0;
}
