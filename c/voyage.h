#ifndef VOYAGE_H_
#define VOYAGE_H_

#include <time.h>
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

static const unsigned char reverseTable[] = {
    // stuff here
};

static const unsigned char splitTable[] = {
    // stuff here
};

static const unsigned char rsplitTable[] = {
    // stuff here
};

// key generator and validator
// using 256-bit keys, but this number is only reflected in getkeydirective() and getkeywidth() and G, H
int[32] keygen() {
    int key[32];
    srand(time(NULL));
 
    int i;
    for (i = 0; i < 32; i++) {
        key[i] = rand() % 255;
    }

    return key;
}

// return key_directives (list of one of 4 operations)
int[32] getkeydirective(int key[32]) {
    int key_directive[32];
    
    int i;

    for (i = 0; i < 32; i++) {
        // Returns 0 for key byte value 0-63, 1 for 64-127, 2 for 128-191, 3 for 192-255
        key_directive[i] = floor(key[i] / 64.0f);
    }

    return key_directive[32];
}

// return key_width (operation bitwise widths)
int[32] getkeywidth(int key[32]) {
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

    return key_width[32];
}

int[64] getroundkey(int key[32]) {
    // this is an ad-hoc created function
    // and is subject to a lot of changes still

    int volume;
    int roundvolume;
    int roundkey[64];
    
    volume = 0;

    for (i = 0; i < 32; i++) {
        volume += key[i];
    }

    roundvolume = (volume * 37) % 64; // this 37 is so arbitrary...

    for (i = 0; i < 64; i++) {
        roundkey[i] = key[(i + roundvolume) % 32];
    }
    
    return roundkey;
}

// add 1 to every n-bit blocks
int[64] add(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = (block[i] + 11) % 256; // the constant addition factor will in the end probably not be 11
        }
    }

    return block;
}

// bitwise cycle to right
int[64] cycle(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] >> 3; // final shifting constant will probably not be 3
        }
    }

    return block;
}

// reverse bit order
int[64] reverse(int block[64], int n) {
 
    // this really could use a lot less memory
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = // some reverse method of block[i]; use a lookup table
        }
    }

    return block;
}

// split and rejoin in juxtaposition
int[64] split(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = // some split method of block[i]; use a lookup table
        }
    }
}

// inverse operations 
int[64] radd(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = (block[i] - 11) % 256;
        }
    }

    return block;
}

int[64] rcycle(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = block[i] << 3;
        }
    }

    return block;
}

int[64] rreverse(int block[64], int n) {
    // for optimization later, this should be taken out entirely
    block = reverse(block);

    return block;
}

int[64] rsplit(int block[64], int n) {
    
    int i;

    for (i = 0; i < 64; i++) {
        if ((i - 1) % n == 0) {
            block[i] = // some split method of block[i]; use a reverse lookup table
        }
    }
}


// G round
int[64] G(int block[64], int key_directive[32], int key_width[32]) {
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
int[64] RG(int block[64], int key_directive[32], int key_width[32]) { 
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
int[64] H(int block[64], int key[32]) {
    int roundkey[64];

    roundkey = getroundkey(key);

    // xor with roundkey
    for (i = 0; i < 64; i++) {
        block[i] = block[i] ^ roundkey[i];
    }

    return block;
}

int[64] encryptBlock(int block[64], int key[32]) {

    block = G(block, getkeydirective(key), getkeywidth(key));

    block = H(block, key);

    return block;
}

int[64] decryptBlock(int block[64], int key[32]) {

    block = H(block, key);

    block = RG(block, getkeydirective(key), getkeywidth(key));

    return block;
}



#endif // VOYAGE_H_
