const {S0, S1, S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12, ERR} = require('./states')

/**
 * 状态转换表
 */
const transform_table = [
  [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
  [S0, S1, S2, S3, S4, S5, S5, S7, S8, S9, S8, ERR],
  [S1, S1, S1, S1, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S2, ERR, S2, S2, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S3, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S4, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S5, ERR, ERR, ERR, ERR, ERR, S6, ERR, ERR, ERR, ERR, ERR],
  [S6, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S7, ERR, ERR, ERR, ERR, ERR, S6, ERR, ERR, ERR, ERR, ERR],
  [S8, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR],
  [S9, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, S10, ERR],
  [S10, S10, S10, S10, S10, S10, S10, S10, S10, S10, S11, S10],
  [S11, S10, S10, S10, S10, S10, S10, S10, S10, S12, S11, S10],
  [S12, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR]
];


/**
 * 状态跳转
 * 根据状态转换表的第一列（每行第一个元素）求出当前的行号
 * 调用 get_column() 求出对应的列号。
 * 返回新的状态
 */
module.exports = function jump(now, c) {
  let row_idx = 0; // 求当前状态行号
  for (let i = 1; i < transform_table.length; i++) {
    if (now == transform_table[i][0]) {
      row_idx = i;
      break;
    }
  }
  //读入当前字符，求得应该跳转的列号
  let col_idx = get_column(c);
  let new_let = transform_table[row_idx][col_idx];
  return new_let;
}

function get_column(ch) {
  // console.log('ch', ch)
  if (ch >= '1' && ch <= '9') {
    return 2;
  } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return 1;
  } else if (ch == '0') {
    return 3;
  } else if (
    ch == '(' ||
    ch == ')' ||
    ch == '{' ||
    ch == '}' ||
    ch == ';' ||
    ch == ','
  ) {
    return 4;
  } else if (ch == '<' || ch == '>') {
    return 5;
  } else if (ch == '+' || ch == '-') {
    return 8;
  } else if (ch == '!') {
    return 7;
  } else if (ch == '=') {
    return 6;
  } else if (ch == '/') {
    return 9;
  } else if (ch == '*') {
    return 10;
  } else {
    return 11;
  }
}