#ifndef VOYAGE_H_
#define VOYAGE_H_

// using 4096-bit keys

// return key_directives (list of one of 4 operations)
int[512] getkeydirective(int key[512]) {
    int key_directive[512];

    for (i = 0; i < 512; i++) {
        // Returns 0 for key byte value 0-63, 1 for 64-127, 2 for 128-191, 3 for 192-255
        key_directive[i] = floor(key[i] / 64);
    }

    return key_directive[512];
}

// return key_width (operation bitwise widths)
int[512] getkeywidth(int key[512]) {
    int key_width[512];

    for (i = 0; i < 512; i++) {
        // Returns modulus of 64 of a given key byte value
        key_width[i] = key[i] % 64;
    }

    return key_width[512];
}

// add 1 to every n-bit blocks
int[8] add(int block[8], n) {
    // needs optimization
    if (n <= 1) {
        for (i = 0; i < 8; i++) {
            block[i] = 63 - block[i];
        }
    } else if (n == 2) {
        // this requires some further optimization specifically
        int mini[4];
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
        for (i = 0; i < 8, i++) {
            mini[0] = block[i] % 16;
            mini[1] = block[i] - mini[0];

            // coming soon, same as above
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (i % (n / 8) == 0) {
                block[i] = (block[i] + 1) % 256;
            }
        }
    }

    return block;
}

// bitwise cycle to right
int[8] cycle(int block[8], n) {
    
    return block;
}

// reverse bit order
int[8] reverse(int block[8], n) {
    
    return block;
}

// split and rejoin in different order
int[8] split(int block[8], n) {
    
    return block;
}

// reverse operators

// add
int[8] radd(int block[8], n) {
    // needs optimization
    if (n <= 1) {
        for (i = 0; i < 8; i++) {
            block[i] = 63 - block[i];
        }
    } else if (n == 2) {
        // this requires some further optimization specifically
        int mini[4];
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
        for (i = 0; i < 8, i++) {
            mini[0] = block[i] % 16;
            mini[1] = block[i] - mini[0];

            // coming soon, same as above
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (i % (n / 8) == 0) {
                block[i] = (block[i] - 1) % 256;
            }
        }
    }

    return block;
}

// cycle
int[8] rcycle(int block[8], n) {
    
    return block;
}

// reverse
int[8] rreverse(int block[8], n) {
    
    return block;
}

// split
int[8] rsplit(int block[8], n) {
    
    return block;
}

// G round
int[8] G(int block[8], int key_directive[], int key_width[]) {

    return block;
}

// reverse G round
int[8] RG(int block[8], int key_directive[], int key_width[]) {
    
    return block;
}

// H round
// Replace current buffer byte with byte of another buffer with offset

// reverse H round

#endif // VOYAGE_H_
