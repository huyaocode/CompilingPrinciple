/**
 * 语法分析
 */
const isTerminator = require('./endSymbol');
const jumpState = require('./transformTabel');
//符号栈
const stack = [];
//剩余符号
const remainWords = [];
//输出字符串
let returnStr = '';

module.exports = function(res) {
  const strList = res.split('\n');
  for (let i in strList) {
    let kv = strList[i].split('\t');
    if (kv.length == 2) {
      remainWords.push({
        type: kv[0],
        value: kv[1]
      });
    }
  }
  remainWords.push({
    type: '#',
    value: '#'
  });

  stack.push('#');
  stack.push('<program>');
  print('符号栈', '', '读入符号', '剩余符号', '使用产生式');

  handleList();

  return returnStr;
};

/**
 * 将剩余字符串的第一个符号弹出
 */
function getsym() {
  return remainWords.shift();
}

/**
 * 主程序， 处理
 * @param {*}
 */
function handleList() {
  //当剩余字符串长度大于0
  let word = getsym(); //取出剩余字串的第一个
  while (remainWords.length > 0) {
    let list = [];
    const state = stack.pop(); //栈顶弹出一个元素（最后一个）
    //如果是终结符
    if (isTerminator(state)) {
      if (word.type == state) {
        word = getsym();
        while (word.type == 'Notes') {
          word = getsym();
        }
        //打印输出
        print(stack, state, word.type, remainWords, null);
        continue;
      } else {
        error(state, word);
        continue;
        break;
      }
    } else if (state === '#') {
      console.log('解析完成');
      returnStr += '\n解析完成'
    }
    // 是非终结符， 那么查表。进行压栈操作
    else if ((list = jumpState(state, word.type))) {
      //打印输出
      const trans = state + '->' + list.split('|').join('');
      print(stack, state, word.type, remainWords, trans);
      //逆序压栈
      list = list.split('|');
      for (let j = list.length - 1; j >= 0; j--) {
        if (list[j] != '#') {
          stack.push(list[j]);
        }
      }
    } else {
      error(state, word);
      break;
    }
  }
}

function error(state, word) {
  const errStr =
    'Error 无法接收这个符号,应该为：' +
    state +
    ' 但实际上是' +
    word.type +
    '  ' +
    word.value +
    '\n';
  returnStr += errStr;
}

/**
 * 打印一行
 */
function print(stack, state, word, remainWords, useProduce) {
  let s;
  if (stack instanceof Array) {
    s = stack.join(' ') + ' ' + state;
  } else {
    s = stack;
  }
  s += printSpace(s, 80);
  let w = word;
  w += printSpace(w, 20);
  let r;
  if (stack instanceof Array) {
    r = getWords(remainWords);
  } else {
    r = stack;
  }
  r += printSpace(r, 230);
  let u = useProduce;
  u += printSpace(u, 100);
  const str = s + w + r + u + '\n';
  returnStr += str;
}

/**
 * 打印相关
 * 得到剩余字符串
 */
function getWords() {
  let str = '';
  for (let i = 0; i < remainWords.length; i++) {
    str += remainWords[i].value + ' ';
  }
  return str;
}

/**
 * 打印相关
 * 计算空格数，为了整齐的打印空格
 */
function printSpace(word, len) {
  let wordLen;
  if (word == null) {
    wordLen = len;
  } else {
    wordLen = word.length;
    if (len - word.length < 0) {
      console.log('小于0');
      console.log(word);
    }
  }
  let space = '';
  for (let i = 0; i < len - wordLen; i++) {
    space += ' ';
  }
  return space;
}

/**
push(#); push(S);      //把#和开始符号S依次压进栈
a=getsym( ) ;  //读入第一个符号给a
flag=1;
while (flag) {
    X=pop( ) ;//从栈中弹出X 
    if (x == 注释) {
      continue；
    }
    if(X ∈ VT )   //  当栈顶元素是一个终结符
	// 如果终结符 == 当前输入字串
       if(X ==a)   a=getsym( ) ; //读入下一个符号给a  OK这个字符
       else error();	//报错
    else if(X ==‘#’)	//结束了
       if(X ==a)   flag=0 ;//分析成功 
       else error();
// 是非终结符， 那么查表。进行压栈操作
    else if(M[X,a]==X → X1X2…Xn)// X ∈Vn查分析表
        {X=pop( ) ; push(Xn…X2X1);//若X1X2…Xn= ε，则不进栈}
    else error();
}//end of while
 */
