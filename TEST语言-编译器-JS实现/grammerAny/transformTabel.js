const transformTabel=[
  [null,'{','int',';','(',')','=','>','<=','write','read','for','+','-','if','while','}','ID','NUM','*','/'],
  ['<program>', '{|<declaration_list_2>|<statement_list_2>|}',,,,,,,,,,,,,,,,,,,],
  ['<declaration_list_2>',,'<declaration_stat>|<declaration_list_2>','#',,,,,,'#','#','#',,,'#','#',,'#','#',,],
  ['<declaration_stat>',,'int|ID|;',,,,,,,,,,,,,,,,,,],
  ['<statement_list_2>','<statement>|<statement_list_2>',,,'<statement>|<statement_list_2>',,,,,'<statement>|<statement_list_2>','<statement>|<statement_list_2>','<statement>|<statement_list_2>',,,'<statement>|<statement_list_2>','<statement>|<statement_list_2>','<statement>|<statement_list_2>','<statement>|<statement_list_2>','<statement>|<statement_list_2>',,],
  ['<statement>','<expression_stat>',,,'<expression_stat>',,,,,'<write|_stat>','<read_stat>','<for|_stat>',,,'<if|_stat>','<while|_stat>',,'<expression_stat>','<expression_stat>',,],
  ['<if_stat>',,,,,,,,,,,,,,'if|(|<expression>|)|<statement>|','if|(|<expression>|)|<statement>|else|<statement>',,,,,,],
  ['<while_stat>',,,,,,,,,,,,,,,'while|(|<expression>|)|<statement>',,,,,],
  ['<for_stat>',,,,,,,,,,,'for|(|<expression>|;|<expression>|;|<expression>|)|<statement>',,,,,,,,,],
  ['<write_stat>',,,,,,,,,'write|<expression>',,,,,,,,,,,],
  ['<read_stat>',,,,,,,,,,'read|ID|;',,,,,,,,,,],
  ['<compound_stat>','{|<statement_list>|}',,,,,,,,,,,,,,,,,,,],
  ['<expression_stat>','<expression>|;',,';',,,,,,,,,,,,,,'<expression>|;','<expression>|;',,],
  ['<expression>','<bool_expr>',,,,,,,,,,,,,,,,'ID|=|<bool_expr>|<bool_expr>','<bool_expr>',,],
  ['<arithmatic_expression_2>',,,,,,,,,,,,'+|<term>|<arithmatic_expression_2>','-|<term>|<arithmatic_expression_2>',,,,,,,],
  ['<term_2>',,,'#',,'#',,'#','#',,,,'#','#',,,,,,'*|<factor>|<term_2>','/|<factor>|<term_2>'],
]

module.exports=function(state ,word){
  let row = 0, col = 0;
  for(let i in transformTabel) {
    if(transformTabel[i][0] == state) {
      row = i;
    }
  }
  for(let j in transformTabel[0]) {
    if(transformTabel[0][j] == word) {
      col = j;
    }
  }
  if(transformTabel[row][col]) {
    return transformTabel[row][col];
  } else {
    return null;
  }
}