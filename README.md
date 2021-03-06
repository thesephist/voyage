# Voyage

[As of July 2017, all work on Voyage has stopped due to time reallocation in my work. Voyage is deprecated]

Encryption library and protocol using a round of block-level bitwise operations with a sigle-block final round.

## Concept

Voyage is a proof-of-concept of an encryption scheme based on an instance-specific, auto-generated encryption algorithm.

Voyage's encryption algorithm is based on the idea of generating a one-time encryption algorithm for each instance of encrypting any binary packet rather than using a global, general algorithm dependent on an instance-specific key or key pair. 

Rather than a single universal algorithm using an instance-specific key to encrypt data, Voyage uses an instance-specific key to randomly generate an encryption algorithm on the spot.

### Structure

1. For each instance, a key and a plaintext is given. The key is, in the current implementation, not a binary, but may be made to fit a 128 / 192 / 256-bit standard as one "word" of a key is a byte. (in the JavaScript library, an ASCII string can be parsed to a key with the rs\_w function)
2. From the key, a single instance-specific encryption algorithm __G__ is generated via the "Generating Algorithm" below.
3. __G__ is applied to the binary stream to be encrypted
4. A second pass of a universal, pre-set juxtaposition function __H__ is applied to the binary stream. __H__ takes the binary stream and key as parameters and produces a single binary ciphertext which acts as the ciphertext output.

### Generating algorithm

The principles behind generating __G__ is fairly simple. __G__ is a sequence of randomly sequenced, 4 different bit-flicking functions:

    add(n) // divides a binary stream into n-bit pieces, addes 1 bit to each binary numeral, concatenates
           // e.g. 00 11 01 => add(2) => 01 00 10
    cycle(n) // equivalent to bit-flicking n-bit pieces circle-right by 1 bit 
             // e.g. 01 10 11 => cycle(2) => 10 01 11
    reverse(n) // reverses the order of n-bit pieces in the binary stream
               // e.g. 101 100 => reverse(3) => 101 001
    split(n) // splits and juxtaposes binary stream into n-bit sized chunks
             // e.g. 11110000 => split(4) => 10101010
             // e.g. 11100010 => split(4) => 10101100

## X-implementation

A few weeks after the initial publication of Voyage, a critical weakness was noticed, that a single-bit change in the encrypted data only slightly modifies the plaintext, lending to possible attacks using slight modifications of plaintext and ciphertext. As a solution, a far more effective H-algorithm was implemented that bit-cycles the binary as a part of the H algorithm to much more effectively hash the output of __G__ in place.

Ciphertexts from the X-implementation returns completely obfuscated data when decrypted with a single bit out of place, and a single change in the plaintext results in a far greater change in the ciphertext. 

For legacy support purposes (mainly as I still have files encrypted with the non-X version that I still need to access), the original algorithm is begin preserved.

## Project Roadmap

voyage.js is a proof-of-concept script capable of producing a "binary" string output (as a concatenated array of 1's and 0's) and as a hex binary, along with a JavaScript buffer object. The JavaScript implementation also includes functions to safely encode and decode text input into binary useable within the voyage.js processors and key generator and interpretor methods from the built-in Math.random.

A more optimized, C-based implementation is also being developed side-by-side that focuses on filesystem-level encryption rather than practical use in encrypting ASCII text. 

For both implementations, the future conceptual roadmap includes, in no particular order,

* Manipuating binary data and binary streams rather than strings of 0's and 1's, with a C port of the Javascript implementation
* Compute optimizations
* Investigations into the cryptographic security of Voyage's encrypted outputs
* Improved security in the H-algorithm implementations

Other possible additions or changes in the algorithm include, in no particular order, 

* Using a different sub-key for each block derived from a cryptographic hash of the key and plaintext of the preceding block
* Using a lookup table in the __H__-algorithm for greater nonlinearity
* Replacing or adding to the generator algorithm elements that use the key as a parameter

- - -

\* At the present moment, for proof-of-concept and debugging purposes, the key is a string of integers.



