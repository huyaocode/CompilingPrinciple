const fs = require('fs');
const path = require('path');

const inputfile = __dirname + '/' + 'input.txt';
const outputfile = __dirname + '/' + 'output.txt';

let file = fs.readFileSync(inputfile, 'utf-8');

fileLen = file.length;
file = file.split('');

let state = 0;  //状态量
let step = 0;   //走了几步
let i = 0;      //走到的位置
//lexer
function lexer() {
    
    for (i = 0; i < fileLen; i++) {
        let ch = file[i];
        switch (state) {
            case 0:
                if (ch == 'a') {
                    state = 1; step++;
                } else if (ch == 'b') {
                    state = 2; step++;
                } else {
                    wrong();
                }
                break;
            case 1:
                if (ch == 'a') {
                    state = 3; step++;
                } else if (ch == 'b') {
                    state = 2; step++;
                } else {
                    wrong();
                }
                break;
            case 2:
                if (ch == 'a') {
                    state = 1; step++;
                } else if (ch == 'b') {
                    state = 3; step++;
                } else {
                    wrong();
                }
                break;
            case 3:
                if (ch == 'a') {
                    state = 3; step++;
                } else if (ch == 'b') {
                    state = 3; step++;
                } else if (step > 0) {
                    output(i, step);
                }
                else {
                    wrong();
                }
                break;
        }
    }
    output(fileLen, step);
}

function wrong(){
    console.log('不满足语法');
    step = 0; 
    state = 0;
}

//将解析成功的词写到output.txt，并打印
function output(i, step) {
    let valuabel = file.slice(i - step, i).join('');
    console.log(valuabel)
    fs.appendFileSync(outputfile, valuabel + '\n', 'utf8');
    step = 0; state = 0;
}


lexer();
