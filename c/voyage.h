#ifndef VOYAGE_H_
#define VOYAGE_H_

#include <time.h>
#include <math.h>
#include <stdlib.h>

/* HEADER FILE ROADMAP
 *
 * the 4 elementary functions each take an array of 64 bytes
 * .... and apply their bitwise modifications to each group. Specifically...
 *    - add(n) adds to every $n$th byte a factor (now 11),
 *    - cycle(n) bitwise RCIRC every $n$th byte a factor (now 3),
 *    - reverse(n) bitwise reverse every $n$th byte
 *    - split(n) bitwise split every $n$th byte
 *
 * G cycles through them per the key, using instruction arrays from the key_directive and key_width
 *
 * H cycles through one last time
 *
 */

// key generator and validator
// using 256-bit keys, but this number is only reflected in getkeydirective() and getkeywidth() and G, H
int * keygen() {
    int key[32];
    srand(time(NULL));
 
    int i;
    for (i = 0; i < 32; i++) {
        key[i] = rand() % 256;
    }

    int *keyp;
    keyp = &key;
    return keyp;
}

// return key_directives (list of one of 4 operations)
int * getkeydirective(int *key) {
    int key_directive[32];
    
    int i;
    for (i = 0; i < 32; i++) {
        // Returns 0 for key byte value 0-63, 1 for 64-127, 2 for 128-191, 3 for 192-255
        key_directive[i] = floor(key[i] / 64.0f);
    }

    int *kd;
    kd = &key_directive;
    return kd;
}

// return key_width (operation bitwise widths)
int * getkeywidth(int *key) {
    int key_width[32];

    int i;
    for (i = 0; i < 32; i++) {
        // Returns modulus of 64 of a given key byte value
        key_width[i] = key[i] % 64;
        if (key_width[i] == 0) {
            // key_width cannot be zero
            key_width[i] = 1;
        }
    }

    int *kw;
    kw = &key_width;
    return kw;
}

int * getroundkey(int *key) {
    // this is an ad-hoc created function
    // and is subject to a lot of changes still

    int volume;
    int roundvolume;
    int roundkey[64];
    
    volume = 0;
    
    int i;
    for (i = 0; i < 32; i++) {
        volume += key[i];
    }

    roundvolume = (volume * 37) % 64; // this 37 is so arbitrary...

    for (i = 0; i < 64; i++) {
        roundkey[i] = key[(i + roundvolume) % 32];
    }
    
    int *rk;
    rk = &roundkey;
    return roundkey;
}

// add 1 to every n-bit blocks
int * add(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = (block[i] + 11) % 256; // the constant addition factor will in the end probably not be 11
        }
    }

    return block;
}

// bitwise cycle to right
int * cycle(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] >> 3; // final shifting constant will probably not be 3
        }
    }

    return block;
}

// reverse bit order
int * reverse(int *block, int n) {
 
    // this really could use a lot less memory

    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = (block[i] & 0xF0) >> 4 | (block[i] & 0x0F) << 4;
            block[i] = (block[i] & 0xCC) >> 2 | (block[i] & 0x33) << 2;
            block[i] = (block[i] & 0xAA) >> 1 | (block[i] & 0x55) << 1;
        }
    }

    return block;
}

int * split(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] & 128
                    + (block[i] & 8) * 8
                    + (block[i] & 64) / 2
                    + (block[i] & 4) * 4
                    + (block[i] & 32) / 4
                    + (block[i] & 2) * 2
                    + (block[i] & 16) / 8
                    + (block[i] & 1);
        }
    }

    return block;
}

// inverse operations 
int * radd(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = (block[i] - 11) % 256;
        }
    }

    return block;
}

int * rcycle(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] << 3;
        }
    }

    return block;
}

int * rreverse(int *block, int n) {
    // for optimization later, this should be taken out entirely
    block = reverse(block, n);

    return block;
}

int * rsplit(int *block, int n) {
    
    int i;
    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] & 128
                    + (block[i] & 32) * 2
                    + (block[i] & 8) * 4
                    + (block[i] & 2) * 8
                    + (block[i] & 64) / 8
                    + (block[i] & 16) / 4
                    + (block[i] & 4) / 2
                    + (block[i] & 1);
         }
    }

    return block;
}


// G round
int * G(int *block, int *key_directive, int *key_width) {
    int key_size = sizeof(key_directive) / sizeof(key_directive[0]);

    int i;
    for (i = 0; i < key_size; i++) {
        if (key_directive[i] == 0) {
            block = add(block, key_width[i]);
        } else if (key_directive[i] == 1) {
            block = cycle(block, key_width[i]);
        } else if (key_directive[i] == 2) {
            block = reverse(block, key_width[i]);
        } else if (key_directive[i] == 3) {
            block = split(block, key_width[i]);
        }
    }

    return block;
}

// reverse G round
int RG(int *block, int *key_directive, int *key_width) { 
    int key_size = sizeof(key_directive) / sizeof(key_directive[0]);
    
    int i;

    for (i = 0; i < key_size; i++) {
        if (key_directive[i] == 0) {
            block = radd(block, key_width[i]);
        } else if (key_directive[i] == 1) {
            block = rcycle(block, key_width[i]);
        } else if (key_directive[i] == 2) {
            block = rreverse(block, key_width[i]);
        } else if (key_directive[i] == 3) {
            block = rsplit(block, key_width[i]);
        }
    }

    return block;
}

// H and RH will be the same, as it is an XOR with a key with modifications
int H(int *block, int *key) {
    int roundkey[64];

    memcpy(roundkey, getroundkey(key), 64);

    // xor with roundkey
    int i;

    for (i = 0; i < 64; i++) {
        block[i] = block[i] ^ roundkey[i];
    }

    return block;
}

int encryptBlock(int *block, int *key) {

    block = G(block, getkeydirective(key), getkeywidth(key));

    block = H(block, key);

    return block;
}

int decryptBlock(int *block, int *key) {

    block = H(block, key);

    block = RG(block, getkeydirective(key), getkeywidth(key));

    return block;
}

#endif // VOYAGE_H_
