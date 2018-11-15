const fs = require('fs');

module.exports = function(state, word) {
  let transformTabel = [];
  const file = fs.readFileSync('./grammerAny/LL1table.csv', 'utf-8');
  transformTabel = file.split('\r\n');
  for (let i in transformTabel) {
    transformTabel[i] = transformTabel[i].split(',');
  }
  let row = 0,
    col = 0;
  for (let i in transformTabel) {
    if (transformTabel[i][0] == state) {
      row = i;
    }
  }
  for (let j in transformTabel[0]) {
    if (transformTabel[0][j] == word) {
      col = j;
    }
  }
  let trans = transformTabel[row][col];
  if(trans) {
    return trans;
  } else {
    return null;
  }
};
