// Voyage Functional Encryption Software: VFES
// Copyright Linus Lee 2015
// Contact linus@thelifelongtraveler.com

// could encrypt text stupidly easily by making text string into ASCII strings

var e_bin="01011010101001010101111000011100101101000";

var decrypted = e_bin, // some binary data (currently 16-bit)
    processing = "", // bit string in processing
    encryptedS = "", // bit string encrypted, but only the input string
    encryptedA = "", // only encrypted algorithm
    encrypted = "", // some encrypted binary data
    empty = 0, // null int value
    key = "0110100110010110"; // usually will be the previous message tho
    
var zerofiller = "00000000000000";
var onefiller  = "11111111111111";

function rset(n){
    rxe = new RegExp(".{1," + n.toString() + "}","g");
    return rxe;
}

// G elements
function adder(n){ // formerly "inverse", when applied only to 1-bit string objects
    // this function is finalized!
    stringBits = processing.match(rset(n));
    totalBits = "";

    stringBits.forEach(function(bitstring){
        e = (parseInt(bitstring, 2) + 1).toString(2);
        if(e.length<n){e=zerofiller.substr(0,n-e.length) + e};
        if(e.length == n+1){
            e = zerofiller.substr(0,n);
        }
        totalBits += e;
    });
    processing = totalBits;
}
function rAdder(n){ // inverse function of adder()
    // this function is finalized!
    stringBits = processing.match(rset(n));
    totalBits = "";

    stringBits.forEach(function(bitstring){
        e=""
        if(bitstring==zerofiller.substr(0,n)){
            e=onefiller.substr(0,n)}
        else{
        e = (parseInt(bitstring, 2) - 1).toString(2);
        }
        if(e.length<n){e=zerofiller.substr(0,n-e.length) + e};
        if(e.length == n+1){
            e = zerofiller.substr(0,n);
        }
        totalBits += e;
    });
    processing = totalBits;
}
function cycle(n){
    // this function is finalized!
    stringBits = processing.match(rset(n));
    totalBits = "";

    stringBits.forEach(function(bitstring){
        e = bitstring.substring(1, n) + bitstring[0];
        totalBits += e;
    });
    processing = totalBits;
}
function rCycle(n){
    // this function is finalized!
    stringBits = processing.match(rset(n));
    totalBits = "";

    stringBits.forEach(function(bitstring){
        e = bitstring[n-1] + bitstring.substring(0,n-1)
        totalBits += e;
    });
    processing = totalBits;
}

function reverse(n){
    // this function is finalized!
    stringBits = processing.match(rset(n));
    processing = stringBits.reverse().join("");
}
function rReverse(n){
    // this function is finalized!
    reverse(n);
}

function split(n){
    // this function is finalized!
    bitsarray = new Array(n);
    for(i=0;i<n;i++){bitsarray[i]=""}
    for(i=0;i<processing.length;i++){
        bitsarray[i%n]+=processing[i]
    }
    processing=bitsarray.join("");
}
function rSplit(n){
    // this function is finalized!
    bitsarray = processing.match(rset(processing.length / n));
    processing="";
    for(i=0;i<bitsarray.join("").length;i++){
        processing+=bitsarray[i%n][Math.floor(i/n)];
    }
}

// Modules:
var random_seed;

function rs_g(){
    // random seed generator
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
    if(random_seed.length%3!=0){random_seed+="2"};

    return random_seed.toString();
}

function G(random_seed){
    split_seed = random_seed.split(" ");

    processing = decrypted;
    adder(8);rAdder(8); // for normalization purposes
    split_seed.forEach(function(pair){
        if      (pair[0] == "0"){adder(parseInt(pair[1]))}
        else if (pair[0] == "1"){cycle(parseInt(pair[1]))}
        else if (pair[0] == "2"){reverse(parseInt(pair[1]))}
        else if (pair[0] == "3"){split(parseInt(pair[1]))}
    });

    encrypted = processing;
    return encrypted;
}

function r_G(binary, random_seed){
    split_seed = random_seed.split(" ").reverse();

    processing = binary;
    adder(8);rAdder(8);
    split_seed.forEach(function(pair){
        if      (pair[0] == "0"){rAdder(parseInt(pair[1]))}
        else if (pair[0] == "1"){rCycle(parseInt(pair[1]))}
        else if (pair[0] == "2"){rReverse(parseInt(pair[1]))}
        else if (pair[0] == "3"){rSplit(parseInt(pair[1]))}
    });

    decrypted = processing;
    return decrypted;
}

String.prototype.repeat=function(num){return new Array(num+1).join(this)}

function H(bits, key){
    key=key.split(" ").join("");

    // this can be later developed into a more secure algorithm, but for now it's very simple: an xor operation
    mln=bits.length;
    xln=key.length;
    exr=mln%xln;
    key=key.repeat(parseInt((mln-exr)/xln))+key.substr(0,exr);

    bits=bits.split("");
    key=key.split("");
    result=[];
    for(i=0;i<mln;i++){
        if(bits[i]+key[i]==1){result.push("1")}
        else{result.push("0")}
    }
    
    return result.join("");
}
    
function r_H(bits,key){
    // for now, the H hash function is its inverse; an xor operation
    return H(bits,key);
}
function encryptor(binary){
    decrypted = binary;
    key = rs_g();

    return H(G(key),key).toString() + " [" + key + "]";
}

function decryptor(binary,key){
    deckey = key.split(" ").reverse().join(" ");

    decrypted=binary;
    key = key;

    return r_H(r_G(key),key).toString();
}

console.log(decrypted);
console.log(G(rs_g()));

/* main encryption process
   - Use the key to generate a one-time bit-flicking encryption algorithm. This is G
   - Use the key to generate a one-time bit-switching encryption algorithm. This is H.
   - The encrypted binary X = H(G(plainText));
*/


