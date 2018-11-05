#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<fstream>
using namespace std;

#define TABLE_COLUMN 12
#define TABLE_ROW 12
FILE *fpWrite;
/**
 * �������״̬
 */
typedef enum {
    S0, S1,	S2, S3, S4, S5,  S6, S7, S8, S9, S10, S11, S12, S13, ERR 
} STATE;

/**
 * �ؼ��� 
 */ 
string keywords[10] = {
	"if", "else", "while", "for", "int", "write", "read"};

/**
 * ״̬ת���� 
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
//��¼��̬�ĸ���
int END_STATE_LEN = 9;

char *state_class[100];	//������Ŷ���̬������
//��̬����
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
 * ������̬������
 */
void init_state_class() {
	state_class[S1] = "ID";
	state_class[S2] = "Number";
	state_class[S3] = "0";
	state_class[S4] = "�ָ���";
	state_class[S5] = "��ֵ����";
	state_class[S6] = "˫�����";
	state_class[S8] = "�������";
	state_class[S9] = "ע��";
	state_class[S12] = "";
}
/**
 * �����ַ��жϸ����ַ����ж����ж�Ӧ����
 * ��Ϊ�ж����ǵ�����Ϊint�����Զ��ַ����Դ洢���Ƚ�����һλ
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
 * �����ַ�����
 * ����״̬��ת����
 *  �ڿ���̨�б���
 */
void analyse(char *line) {
	STATE now = S0;	//���õ�ǰ״̬ΪS0
	int index = 0;	// index ~ i ���м���ִ�����Ϊ��һ������
	int len = strlen(line);
	for(int i = 0; i < len; i++) {
		char c = line[i];
		if( c == ' ' || c == '\n' || c == '\t') {
			if(index == i) {
				index+=1;
			}
			continue;
		}
		//״̬��ת
		now = jump(now, line[i]);
		//����ǰ״̬Ϊ��̬������һ״̬�Ǵ���״̬ʱ
		if (in_end_state(now) && jump(now,line[i+1])==ERR) {
			print_char(line, index, i, now);
			index = i + 1;	//״̬ת����index��Ҫ�������ڷ������ַ�����һ��
			now = S0;
		} else if(!in_end_state(now) && jump(now,line[i+1])==ERR || now == ERR) {
			//������λ���������ǰλ��
			for (int j = index; j <= i; j++) {
				cout <<line[j];
			}
			cout  << "\t����" << endl;
			index +=1;
			now = S0;
		}
	}
	if(!in_end_state(now) && index < len) {
		//������λ���������ǰλ��
		for (int j = index; j <= len; j++) {
			cout <<line[j];
		}
		cout  << "\t����" << endl;
		index +=1;
		now = S0;
	}
}

/**
 * ״̬��ת
 * ����״̬ת����ĵ�һ�У�ÿ�е�һ��Ԫ�أ������ǰ���к�
 * ���� get_column() �����Ӧ���кš�
 * �����µ�״̬
 */
STATE jump(STATE now, char c) {
	STATE new_state;
	int row_idx = 0;  // ��ǰ״̬�к�
	for (int i = 1; i < TABLE_ROW; i++) {
		if (now == transform_table[i][0]) {
			row_idx = i;
			break;
		}
	}
	//���뵱ǰ�ַ������Ӧ����ת���к�
	int col_idx = get_column(c);
	new_state = (STATE) transform_table[row_idx][col_idx];
	return new_state;
}

/**
 * �鿴�Ƿ�����̬������
 */
int in_end_state(STATE now) {
	for (int i = 0; i < END_STATE_LEN; i++) {
		if (end_state[i] == now) {
			return 1; // ����̬������
		}
	}
	return 0; // ������̬������
}
/**
 * �жϵ������ĸ��ؼ��� 
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
 * ��str��s��eд�뵽�ļ���
 * �������ǰ��̬������
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



