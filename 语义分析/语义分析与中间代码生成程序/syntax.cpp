//语法、语义分析及代码生成
#include<stdio.h>
#include<stype.h>
#include<ctype.h>
#include<conio.h>
#define maxvartablep 500  //定义符号表容量
int TESTparse();
int program();
int compound_stat();
int statement();
int arithmetic_expression();
int assignment_expression();
int assignment_stat();
int bool_expression();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int declaration_list();
int statement_list();
int compound_stat();
int name_def(char *name);
char token[20],token1[40];//token保存单词符号，token1保存单词值
extern char Scanout[300],Codeout[300];//保存词法分析输出文件名
FILE * fp, * fout;//用于指向输入输出文件的指针
struct{//定义符号表结构
    char name[8];
    int address;
}vartable[maxvartablep];
int vartablep=0,labelp=0,datap=0;   //改符号表最多容纳maxvartablep个记录

//插入符号表动作@name-def n，t的程序如下：
int name_def(char *name)
{
    int i,es=0;
    if(vartablep >= maxvartablep)   return(21);
    for(i=vartablep-1;i==0;i--)    //查符号表
    {
        if(strcmp(vartable[i].name,name)==0)
        {
            es=22;     //22表示变量重复声明
            break;
        }
    }
    if(es>0)    return(es);
    strcpy(vartable[vartablep].name,name);
    vartable[vartablep].address=datap;
    datap++;         //分配一个单元，数据区指针加1
    vartablep++;
    return(es);
}

//查询符号表返回地址
int lookup(char *name,int *paddress)
{
    int i,es=0;
    for(i=0;i<vartablep;i++)
    {
        if(strcmp(vartable[i].name,name)==0)
        {
            *paddress=vartable[i].address;
            return(es);
        }
    }
    es=23;
    return(es);   //变量没有声明
}

//语法、语义分析及代码生成程序
int TESTparse()
{
    int es=0;
    if((fp=fopen(Scanout,"r"))==NULL)
    {
        printf("\n打开%s错误!\n",Scanout);
        es=10;
        return(es);
    }
    printf("请输入目标文件名（包含路径）：");
    scanf("%s",Codeout);
    if((fout=fopen(Codeout,"w"))==NULL)
    {
        printf("\n创建%s错误!\n",Codeout);
        es=10;
        return(es);
    }
    if(es==0)   es=program();
    printf("==语法、语义分析及代码生成程序结果==\n");
    switch(es)
    {
        case 0:printf("语法、语义分析成功并抽象机汇编生成代码!\n");break;
        case 10:printf("打开文件%s失败!\n",Scanout);break;
        case 1:printf("缺少{!\n");break;
        case 2:printf("缺少}!\n");break;
        case 3:printf("缺少标识符!\n");break;
        case 4:printf("少分号!\n");break;
        case 5:printf("缺少(!\n");break;
        case 6:printf("缺少)!\n");break;
        case 7:printf("缺少操作数!\n");break;
        case 21:printf("符号表溢出!\n");break;
        case 22:printf("变量重复定义!\n");break;
        case 23:printf("变量未声明!\n");break;
    }
    fclose(fp);
    fclose(fout);
    return(es);
}

//program::={<declaration_list><statement_list>}
int program()
{
    int es=0,i;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"{"))    //判断是否'{'
    {
        es=1;
        return(es);
    }
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=declaration_list();
    if(es>0)    return(es);
    printf("    符号表\n");
    printf("    名字    地址\n");
    for(i=0;i<vartablep;i++)
        printf("   %s   %d\n",vartable[i].name,vartable[i].address);
    es=statement_list();
    if(es>0)    return(es);
    if(strcmp(token,"}"))   //判断是否'}'
    {
        es=2;
        return(es);
    }
    fprintf(fout,"    STOP\n");    //产生停止指令
    return(es);
}

//
//
int declaration_list()
{
    int es=0;
    while(strcmp(token,"int")==0)
    {
        es=declaration_stat();
        if(es>0)    return (es);
    }
    return(es);
}

//
int declaration_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"ID"))  return(es=3);   //不是标识符
    es=name_def(token1);                    //插入符号表
    if(es>0)    return(es);
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,";"))   return(es==4);
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    return(es);
}

//
//
int statement_list()
{
    int es=0;
    while(strcmp(token,"}"))
    {
        es=statement();
        if(es>0)    return(es);
    }
    return (es);
}

//
//
int statement()
{
    int es=0;
    if(es==0 && strcmp(token,"if")==0)  es=if_stat();  //if语句
    if(es==0 && strcmp(tokenm"while")==0)   es=while_stat();  //while语句
    if(es==0 && strcmp(tokenm"for")==0)   es=for_stat();  //for语句
    if(es==0 && strcmp(tokenm"read")==0)   es=read_stat();  //read语句
    if(es==0 && strcmp(tokenm"write")==0)   es=write_stat();  //write语句
    if(es==0 && strcmp(tokenm"{")==0)   es=compound_stat();  //复合语句
    if(es==0 && (strcmp(token,"ID")==0 )  //赋值语句
    es=assignment_stat();
    return(es);
}

int if_stat()
{
    int es=0,label1,label2;                //if
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return(es=5);  //少左括号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    if(strcmp(token,")"))   return(es=6);  //少右括号
    label1=labelp++;   //用label1记住条件为假时要转向的标号
    fprintf(fout,"      BRF LABEL %d\n",label1);  //输出假转移指令
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    label2=labelp++;  //用label2记住要转向的标号
    fprintf(fout,"      BR LABEL %d\n",label2);  //输出无条件转移指令
    fprintf(fout,"LABEL%d:\n",label1);  //设置label1要标记的符号
    if(strcmp(token,"else")==0)  //else部分处理
    {
        fscanf(fp,"%s %s\n",&token,&token1);
        printf("%s %s\n",token,token1);
        es=statement();
        if(es>0)    return(es);
    }
    fprintf(fout,"LABEL%d:\n",label2);  //设置label2记住的标号
    return(es);
}

int while_stat()
{
    int es=0,label1,label2;
    label1=labelp++;
    fprintf(fout,"LABEL%d:\n",label1);//设置label1标号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return(es=5);//少左括号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    if(strcmp(token,")"))   return(es=6);//少右括号
    label2=labelp++;
    fprintf(fout,"      BRF LABEL %d\n",label2);//输出假转移指令
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    fprintf(fout,"      BR LABEL %d\n",label1);//输出无条件转移指令
    fprintf(fout,"LABEL%d:\n",label2);//设置label2标号
    return(es);
}

int for_stat()
{
    int es=0,label1,label2,label3,label4;
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return (es=5);//少左括号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=assignment_expression();
    if(es>0)    return(es);
    if(strcmp(token,";"))   return(es=4);//少分号
    label1=labelp++;
    fprintf(fout,"LABEL%d:\n",label1);//设置label1标号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    label2=labelp++;
    fprintf(fout,"      BRF LABEL%d\n",label2);//输出假条件转移指令
    label3=labelp++;
    fprintf(fout,"      BR LABEL%d\n",label3);//输出无条件转移指令
    if(strcmp(token,";"))   return(es=4);//少分号
    label4=labelp++;
    fprintf(fout,"LABEL%d\n",label4);//设置label4标号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=assignment_ expression();
    if(es>0)    return(es);
    fprintf(fout,"      POP\n");//输出出栈指令
    fprintf(fout,"      BR LABEL%d\n",label1);//输出无条件转移指令
    if(strcmp(token,")"))   return(es=6);//缺少右括号
    fprintf(fout,"LABEL%d:\n",label3);//设置label3标号
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    fprintf(fout,"      BR LABEL%d\n",label4);//输出无条件转移指令
    fprintf(fout,"LABEL%d:\n",label2);//设置label2标号
    return(es);

}

int write_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    es=arithmetic_expression();
    if(es>0)    return(es);
    if(strcmp(token,";"))   return(es=4);//少分号
    fprintf(fout,"      OUT\n");
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    return(es);
}

int read_stat()
{
    int es=0,address;
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"ID"))  return(es=3);//少标识符
    es=lookup(token1,&address);
    if(es>0)    return(es);
    fprintf(fout,"      IN  \n");//输出指令
    fprintf(fout,"      STO  %d\n",address);//输出STO指令
    fprintf(fout,"      POP  \n");
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,";"))   return(es=4);//少分号
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    return(es);
}

int compound_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    es=statement_list();
    return(es);
}

int assignment_expression()
{	int es = 0, address;
	if (strcmp(token,"ID")) return es=9; 
    es = lookup(token1,&address) ;
	if (es > 0)return es;
	fscanf(fp,"%s %s\n",&token,&token1);
	if (strcmp(token,“=")) return es =10;
    fscanf(fp,"%s %s\n",&token,&token1);
    es = arithmetic_expression ();
	if (es > 0)return es;
	fprintf(fout,“ STO %d\n",address);
	fprintf(fout,“ POP\n");
}
int assignment_stat()
{
//请添加代码
}
int bool_expression()
{
    int es=0;
    es=arithmetic_expression();
    if(es>0)    return(es);
    if(strcmp(token,">")==0 || strcmp(token,">=")==0
       || strcmp(token,"<")==0 || strcmp(token,"<=")==0
       || strcmp(token,"==")==0 || strcmp(token,"!=")==0)
    {
        char token2[20];
        strcpy(token2,token);//保存运算符
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
        es=arithmetic_expression();
        if(es>0)    return(es);
        if(strcmp(token2,">")==0)   fprintf(fout,"      GT\n");
        if(strcmp(token2,">=")==0)   fprintf(fout,"      GE\n");
        if(strcmp(token2,"<")==0)   fprintf(fout,"      LES\n");
        if(strcmp(token2,"<=")==0)   fprintf(fout,"      LE\n");
        if(strcmp(token2,"==")==0)   fprintf(fout,"      EQ\n");
        if(strcmp(token2,"!=")==0)   fprintf(fout,"      NOTEQ\n");
    }
    return(es);
}

int arithmetic_expression()
{
    int es=0;
    es=term();
    if(es>0)    return(es);
    while(strcmp(token,"+")==0 || strcmp(token,"-")==0)
    {
        char token2[20];
        strcpy(token2,token);//保存运算符
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
        es=term();
        if(es>0)    return(es);
        if(strcmp(token2,"+")==0)   fprintf(fout,"      ADD\n");
        if(strcmp(token2,"-")==0)   fprintf(fout,"      SUB\n");
    }
    return(es);
}

int term()
{
    int es=0;
    es=factor();
    if(es>0)    return(es);
    while(strcmp(token,"*")==0 || strcmp(token,"/")==0)
    {
        char token2[20];
        strcpy(token2,token);
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
        es=factor();
        if(es>0)    return(es);
        if(strcmp(token2,"*")==0)   fprintf(fout,"      MULT\n");
        if(strcmp(token2,"/")==0)   fprintf(fout,"      DIV\n");
    }
    return(es);
}

int factor()
{
    int es=0;
    if(strcmp(token,"(")==0)
    {
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
        es=arithmetic_expression();
        if(es>0)    return(es);
        if(strcmp(token,")"))   return(es=6);//缺少右括号
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
    }else
    {
        if(strcmp(token,"ID")==0)
        {
            int address;
            es=lookup(token1,&address);//查符号表，获取变量地址
            if(es>0)    return(es);
            fprintf(fout,"      LOAD%d\n",address);
            fscanf(fp,"%s %s\n",token,&token1);
            printf("%s %s\n",token,token1);
            return(es);
        }
        if(strcmp(token,"NUM")==0)
        {
            fprintf(fout,"      LOADI %s\n",token1);
            fscanf(fp,"%s %s\n",token,&token1);
            printf("%s %s\n",token,token1);
            return(es);
        }else
        {
            es=7;//缺少操作数
            return(es);
        }
    }
    return(es);
}





