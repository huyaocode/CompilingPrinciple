const {S0, S1, S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12, ERR} = require('./states')
const jump = require('./jumpState')
let returnStr = '';
const END_STATE_LEN = 9;
/**
 * 关键字
 */
const keywords = ['if', 'else', 'while', 'for', 'int', 'write', 'read'];

/**
 * 查看是否在终态集合中
 */
function in_end_state(now) {

  for (let i = 0; i < END_STATE_LEN; i++) {
    if (
      now == S1 ||
      now == S2 ||
      now == S3 ||
      now == S4 ||
      now == S5 ||
      now == S6 ||
      now == S8 ||
      now == S9 ||
      now == S12
    ) {
      return true; // 在终态集合中
    }
  }
  return false; // 不在终态集合中
}
/**
 * 判断到底是哪个关键字
 */
function iskeyWorld(word) {
  for (let i = 0; i < 10; i++) {
    if (word == keywords[i]) {
      return 1;
    }
  }
  return 0;
}

/**
 * 将str从s到e写入到文件中
 * 并输出当前终态的名称
 */
function print_char(str, s, e, now) {
  let word = '';
  for (let i = s; i <= e; i++) {
    word += str[i];
  }
  let snow = '';
  if (now == S1 && iskeyWorld(word)) {
    snow = word;
  } else if (now == S5 || now == S6 || now == S7 || now == S4 || now == S8) {
    snow = word;
  } else {
    snow = now;
  }
  let strState = '\n' + snow + '\t';

  for (let j = 0; j < strState.length; j++) {
    returnStr += strState[j];
  }
  for (let i = s; i <= e; i++) {
    returnStr += str[i];
  }
}

/**
 * 分析字符函数
 * 调用状态跳转函数
 *  在控制台中报错
 */
module.exports = function analyse(line) {
  let now = S0;	//设置当前状态为S0
	let index = 0;	// index ~ i 这中间的字串被视为以一个整体
  let len = line.length;
	for(let i = 0; i < len; i++) {
		let c = line[i];
		if( c == ' ' || c == '\n' || c == '\t' || c == '\r') {
			if(index == i) {
				index+=1;
			}
			continue;
    }
		//状态跳转
    now = jump(now, line[i]);
		//当当前状态为终态并且下一状态是错误状态时
		if (in_end_state(now) && jump(now,line[i+1])==ERR) {
			print_char(line, index, i, now);
			index = i + 1;	//状态转换后，index就要跳到正在分析的字符的下一个
			now = S0;
		} else if((!in_end_state(now) && jump(now,line[i+1])==ERR) || now == ERR) {
      //从索引位置输出到当前位置
      let s = ''
			for (let j = index; j <= i; j++) {
				s += line[j];
			}
      console.log(s + '\t 错误')
			index += 1;
			now = S0;
		}
  }
  // 当文档结尾时没有跳到终态
	if(!in_end_state(now) && index < len) {
    //从索引位置输出到当前位置
    let s = "";
		for (let j = index; j <= len; j++) {
			s += line[j];
    }
    console.log(s+ '\t 错误');
		index +=1;
		now = S0;
	}
  return returnStr;
};
