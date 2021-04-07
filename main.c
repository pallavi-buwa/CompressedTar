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
        if(argc > 5) {
            printf("Too many arguments\n");
            exit(1);
        }

        list l;
        init(&l);

        char name[20] = {};
        char c_name[20] = {};
        char c;
        char* nme[1];


        strcpy(name, argv[3]);
        strcpy(c_name, argv[4]);
        nme[0] = name;

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

        write_table(nme, m, 1, c_name);

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
        char* s[50];
        char c_name[20] = {};

        int n;
        char ch;

        list l;
        init(&l);
        FILE *fp;

        char num = *argv[3];
        n = num - '0';
        int i = 0;
        int pos = 4;

        while(n) {
            n--;
            s[i] = (char*)malloc(sizeof(char) * 20);
            strcpy(s[i], argv[pos++]);
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
        strcpy(c_name, argv[pos]);
        make_tree(&l);
        int a[50];
        map m;
        init_map(&m);
        get_code(l, a, 0, &m);
        n = i;
        i = 0;
        write_table(s, m, n, c_name);
    }
    else if(ch == 'm') {

        //1st archive will be extracted and moved to dir
        //second archive contents should be sent to the same place
        //all files of dir should be compressed
        //all files in dir should be deleted

        FILE *fp;
        char* s[50];
        int n;
        char ch;
        char c_name[20] = {};

        list l;
        init(&l);

        char num = *argv[3];
        n = num - '0';

        int i = 0;
        int pos = 4;

        list l1;
        mkdir("DIR");

        while(n) {
            n--;
            s[i] = (char*)malloc(sizeof(char) * 20);
            strcpy(s[i], argv[pos++]);
            fp = fopen(s[i], "r");
            init(&l1);
            read_header(fp, &l1);
        }
        //got all files in DIR
        i = 0;
        n = merge_count;
        char* append_nme[20] = {};
        while(n) {
            n--;

            append_nme[i] = (char*)malloc(sizeof(char) * 50);
            strcpy(append_nme[i], "DIR/");
            strcat(append_nme[i], merge_nme[i]);

            fp = fopen(append_nme[i], "r");

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
            i++;
        }
        strcpy(c_name, argv[pos]);

        make_tree(&l);

        int a[50];
        map m;
        init_map(&m);
        get_code(l, a, 0, &m);

        n = i;
        i = 0;
        write_table(append_nme, m, n, c_name);
        for(int i = 0; i < merge_count; i++) {
            remove(append_nme[i]);
        }
    }
    return 0;
}
