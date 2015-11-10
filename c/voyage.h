#ifndef VOYAGE_H_
#define VOYAGE_H_

// using 4096-bit keys

// return key_directives (one of 4 operations)
int[512] getkeydirective(int key[512]) {

    return key_directive[512];
}

// return key_width (operation bitwise widths)
int[512] getkeywidth(int key[512]) {
    
    return key_width[512];
}

// add 1 to every n-bit blocks
int[8] add(int block[8], n) {
    
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
