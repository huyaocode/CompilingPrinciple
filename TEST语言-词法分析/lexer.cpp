#include<iostream>
#include "wordAnalysis.h"
using namespace std;

int main() {
	char buffer[2048];
	fstream file;
	file.open("./lex.txt", ios::in);
	fpWrite=fopen("out.txt","w");
	init_state_class();
	// 一次性读取文件所有内容，直到碰到EOF 
	file.get(buffer, 2048, EOF);
	//词法分析 
	analyse(buffer);
	file.close(); 
	return 0;
}

