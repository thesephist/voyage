// main code using voyage.h
// compile with the following: 
// gcc main.c voyage.h -lm -w -o voyage.out

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "voyage.h"

// encoding and decoding text in stdin/stdout
int encode(char text[]) {
        
    int i = 0;
    int encoded[ sizeof(text) / sizeof(text[0]) ];

    while (text[i]) {
        
        encoded[i] = text[i];

        i++;
    }
    
    return encoded;
}

char decode(int encoded[]) {
        
    int i = 0;
    char decoded[ sizeof(encoded) / sizeof(encoded[0]) ];

    while (encoded[i]) {
        
        decoded[i] = encoded[i];

        i++;
    }
    
    return decoded;
}

// standard encryption and decryption functions
// run encryptBlock and decryptBlock for each block in loop
int encrypt(int plain[]) {
    // encryption of integer blocks, run while there's file for each 64 byte block
}

int  decrypt(int cipher[]) {
    // decryption of integer blocks, run while there's file for each 64 byte block
}

int main(int argc, char ** argv) {

    // Disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // process flags
    // [en/decrypt, base64/hex, autogenerate key]

    // file level things
    FILE *inputFile, *outputFile;
    int in, out;
    int *buf[64]; // 64 bytes for now, and maybe even forever for low memory utilization, just recycling the same pointer

    // TEMPORARY
    int *key;
    key = keygen();

    if (argc != 3) { // for now accept no options
        fprintf(stderr, "Voyage encryption tool\n");
        exit(1);
    }

    if ((inputFile = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Input file open error\n");
        exit(1);
    } else {
        fprintf(stdout, "=> Input file opened...");
    }

    if ((outputFile = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "Output file open error\n");
        exit(1);
    } else {
        fprintf(stdout, "Output file opened\n");
    }

    while (feof(inputFile) == 0) {
        if ((in = fread(buf, 1, 64, inputFile)) != 100) {
            if (ferror(inputFile) != 0) {
                fprintf(stderr, "File read error\n");
                exit(1);
            }
        }
        
        // voyage processing here on int *buf[64]
        // practice encrypting for now 
        // encryptBlock(buf, key);

        if ((out = fwrite(buf, 1, in, outputFile)) != in) {
            fprintf(stderr, "File write error\n");
            exit(1);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

