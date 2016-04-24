// main code using voyage.h

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

}

int  decrypt(int cipher[]) {

}

int main() {

    // Disable stdout buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // process flags
    // [en/decrypt, base64/hex, autogenerate key]

    return 0;
}

main();

