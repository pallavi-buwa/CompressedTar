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

    write_table(&fp, m);
    //Some function to write the codes as header of file


    encode(fp, m);
    fclose(fp);
    //int arr[] = {1,0,1,1,1,1,0,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0};
    //decode(arr, l, 29);
    return 0;
}
