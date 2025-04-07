#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
const int PASS_LEN = 100;       //handles long words like full phrases
const int HASH_LEN = 33;        //length of MD5 hash strings + null terminator
//finished tryWord function
char *tryWord(char *plaintext, char *hashFilename)
{
    // hash the plaintext
    char *hashed = md5(plaintext, strlen(plaintext));

    // open the hash file
    FILE *file = fopen(hashFilename, "r");
    if (!file) {
        printf("error opening hash file\n");
        return NULL;
    }

    char line[HASH_LEN];
    while (fgets(line, sizeof(line), file)) {
        // newline remover
        for (int i = 0; i < HASH_LEN; i++) {
            if (line[i] == '\n' || line[i] == '\r') {
                line[i] = '\0';
                break;
            }
        }

        if (strcmp(hashed, line) == 0) {
            fclose(file);
            //return the matched hash
            return hashed;  
        }
    }
//close file
    fclose(file);
    return NULL;
}
//mainnnnnnnn
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        return 1;
    }

    char *hashFilename = argv[1];
    char *dictFilename = argv[2];

    FILE *dictFile = fopen(dictFilename, "r");
    if (!dictFile) {
        printf("error opening dictionary file\n");
        return 1;
    }

    int cracked = 0;
    char word[PASS_LEN];
//fgets input for the file
    while (fgets(word, sizeof(word), dictFile)) {
        // manually remove newline
        for (int i = 0; i < PASS_LEN; i++) {
            if (word[i] == '\n' || word[i] == '\r') {
                word[i] = '\0';
                break;
            }
        }
//
        char *found = tryWord(word, hashFilename);
        if (found) {
            printf("%s %s\n", found, word);
            cracked++;
        }
    }
 //closes the file
    fclose(dictFile);
    printf("%d hashes cracked!\n", cracked);
}
