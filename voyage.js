// Voyage Functional Encryption Software: VFES
// Copyright Linus Lee 2015

var processing, // bit string in processing
    key; // encryption key

var zerofiller = "00000000000000";
var onefiller  = "11111111111111";

function rset(n){
    rxe = new RegExp(".{1," + n.toString() + "}","g");
    return rxe;
}

function encode(words){
    // encodes string to binary ASCII string
    wordsList = words.split("");
    bits = "";
    wordsList.forEach(function(letter){
        mlk = "0"+parseInt(letter.charCodeAt()).toString(2);
        bits += zerofiller.substr(0, 8 - mlk.length) + mlk;
    });
    
    return bits;
}

function decode(bits){
    // decodes binary ASCII to string
    words = "";
    bitsList = bits.match(rset(8));
    bitsList.forEach(function(code){
        words += String.fromCharCode(parseInt(code,2));
    });
    return words;
}

// G elements
function adder(n){ // formerly "inverse", when applied only to 1-bit string objects
    stringBits = processing.match(rset(n));
    totalBits = "";
    
    stringBits.forEach(function(bitstring){
        e = (parseInt(bitstring, 2) + 1).toString(2);
        if (e.length<n) {
            e=zerofiller.substr(0, n - e.length) + e
        }
        if (e.length == n + 1) {
            e = zerofiller.substr(0, n)
        }
        totalBits += e;
    });
    
    processing = totalBits;
}
function rAdder(n){ // inverse function of adder()
    stringBits = processing.match(rset(n));
    totalBits = "";
    
    stringBits.forEach(function(bitstring){
        e = "";
        if (bitstring == zerofiller.substr(0, n)) {
            e = onefiller.substr(0, n)}
        else {
            e = (parseInt(bitstring, 2) - 1).toString(2);
        }
        if (e.length<n){
            e = zerofiller.substr(0, n - e.length) + e
        }
        if (e.length == n + 1) {
            e = zerofiller.substr(0, n);
        }
        totalBits += e;
    });

    processing = totalBits;
}
function cycle(n){
    stringBits = processing.match(rset(n));
    totalBits = "";
    
    stringBits.forEach(function(bitstring){
        e = bitstring.substring(1, n) + bitstring[0];
        totalBits += e;
    });

    processing = totalBits;
}
function rCycle(n){   
    stringBits = processing.match(rset(n));
    totalBits = "";
    
    stringBits.forEach(function(bitstring){
        e = bitstring[n-1] + bitstring.substring(0,n-1)
        totalBits += e;
    });
    
    processing = totalBits;
}

function reverse(n){    
    stringBits = processing.match(rset(n));
    processing = stringBits.reverse().join("");
}
function rReverse(n){
    reverse(n);
}

function split(n){    
    bitsarray = new Array(n);
    for (i = 0; i < n; i++) {
        bitsarray[i] = ""
    }
    for (i = 0; i < processing.length; i++) {
        bitsarray[i % n] += processing[i]
    }
    
    processing=bitsarray.join("");
}
function rSplit(n){
    bitsarray = processing.match(rset(processing.length / n));
    processing = "";
    for(i = 0; i < bitsarray.join("").length; i++) {
        processing += bitsarray[i % n][Math.floor(i / n)];
    }
}

// Other Modules:
var random_seed;

function rs_g(){
    // random key generator
    random_seed = "";
    digits = Math.floor((Math.random() * 4) + 4) * 2 + 1; // digit # from 100 to 200
    for(i=0; i < digits; i++){
        if(i % 2 == 0){
            random_seed += Math.floor((Math.random() * 4)).toString();
        }else{
            random_seed += Math.pow(2,Math.floor((Math.random() * 4))).toString(); // May be changed to 16-, 32- or 64- bit encryption later
            random_seed += " ";
        }
    }

    // makes up for odd-numbered digit keys
    if(random_seed.length%3!=0){random_seed+="2"};
    
    return random_seed.toString();
}

function key_norm(size, key){
    key = parseInt(key.split(" ").join("")).toString(2);

    key_size = key.length;
    diff = size % key_size;
    return key.repeat(parseInt((size - diff) / key_size)) + key.substr(0, diff);
}

function bit_norm(){
    console.log(processing);
    
    adder(8);
    rAdder(8);
}

function G(binary, random_seed){
    split_seed = random_seed.split(" ");
    
    console.log(processing);

    processing = binary;
    console.log("hi " + processing);
    bit_norm();
    split_seed.forEach(function(pair){
        if      (pair[0] == "0"){adder(parseInt(pair[1]))}
        else if (pair[0] == "1"){cycle(parseInt(pair[1]))}
        else if (pair[0] == "2"){reverse(parseInt(pair[1]))}
        else if (pair[0] == "3"){split(parseInt(pair[1]))}
    });
    
    return processing;
}

function r_G(binary, random_seed){
    split_seed = random_seed.split(" ").reverse();
    
    console.log(binary);
    
    processing = binary;
    bit_norm();
    split_seed.forEach(function(pair){
        if      (pair[0] == "0"){rAdder(parseInt(pair[1]))}
        else if (pair[0] == "1"){rCycle(parseInt(pair[1]))}
        else if (pair[0] == "2"){rReverse(parseInt(pair[1]))}
        else if (pair[0] == "3"){rSplit(parseInt(pair[1]))}
    });
    
    return processing;
}

String.prototype.repeat = function(num) {
    return new Array(num + 1).join(this)
}

function H(bits, key) {
    key = key_norm(bits.length, key);
    
    result = "";
    for(i=0; i<bits.length; i++){
        if(bits[i] == key[i]){result += "1"}
        else{result += "0"}
    }
    
    return result;
}

function r_H(bits, key){
    // H is its own inverse
    return H(bits, key);
}

function encryptor(text, key){
    return H(G(encode(text), key), key).toString();
}

function decryptor(binary,key){
    return decode(r_G(r_H(binary, key), key).toString());
}

