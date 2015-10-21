Voyage Project Outline 
===

The key function for the algorithm is the generator function G() that generates randomly seeded, perfectly reversible bit string flicking functions working on a binary. 

The function first takes several "elements" of the generator:
* Inverse() - 0 -> 1, 1 -> 0
  * inverse(n) will probably be the official function, where the n-digit binary is added one, so like 10 -> 11, 01 -> 10, and 1101 -> 1110
* Cycle(n) - Rotates to the right x digits
* Reverse(n) - Split bitstring into n-length pieces, reverses front / back (reverses the order)
* Split(n) - Split string into n segments and uniformly "scramble", reorder as a shuffle

- - -

The seed is a bit string of even numbered digits, and the function G(seed) then returns a sequence of functions where each of the four possible double-digit bit strings correspond to one element of the generator function. Thus G(seed) returns something of the form:

// have to determine how to store the integer arguments....

G(seed_string);
=>  function(){
    	inverse(3);
    	cycle(4);
    	reverse();
    	cycle(2);
    	split(32);
    	inverse(6);
    	cycle(-5);
    };

- - -

// work more on those;

