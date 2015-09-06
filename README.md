# Voyage
Bit-level encryption library using instance-specific encryption algorithms

## Concept

Voyage is a proof-of-concept JS script for encryption.

Voyage's encryption algorithm is based on the idea of generating a one-time encryption algorithm for each instance of encrypting any binary packet rather than using a global, general algorithm dependent on an instance-specific key or key pair. 

Rather than a single universal algorithm using an instance-specific key to encrypt data, Voyage uses an instance-specific key to randomly generate an encryption algorithm on the spot.

### Structure

1. For each instance, a key and a plaintext is given. The plaintext is most likely converted into 8-bit ASCII, concatenated into a single binary stream. The key\*\* is a binary stream.
2. From the key, a single instance-specific encryption algorithm __G__ is generated.
3. __G__ is applied to the binary stream to be encrypted
4. A second pass of a universal, pre-set juxtaposition function __H__\* is applied to the binary stream. __H__ takes the binary stream and key as parameters and produces a single statistically random binary stream.

### Generating algorithm

The principles behind generating __G__ is fairly simple. __G__ is a sequence of randomly sequenced, 4 different bit-flicking functions:

    add(n) // divides a binary stream into n-bit pieces, addes 1 bit to each binary numeral, concatenates
           // e.g. 001101 => add(2) => 010010
    cycle(n) // equivalent to bit-flicking n-bit pieces circle-right by n bits
             // e.g. 011011 => cycle(2) => 100111
    reverse(n) // reverses the order of n-bit pieces in the binary stream
               // e.g. 101100 => reverse(2) => 001110
    split(n) // splits and juxtaposes binary stream into n-bit sized chunks
             // e.g. 11110000 => reverse(4) => 10101010
             // e.g. 11100010 => reverse(4) => 10101100

## Project Roadmap

voyage.js is a proof-of-concept script, and there are several featuers in development

* Manipuating binary data and binary streams rather than strings of 0's and 1's used for easy development
* Compute optimizations
* More complex __H__
* Key as a single binary, not a sequence of integers

- - -

\* At the present moment, for proof-of-concept purposes, __H__ is simplified.

\*\* At the present moment, for proof-of-concept and debugging purposes, the key is a string of integers.



