#include <stdio.h>
#include <stdlib.h>
#include "tar.h"
#include <string.h>

/* format: tar -flag (number of arguments if needed) (File names if needed) (Archive name)
    flags:
    -c: Compress a single file
    -a: Compress multiple files to make an archive
    -e: Extract an archive
    -m: Merge compressed archives
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
        //compress a single file
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

        strcpy(name, argv[3]);  //name of file
        strcpy(c_name, argv[4]); //name of archive to be created
        nme[0] = name;

        FILE* fp = fopen(name, "r");

        if(!fp) {
            printf("Cannot find file\n");
            exit(1);
        }

        //read the file character by character
        fscanf(fp, "%c", &c);
        while(!feof(fp)) {
            append(&l, ch);
            fscanf(fp, "%c", &ch);
        }

        fclose(fp);
        //link list of characters obtained

        make_tree(&l);  //to make a Huffman tree

        int a[50];
        map m;
        init_map(&m);
        get_code(l, a, 0, &m); //link list(map) of codes of characters obtained

        //writes the header and calls a function to encrypt the file
        write_table(nme, m, 1, c_name);

    }
    else if(ch == 'e') {
        //extract an archive
        if(argc > 4) {
            printf("Too many arguments\n");
            exit(1);
        }
        //DIR stores the extracted files
        mkdir("DIR");

        char name[20] = {};
        strcpy(name, argv[3]);

        FILE* fp = fopen(name, "r");

        list l1;
        init(&l1);

        //reads the header to make a Huffman tree and calls a function to decrypt the archive
        read_header(fp, &l1);
        fclose(fp);
    }
    else if(ch == 'a') {
        //compress multiple files into an archive
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
        //merge multiple compressed archives
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
        //temporary storage for the extracted files
        mkdir("DIR");

        while(n) {
            n--;
            s[i] = (char*)malloc(sizeof(char) * 20);
            strcpy(s[i], argv[pos++]);
            fp = fopen(s[i], "r");
            init(&l1);
            read_header(fp, &l1);
        }
        i = 0;
        n = merge_count;
        char* append_nme[20] = {};
        while(n) {
            n--;
            append_nme[i] = (char*)malloc(sizeof(char) * 50);
            strcpy(append_nme[i], "DIR/");
            strcat(append_nme[i], merge_nme[i]);

            //reading files from temporary storage to make an archive
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
        //creating final archive
        write_table(append_nme, m, n, c_name);

        //removing temporary files and directory
        for(int i = 0; i < merge_count; i++) {
            remove(append_nme[i]);
        }
        rmdir("DIR");
    }
    return 0;
}
