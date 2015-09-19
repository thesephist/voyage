// Voyage Functional Encryption Software: VFES
// Element: Voyage statistical analysis

// run through frequencies of 1, 2, 4-bit chunks using rest(n) and lists += 1 counters

var bitstream = "";

function rset(n){
    rxe = new RegExp(".{1," + n.toString() + "}","g");
    return rxe;
}

matcher = [
    ["0","1"],
    ["00","01","10","11"],
    ["000","001","010","011","100","101","110","111","011"],
    ["0000","0001","0010","0100","1000","1001","1010","1100","1011","1101","1110","1111","0111","0101","0110","0011"]
];

versions = [
    bitstream.match(rset(1)),
    bitstream.match(rset(2)),
    bitstream.match(rset(3)),
    bitstream.match(rset(4))
];

records = [
    [0,0],
    [0,0,0,0],
    [0,0,0,0,0,0,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
];

for (i=0; i<versions.length; i++) {
    record = new Array(i + 1);
    poss = Math.pow(2, i + 1);
    versions[i].forEach(function(key){
        for (j=0; j<poss; j++) {
            if (key == matcher[i][j]) {
                records[i][j] += 1;
            }
        }
    });
}

console.log(records);




