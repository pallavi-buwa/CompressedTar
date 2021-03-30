#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

int main()
{
    char* s[50];
    int n;
    char ch;
    list l;
    init(&l);
    FILE *fp;
    printf("Number of files to be compressed: ");
    scanf("%d", &n);
    getc(stdin);
    int i = 0;

    while(n) {
        n--;
        s[i] = (char*)malloc(sizeof(char) * 20);
        printf("Name of the file to the compressed: ");
        gets(s[i]);
        fp = fopen(s[i], "r");
        i++;
        if(!fp) {
            printf("Error\n");
            return 1;
        }

        fscanf(fp, "%c", &ch);
        while(!feof(fp)) {
            append(&l, ch);
            fscanf(fp, "%c", &ch);
        }
        fclose(fp);
    }

    make_tree(&l);
    int a[50];
    map m;
    init_map(&m);
    get_code(l, a, 0, &m);
    //traverse_map(m); //map aka linked list of codes obtained
    n = i;
    i = 0;
    write_table(s, m, n);

    //check_file();//file written correctly

    //*************************DECODING**********************************

    FILE* deletethis = fopen("D:/Semester3/DSA/Programs/HuffmanFinal.txt", "r");

    list l1;
    init(&l1);

    read_header(deletethis, &l1);
    fclose(deletethis);
    return 0;
}
