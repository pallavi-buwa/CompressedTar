#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

/*
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
    mkdir("DIR");

    //check_file();//file written correctly

    //*************************DECODING**********************************

    FILE* deletethis = fopen("HuffmanFinal.txt", "r");

    list l1;
    init(&l1);

    read_header(deletethis, &l1);
    fclose(deletethis);
    return 0;
}
*/

int main(int argc, char* argv[]) {
    char *ch1 = (char*)malloc(sizeof(char));
    char *com = (char*)malloc(sizeof(char) * 10);
    ch1 = argv[2];
    char ch = ch1[1];
    com = argv[1];
    if(strcmp(com, "tar")) {
        printf("Invalid Command\n");
        exit(1);
    }
    if(ch != 'c' && ch != 'a' && ch != 'e' && ch != 'm') {
        printf("Invalid Parameter\n");
        exit(1);
    }
    if(ch == 'c') {
        if(argc > 4) {
            printf("Too many arguments\n");
            exit(1);
        }

        list l;
        init(&l);

        char name[20] = {};
        char c;
        char* nme[1];
        nme[0] = name;

        strcpy(name, argv[3]);

        FILE* fp = fopen(name, "r");

        if(!fp) {
            printf("Cannot find file\n");
            exit(1);
        }

        fscanf(fp, "%c", &c);
        while(!feof(fp)) {
            append(&l, ch);
            fscanf(fp, "%c", &ch);
        }

        fclose(fp);

        make_tree(&l);

        int a[50];
        map m;
        init_map(&m);
        get_code(l, a, 0, &m);

        write_table(nme, m, 1);

    }
    else if(ch == 'e') {
        if(argc > 4) {
            printf("Too many arguments\n");
            exit(1);
        }
        mkdir("DIR");

        char name[20] = {};
        strcpy(name, argv[3]);

        FILE* fp = fopen(name, "r");

        list l1;
        init(&l1);

        read_header(fp, &l1);
        fclose(fp);
    }
    else if(ch == 'a') {
            printf("here\n");
        char* s[50];
        int n;
        char ch;
        list l;
        init(&l);
        FILE *fp;
        char num = *argv[3];
        n = num - '0';
    printf("%d\n", n);
        int i = 0;
        int pos = 4;
        while(n) {
            n--;
            s[i] = (char*)malloc(sizeof(char) * 20);
            strcpy(s[i], argv[pos++]);
            printf("%s\n", s[i]);
            fp = fopen(s[i], "r");
            i++;
            if(!fp) {
                printf("Cannot open file\n");
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
        n = i;
        i = 0;
        write_table(s, m, n);

    }
    return 0;
}
