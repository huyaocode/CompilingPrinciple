#include<fstream>
#include<string.h>
#include <iostream>
#include <map>

using namespace std;

#define TABLE_COLUMN 19
//#define TABLE_ROW 11
#define TABLE_ROW 13

/**
 * 定义各种状态
 */
typedef enum {
    START,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    ERR // 错误状态
    // TODO 写出足够的状态
} STATE;

/**
 * 正则表达式所对应的状态转换表
 */
int transform_table[][20] = {
        {-1,/*   */  'a',/*A-Za-z*/    1,/*1-9*/    0,/*  0  */   '+',/*  */ '-',/*  */ '*',/*  */ '/', '=', '<', '>', '(', ')', '{', '}', ';', ',', '!', 'o'/*其他的字符*/},
        {START,/* */ A,/*           */ B,/*       */C,/*       */ E,/*   */  E,/*    */ E,/*    */ H,   F,   F,   F,   D,   D,   D,   D,   D,   D,   G,   ERR},
        {A,/*     */ A,                A,           A,            ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {B,          ERR,              B,           B,            ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {C,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {D,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {E,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {F,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, E,   ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {G,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, E,   ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {H,          ERR,              ERR,         ERR,          ERR,       ERR,       I,         ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR},
        {I,          I,                I,           I,            I,         I,         J,         I,   I,   I,   I,   I,   I,   I,   I,   I,   I,   I,   I},
        {J,          I,                I,           I,            I,         I,         J,         K,   I,   I,   I,   I,   I,   I,   I,   I,   I,   I,   I},
        {K,          ERR,              ERR,         ERR,          ERR,       ERR,       ERR,       ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR}
};

int reserved_word_count = 7;
char *reserved_word[100] = {
        "if",
        "else",
        "for",
        "while",
        "int",
        "write",
        "read"
};

string get_word(char *s, int start, int end) {
    string word;
    for (int i = start; i <= end; i++) {
        word += s[i];
    }
    return word;
}

/**
 * 判断是否是保留字
 * @param s
 * @param start
 * @param end
 * @return
 */
int is_reserved_word(string word) {
    for (int i = 0; i < 7; i++) {
        if (word == reserved_word[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 * 根据输入符号返回转换表中的列
 * @param ch 输入的符号
 * @return
 */
int get_column(char ch) {
    if (ch >= '1' && ch <= '9') {
        return 2;
    } else if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z') {
        return 1;
    } else if (ch == '0') {
        return 3;
    } else {
        for (int i = 4; i < TABLE_COLUMN; i++) {
            if (ch == transform_table[0][i]) {
                return i;
            }
        }
        return TABLE_COLUMN - 1;
    }
}

char *state_class[100]; // 终态集合
char *err_class[100];   // 错误集合
/*接受状态的集合*/
STATE end_state[] = {A/*标识符*/, B/*无符号整数*/, C/*无符号整数*/, D/*分隔符*/, E/*运算符*/, F/*运算符*/, H/*运算符*/, K/*注释*/};
int end_state_len = 8;

map<string, int> word_class;

int get_word_class(STATE state, string word) {
    if (state == K) { // 是注释的话
        return word_class["**"];
    } else {
        return word_class[word];
    }
}

void init_word_class() {
    word_class["NUM"] = 1;
    word_class["ID"] = 2;
    word_class["("] = 3;
    word_class[")"] = 4;
    word_class[";"] = 5;
    word_class["{"] = 6;
    word_class["}"] = 7;
    word_class[","] = 8;
    word_class["+"] = 9;
    word_class["-"] = 10;
    word_class["*"] = 11;
    word_class["/"] = 12;
    word_class["="] = 13;
    word_class[">"] = 14;
    word_class["<"] = 15;
    word_class[">="] = 16;
    word_class["<="] = 17;
    word_class["!="] = 18;
    word_class["=="] = 19;
    word_class["if"] = 20;
    word_class["else"] = 21;
    word_class["for"] = 22;
    word_class["while"] = 23;
    word_class["int"] = 24;
    word_class["write"] = 25;
    word_class["read"] = 26;
    word_class["**"] = 27; // 注释类别
    word_class["err"] = 1000; // 识别不了就为错误
}

void init_state_class() {
    state_class[A] = "ID";
    state_class[B] = "NUM";
    state_class[C] = "NUM";
    state_class[D] = "own"; /*分隔符*/
    state_class[E] = "own"; /*运算符*/
    state_class[F] = "own"; /*运算符*/
    state_class[H] = "own"; /*运算符*/
    state_class[K] = "own";  /*注释*/
}

void init_err_class() {
    err_class[I] = "未闭合多行注释";
    err_class[J] = "未闭合多行注释";
    err_class[G] = "!用法不对";
}

/**
 *  根据状态转换表的状态转化函数
 */
STATE new_state(STATE now, char ch) {
    STATE new_state;
    // 求出当前状态行号
    int row_idx = 0;
    for (int i = 1; i < TABLE_ROW; i++) {
        if (now == transform_table[i][0]) {
            row_idx = i;
        }
    }
    int col_idx = get_column(ch);
    new_state = (STATE) transform_table[row_idx][col_idx];
    return new_state;
}

/**
 * 查看是否在终态集合中
 * @param now
 * @return
 */
int in_end_state(STATE now) {
    for (int i = 0; i < end_state_len; i++) {
        if (end_state[i] == now) {
            return 1; // 在终态集合中
        }
    }
    return 0; // 不在终态集合中
}

int line_number = 1;

void judge(char *test) {
    ofstream lex("./词法分析结果.txt");
    int test_len = strlen(test);
    STATE now = START;
    int idx = 0;
    for (int i = 0; i < test_len; i++) {
        if (test[i] == '\n') {
            line_number++;
        }
        if (test[i] == ' ' || test[i] == '\n' || test[i] == '\t') {
            if (idx == i) {
                idx++;
            }
            continue;
        }
        // 不识别的字符
        if (now == ERR || now == START && new_state(now, test[i]) == ERR && test[i] != ' ') {
            idx = i + 1;
            now = START;
            cout << " ---- " << (test[i] - '0') << endl;
            continue;
        }

        now = new_state(now, test[i]);
        if (in_end_state(now) && new_state(now, test[i + 1]) == ERR) {
            string word = get_word(test, idx, i);

            if (strcmp(state_class[now], "own") == 0 || is_reserved_word(word) == 1) {
                // 符号或保留字的情况输出自己
                lex <<word;
            } else {
                lex << state_class[now];
            }
            lex <<"    " << word << "\n";
            idx = i + 1;
            now = START;
        } else if (!in_end_state(now) && new_state(now, test[i + 1]) == ERR) {
            cout << err_class[now] << endl;
            idx = i + 1;
            now = START;
        }
    }

    if (!in_end_state(now)) {
        cout << err_class[now] << endl;
    }
    lex.close();
}

int main() {
    init_word_class();
    init_state_class();
    init_err_class();
    char buffer[10000];
    fstream file;
    file.open("./目标代码.txt", ios::in);
    while (!file.eof()) {
        file.read(buffer, 10000);
        judge(buffer);
    }
    file.close();
    return 0;
}

