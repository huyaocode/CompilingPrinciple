const fs = require('fs')
const wordAny = require('./wordAny');
const grammerAny = require('./grammerAny');
//读源文件
const file = fs.readFileSync('./lex.txt', 'utf-8');

//词法分析
const wordCutStr = wordAny(file);
//写入文件
fs.writeFileSync('./dist/wordCut.txt', wordCutStr, 'utf-8')

//语法分析
const grammerAnyStr = grammerAny(wordCutStr);
//写入文件
fs.writeFileSync('./dist/grammerAny.txt', grammerAnyStr, 'utf-8')