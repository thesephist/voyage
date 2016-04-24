#!/usr/bin/env node
// Mediator for command-line Voyage-X / ISAC0-X encryption script

// runtime args
var args = process.argv;
var voyage = require('/home/thesephist/voyager/x-voyage.js');
var options = [];
var version = 0.5; // software version

// parameters
var message = "";
var vkey = "";
var out_type = "hex";
var will_encrypt = true;

function hasAlphabet(str) {
    if (str.match(/[a-z]/i)) {
        return true;
    }
}

args.forEach(function(query){
    if (query.substr(0, 2) == "--") {
        options.push([query.substr(2), args[args.indexOf(query) + 1]]);
    } else if (query[0] == "-") {
        options.push([query.substr(1), args[args.indexOf(query) + 1]]);
    }
});

if (options.length == 0) {
    process.exit(1);
}

options.forEach(function(option){
    if (option[0][0] == "k") {
        // process encryption key
        if (hasAlphabet(option[1])) {
            vkey = voyage.rs_w(option[1]);
        } else {
            vkey = option[1];
        }
    }
    if (option[0][0] == "t") {
        // process output type
        if(option[1] == "binary") {
            out_type = undefined;
        }
    }
    if (option[0][0] == "m") {
        // process message to be ingested
        message = option[1];
        if (option[1] == undefined) {
            console.error("ERROR: No payload or message specified");
            process.exit(1);
        }
    }
    if (option[0][0] == "e") {
        // is this an encryption operation?
        will_encrypt = true;
    }
    if (option[0][0] == "d") {
        // is this a decryption operation?
        will_encrypt = false;
    }
    if (option[0][0] == "h") {
        // display help and exit
        console.log("Voyage-X / ISAC0-X Node Console\n");
        console.log("    -k    --key [STRING], specifies a pre-generated encryption key\n");
        console.log("    -t    --type [STRING], specifies output type as either 'hex' or");
        console.log("            'binary'. This value defaults to 'hex'\n");
        console.log("    -m    --message [STRING], specifies message to be encrypted or payload to be decrypted\n");
        console.log("    -d    --decrypt, specifies a decryption operation. This is not default\n");
        console.log("    -e    --encrypt, specifies an encryption operation. This is the default behavior\n");
        console.log("    -h    --help, displays help message and exits\n");
        console.log("    -v    --version, displays current version number and exits\n");
        process.exit(0);
    }
    if (option[0][0] == "v") {
        // display version number and exit
        console.log("Voyage-X / ISAC0-X Node Console Version: " + version);
        process.exit(0);
    }
    // Future options possibilities
    // - write to and read from text files
});

if (message == "") {
    console.error("ERROR: No payload or message specified");
    process.exit(1);
}

if (will_encrypt == false && vkey == "") {
    console.error("ERROR: No key specified for decryption operation");
    process.exit(1);
}

if (vkey == "") {
    vkey = voyage.rs_g(30, 40);
    console.log("EMPHEMERAL KEY: " + vkey);
}

if (will_encrypt) {
    out = voyage.encryptor(message, vkey, out_type);
    console.log(out);
} else {
    out = voyage.decryptor(message, vkey, out_type);
    console.log(out);
}

