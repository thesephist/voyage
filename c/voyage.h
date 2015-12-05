#ifndef VOYAGE_H_
#define VOYAGE_H_

// using 4096-bit keys, but this number only reflects getkeydirective() and getkeywidth()

// return key_directives (list of one of 4 operations)
int[512] getkeydirective(int key[512]) {
    int key_directive[512];
    
    int i;

    for (i = 0; i < 512; i++) {
        // Returns 0 for key byte value 0-63, 1 for 64-127, 2 for 128-191, 3 for 192-255
        key_directive[i] = floor(key[i] / 64.0f);
    }

    return key_directive[512];
}

// return key_width (operation bitwise widths)
int[512] getkeywidth(int key[512]) {
    int key_width[512];

    int i;

    for (i = 0; i < 512; i++) {
        // Returns modulus of 64 of a given key byte value
        key_width[i] = key[i] % 64;
    }

    return key_width[512];
}

// returns an array of divided-up values according to getkeywidth()'s return value
int[] divider(int block[8], int width) {
    int arraywidth = 64 / width;
    
    int subblock[arraywidth];

    if (arraywidth >= 8) {
        subblock = block; // for now; will change in future program architectures
    } else if (arraywidth == 4) {
        int i;

        for (i = 0; i < 8; i++) {
            subblock[i] = block[i] & 240;
            subblock[2 * i + 1] = block[i] & 15;
        }
    } else if (arraywidth == 2) {
        int i;

        for (i = 0; i < 8; i++) {
            subblock[i] = block[i] & 192;
            subblock[4 * i + 1] = block[i] & 48;
            subblock[4 * i + 2] = block[i] & 12;
            subblock[4 * i + 3] = block[i] & 3;
        } 
    } else if (arraywidth == 1 || arraywidth == 0) {
        int i;

        for (i = 0; i < 8; i++) {
            subblock[i] = block[i] & 128;
            subblock[8 * i + 1] = block[i] & 64;
            subblock[8 * i + 2] = block[i] & 32;
            subblock[8 * i + 3] = block[i] & 16;
            subblock[8 * i + 4] = block[i] & 8;
            subblock[8 * i + 5] = block[i] & 4;
            subblock[8 * i + 6] = block[i] & 2;
            subblock[8 * i + 7] = block[i] & 1;
        }
    }

    return subblock;
}

// add 1 to every n-bit blocks
int[8] add(int block[8], int n) {
    // needs optimization
    if (n <= 1) {
        int i;

        for (i = 0; i < 8; i++) {
            block[i] = 63 - block[i];
        }
    } else if (n == 2) {
        // this requires some further optimization specifically
        int mini[4];
        int i;

        for (i = 0; i < 8; i++) {
            mini[0] = block[i] % 4;
            mini[1] = block[i] % 16 - mini[0];
            mini[2] = block[i] % 64 - mini[0] - mini[1];
            mini[3] = block[i] - block[i] % 64;

            // coming soon
        }
    } else if (n == 4) {
        // this too requires some further optimization
        int mini[2];
        int i;

        for (i = 0; i < 8, i++) {
            mini[0] = block[i] % 16;
            mini[1] = block[i] - mini[0];

            // coming soon, same as above
        }
    } else {
        int i;

        for (i = 0; i < 8; i++) {
            if (i % (n / 8) == 0) {
                block[i] = (block[i] + 1) % 256;
            }
        }
    }

    return block;
}

// bitwise cycle to right
int[8] cycle(int block[8], int n) {
    // need to be made to work with not just byte-units but all values of n
    block = block >> 1;
    
    return block;
}

// reverse bit order
int[8] reverse(int block[8], int n) {
    
    return block;
}

// split and rejoin in different order
int[8] split(int block[n], int n) {
    
    return block;
}

// reverse operators

// add
int[8] radd(int block[n], int n) {
    // needs optimization
    if (n <= 1) {
        int i;

        for (i = 0; i < 8; i++) {
            block[i] = 63 - block[i];
        }
    } else if (n == 2) {
        // this requires some further optimization specifically
        int mini[4];
        int i;

        for (i = 0; i < 8; i++) {
            mini[0] = block[i] % 4;
            mini[1] = block[i] % 16 - mini[0];
            mini[2] = block[i] % 64 - mini[0] - mini[1];
            mini[3] = block[i] - block[i] % 64;

            // coming soon
        }
    } else if (n == 4) {
        // this too requires some further optimization
        int mini[2];
        int i;

        for (i = 0; i < 8, i++) {
            mini[0] = block[i] % 16;
            mini[1] = block[i] - mini[0];

            // coming soon, same as above
        }
    } else {
        int i;

        for (i = 0; i < 8; i++) {
            if (i % (n / 8) == 0) {
                block[i] = (block[i] - 1) % 256;
            }
        }
    }

    return block;
}

// cycle
int[8] rcycle(int block[n], int n) {
    // need to be made to work with not just byte-units but all values of n
    block = block << 1;

    return block;
}

// reverse
int[8] rreverse(int block[n], int n) {
    
    return block;
}

// split
int[8] rsplit(int block[n], int n) {
    
    return block;
}

// G round
int[8] G(int block[8], int key_directive[], int key_width[]) {
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
int[8] RG(int block[8], int key_directive[], int key_width[]) { 
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

// H round
// Replace current buffer byte with byte of another buffer with offset

// reverse H round

#endif // VOYAGE_H_
