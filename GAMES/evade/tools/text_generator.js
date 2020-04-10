// JAVASCRIPT To generate this stuff below: (Paste in JSFIddle.net)
var map = {
    'A' : 0,
    'B' : 1,
    'C' : 2,
    'D' : 3,
    'E' : 4,
    'F' : 5,
    'G' : 6,
    'H' : 7,
    'I' : 8,
    'J' : 9,
    'K' : 10,
    'L' : 11,
    'M' : 12,
    'N' : 13,
    'O' : 14,
    'P' : 15,
    'Q' : 16,
    'R' : 17,
    'S' : 18,
    'T' : 19,
    'U' : 20,
    'V' : 21,
    'W' : 22,
    'X' : 23,
    'Y' : 24,
    'Z' : 25,
    ':' : 26,
    '.' : 27,
    ' ' : 255
}

var credits = [
     "CREDITS",
     "Jay Garcia",
     "Simon Prickett",
     "Stan Bershadskiy", 
     "Andrew Owen", 
     "Andy Dennis", 
     "Tim Eagan", 
     "Drew Griffith", 
     "JD Jones", 
     "Jon Van Dalen", 
     "Lucas Still", 
     "Matt McCants",
     "Play",
     "Settings",
     "Credits"
]

var x = 0;

var endStr = '\n\n';

credits.forEach(function(creditStr, index) {
  creditStr = creditStr.toUpperCase();
    
    var header = `\n\n//${creditStr}\nPROGMEM const uint8_t credits${index}[] = {\n`,
      outStr = '';
    creditStr = creditStr.split('');
    
    
    creditStr.forEach(function(str) {
      if (outStr.length > 0) {
          outStr += ', '; 
        }
      outStr += map[str];
    });
    
    outStr = creditStr.length +',// Number characters\n' + outStr + '\n};';
    
    //console.log(header, outStr)
    endStr += (header + outStr);
    
})

console.log(endStr);
