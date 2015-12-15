#ifndef VOYAGE_H_
#define VOYAGE_H_

/* HEADER FILE ROADMAP
 *
 * the 4 elementary functions each take an array of integers
 * .... and apply their bitwise modifications to each integer
 *
 * the divider function takes a block[8] and returns an array of integers to be operated upon. 
 *      if keywidth > 8, the list will only contain the 8-bit units that need to be modified.
 *
 * the combinator function takes a block[8] and the output of modified divider(block) and combines it into ah new block[8]
 *
 * within a cycle, divider (modblock array)=> 4 elementary functions (modblock array, block array)=> combinator (new block)
 *
 */

// using 256-bit keys, but this number is only reflected in getkeydirective() and getkeywidth()

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

// returns an array of divided-up values according to getkeywidth()'s return value
int[] divider(int block[8], int width) {
    int arraywidth = 64 / width;
    
    int modblock[arraywidth];

    if (arraywidth >= 8) {
        int i;

        for (i = 0; i < 64 / arraywidth; i++) {
            modblock[i] = block[i / 8];
        }
    } else if (arraywidth == 4) {
        int i;

        for (i = 0; i < 8; i++) {
            modblock[i] = block[i] & 240;
            modblock[2 * i + 1] = block[i] & 15;
        }
    } else if (arraywidth == 2) {
        int i;

        for (i = 0; i < 8; i++) {
            modblock[i] = block[i] & 192;
            modblock[4 * i + 1] = block[i] & 48;
            modblock[4 * i + 2] = block[i] & 12;
            modblock[4 * i + 3] = block[i] & 3;
        } 
    } else { // if arraywidth == 1
        int i;

        for (i = 0; i < 8; i++) {
            modblock[i] = block[i] & 128;
            modblock[8 * i + 1] = block[i] & 64;
            modblock[8 * i + 2] = block[i] & 32;
            modblock[8 * i + 3] = block[i] & 16;
            modblock[8 * i + 4] = block[i] & 8;
            modblock[8 * i + 5] = block[i] & 4;
            modblock[8 * i + 6] = block[i] & 2;
            modblock[8 * i + 7] = block[i] & 1;
        }
    }

    return modblock;
}

int[8] combinator(int block[8], int modblock[], int width) {
    // combinator function
    
    if (width >= 8) {
        int i;
        
        for (i = 0; i < 64 / width; i++) {
            block[i / 8] = modblock[i];
        }
    } else if (width == 4) {
        int i;

        for (i = 0; i < 8; i++) {
            block[i] = modblock[2 * i] & 240
              + modblock[2 * i + 1] & 15;
        }
    } else if (width == 2) {
        int i;

        for (i = 0; i < 8; i++) {
            block[i] = modblock[4 * i] & 192
              + modblock[4 * i + 1] & 48
              + modblock[4 * i + 2] & 12
              + modblock[4 * i + 3] & 3;
        }
    } else {
        int i;

        for (i = 0; i < 8; i++) {
            block[i] = modblock[8 * i] & 128 + modblock[8 * i + 1] & 64
              + modblock[8 * i + 2] & 32
              + modblock[8 * i + 3] & 16
              + modblock[8 * i + 4] & 8
              + modblock[8 * i + 5] & 4
              + modblock[8 * i + 6] & 2
              + modblock[8 * i + 7] & 1;
 
        }
    }

    return block;
}

// add 1 to every n-bit blocks
int[] add(int modblock[], int n) {

    int i;
    if (n >= 8) {
        n = 8;
    }

    for (i = 0; i < 64 / n; i++) {
        modblock[i] = (modblock[i] + 1) % pow(2, n)
    }

    return modblock;
}

// bitwise cycle to right
int[] cycle(int modblock[], int n) {
     
    int i;

    for (i = 0; i < 64 / n; i++) {
        modblock[i] = block >> 1;
    }

    return modblock;
}

// reverse bit order
int[] reverse(int modblock[], int n) {
    
    return modblock;
}

// split and rejoin in different order
int[] split(int modblock[], int n) {
    
    return modblock;
}

// reverse operators

// add
int[] radd(int modblock[], int n) {

    int i;
    if (n >= 8) {
        n = 8;
    }

    for (i = 0; i < 64 / n; i++) {
        modblock[i] = (modblock[i] - 1) % pow(2, n) + pow(2, n);
    }

    return modblock;
}

// cycle
int[] cycle(int modblock[], int n) {
     
    int i;

    for (i = 0; i < size; i++) {
        modblock[i] = block << 1;
    }

    return modblock;
}

// reverse bit order
int[] reverse(int modblock[], int n) {
    
    return modblock;
}

// split and rejoin in different order
int[] split(int modblock[], int n) {
    
    return modblock;
}


// G round
int[8] G(int block[8], int key_directive[], int key_width[]) {
    int key_size = sizeof(key_directive) / sizeof(key_directive[0]);

    int i;

    for (i = 0; i < key_size; i++) {
        if (key_directive[i] == 0) {
            block = add(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 1) {
            block = cycle(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 2) {
            block = reverse(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 3) {
            block = split(divider(block, key_width[i]), key_width[i]);
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
            block = radd(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 1) {
            block = rcycle(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 2) {
            block = rreverse(divider(block, key_width[i]), key_width[i]);
        } else if (key_directive[i] == 3) {
            block = rsplit(divider(block, key_width[i]), key_width[i]);
        }
    }

    return block;
}

// H round
// Replace current buffer byte with byte of another buffer with offset

// reverse H round

#endif // VOYAGE_H_
