#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<fstream>
using namespace std;

#define TABLE_COLUMN 12
#define TABLE_ROW 12
FILE *fpWrite;
/**
 * 定义各种状态
 */
typedef enum {
    S0, S1,	S2, S3, S4, S5,  S6, S7, S8, S9, S10, S11, S12, S13, ERR 
} STATE;

/**
 * 关键字 
 */ 
string keywords[10] = {
	"if", "else", "while", "for", "int", "write", "read"};

/**
 * 状态转换表 
 */
int transform_table[][12] = {
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11},
	{S0,S1,S2,S3,S4,S5,S5,S7,S8,S9,S8,ERR},
	{S1,S1,S1,S1,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S2,ERR,S2,S2,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S3,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S4,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S5,ERR,ERR,ERR,ERR,ERR,S6,ERR,ERR,ERR,ERR,ERR},
	{S6,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S7,ERR,ERR,ERR,ERR,ERR,S6,ERR,ERR,ERR,ERR,ERR},
	{S8,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
	{S9,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,S10,ERR},
	{S10,S10,S10,S10,S10,S10,S10,S10,S10,S10,S11,S10},
	{S11,S10,S10,S10,S10,S10,S10,S10,S10,S12,S11,S10},
	{S12,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR}
};
//记录终态的个数
int END_STATE_LEN = 9;

char *state_class[100];	//用来存放对终态的描述
//终态集合
STATE end_state[] = {
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,
	S8,
	S9,
	S12,
	S13
};

STATE jump(STATE now, char c);
int in_end_state(STATE now);
void init_state_class();
int get_column(char ch);
void print_char(char *str, int s, int e, STATE now);
void analyse(char *line);
void init_state_class();

/**
 * 定义终态的类型
 */
void init_state_class() {
	state_class[S1] = "ID";
	state_class[S2] = "Number";
	state_class[S3] = "0";
	state_class[S4] = "分隔符";
	state_class[S5] = "赋值符号";
	state_class[S6] = "双运算符";
	state_class[S8] = "单运算符";
	state_class[S9] = "注释";
	state_class[S12] = "";
}
/**
 * 根据字符判断改类字符在判定表中对应的列
 * 因为判定表是的类型为int，所以对字符而言存储长度仅限于一位
 */
int get_column(char ch) {
	if (ch >= '1' && ch <= '9') {
		return 2;
	} else if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
		return 1;
	} else if (ch == '0') {
		return 3;
	} else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';' || ch == ',' ) {
		return 4;
	} else if (ch == '<' || ch == '>') {
		return 5;
	} else if (ch == '+'|| ch == '-') {
		return 8;
	} else if (ch == '!' ) {
		return 7;
	} else if (ch == '=' ) {
		return 6;
	} else if(ch =='/') {
		return 9;
	} else if (ch=='*') {
		return 10;
	} else {
		return 11;
	}
}
/**
 * 分析字符函数
 * 调用状态跳转函数
 *  在控制台中报错
 */
void analyse(char *line) {
	STATE now = S0;	//设置当前状态为S0
	int index = 0;	// index ~ i 这中间的字串被视为以一个整体
	int len = strlen(line);
	for(int i = 0; i < len; i++) {
		char c = line[i];
		if( c == ' ' || c == '\n' || c == '\t') {
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
		} else if(!in_end_state(now) && jump(now,line[i+1])==ERR || now == ERR) {
			//从索引位置输出到当前位置
			for (int j = index; j <= i; j++) {
				cout <<line[j];
			}
			cout  << "\t错误！" << endl;
			index +=1;
			now = S0;
		}
	}
	if(!in_end_state(now) && index < len) {
		//从索引位置输出到当前位置
		for (int j = index; j <= len; j++) {
			cout <<line[j];
		}
		cout  << "\t错误！" << endl;
		index +=1;
		now = S0;
	}
}

/**
 * 状态跳转
 * 根据状态转换表的第一列（每行第一个元素）求出当前的行号
 * 调用 get_column() 求出对应的列号。
 * 返回新的状态
 */
STATE jump(STATE now, char c) {
	STATE new_state;
	int row_idx = 0;  // 求当前状态行号
	for (int i = 1; i < TABLE_ROW; i++) {
		if (now == transform_table[i][0]) {
			row_idx = i;
			break;
		}
	}
	//读入当前字符，求得应该跳转的列号
	int col_idx = get_column(c);
	new_state = (STATE) transform_table[row_idx][col_idx];
	return new_state;
}

/**
 * 查看是否在终态集合中
 */
int in_end_state(STATE now) {
	for (int i = 0; i < END_STATE_LEN; i++) {
		if (end_state[i] == now) {
			return 1; // 在终态集合中
		}
	}
	return 0; // 不在终态集合中
}
/**
 * 判断到底是哪个关键字 
 */
int iskeyWorld(string word) {
	
	for(int i = 0; i < 10; i++) {
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
void print_char(char *str, int s, int e, STATE now) {
	string word;
	for (int i = s; i <= e; i++) {
		word += str[i]; 
	}
	string snow;
	if(now == S1 && iskeyWorld(word)) {
		snow = word;
	} else if(now == S5 || now == S6 || now == S7 || now == S4 || now == S8) {
		snow = word;
	} else {
		snow = state_class[now];
	}
	string strState =  "\n" + snow + "\t";
	for (int j = 0; j < strState.length(); j++) {
		fprintf(fpWrite,"%c",strState[j]);
	}
	for (int i = s; i <=e; i++) {

		fprintf(fpWrite,"%c",str[i]);
	}
}



