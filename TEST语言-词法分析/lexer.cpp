#include<iostream>
#include "wordAnalysis.h"
using namespace std;

int main() {
	char buffer[2048];
	fstream file;
	file.open("./lex.txt", ios::in);
	fpWrite=fopen("out.txt","w");
	init_state_class();
	// һ���Զ�ȡ�ļ��������ݣ�ֱ������EOF 
	file.get(buffer, 2048, EOF);
	//�ʷ����� 
	analyse(buffer);
	file.close(); 
	return 0;
}

