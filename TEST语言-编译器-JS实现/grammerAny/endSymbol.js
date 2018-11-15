const endSymble = [`{`,`int`,`;`,`(`,`)`,`=`,`>`,`<=`,`write`,'read',`for`,`+`,`-`,`if`,'else',`while`,`}`,`ID`,`NUM`,`*`,`/`];
/**
 * 判断是否为终结符
 */
module.exports = function isTerminator(word) {
  for(let i in endSymble ){
    if(endSymble[i] == word){
      return true;
    }
  }
  return false;
}