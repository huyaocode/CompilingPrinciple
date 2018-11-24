//�﷨�������������������
#include<stdio.h>
#include<stype.h>
#include<ctype.h>
#include<conio.h>
#define maxvartablep 500  //������ű�����
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
char token[20],token1[40];//token���浥�ʷ��ţ�token1���浥��ֵ
extern char Scanout[300],Codeout[300];//����ʷ���������ļ���
FILE * fp, * fout;//����ָ����������ļ���ָ��
struct{//������ű�ṹ
    char name[8];
    int address;
}vartable[maxvartablep];
int vartablep=0,labelp=0,datap=0;   //�ķ��ű��������maxvartablep����¼

//������ű���@name-def n��t�ĳ������£�
int name_def(char *name)
{
    int i,es=0;
    if(vartablep >= maxvartablep)   return(21);
    for(i=vartablep-1;i==0;i--)    //����ű�
    {
        if(strcmp(vartable[i].name,name)==0)
        {
            es=22;     //22��ʾ�����ظ�����
            break;
        }
    }
    if(es>0)    return(es);
    strcpy(vartable[vartablep].name,name);
    vartable[vartablep].address=datap;
    datap++;         //����һ����Ԫ��������ָ���1
    vartablep++;
    return(es);
}

//��ѯ���ű��ص�ַ
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
    return(es);   //����û������
}

//�﷨������������������ɳ���
int TESTparse()
{
    int es=0;
    if((fp=fopen(Scanout,"r"))==NULL)
    {
        printf("\n��%s����!\n",Scanout);
        es=10;
        return(es);
    }
    printf("������Ŀ���ļ���������·������");
    scanf("%s",Codeout);
    if((fout=fopen(Codeout,"w"))==NULL)
    {
        printf("\n����%s����!\n",Codeout);
        es=10;
        return(es);
    }
    if(es==0)   es=program();
    printf("==�﷨������������������ɳ�����==\n");
    switch(es)
    {
        case 0:printf("�﷨����������ɹ��������������ɴ���!\n");break;
        case 10:printf("���ļ�%sʧ��!\n",Scanout);break;
        case 1:printf("ȱ��{!\n");break;
        case 2:printf("ȱ��}!\n");break;
        case 3:printf("ȱ�ٱ�ʶ��!\n");break;
        case 4:printf("�ٷֺ�!\n");break;
        case 5:printf("ȱ��(!\n");break;
        case 6:printf("ȱ��)!\n");break;
        case 7:printf("ȱ�ٲ�����!\n");break;
        case 21:printf("���ű����!\n");break;
        case 22:printf("�����ظ�����!\n");break;
        case 23:printf("����δ����!\n");break;
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
    if(strcmp(token,"{"))    //�ж��Ƿ�'{'
    {
        es=1;
        return(es);
    }
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=declaration_list();
    if(es>0)    return(es);
    printf("    ���ű�\n");
    printf("    ����    ��ַ\n");
    for(i=0;i<vartablep;i++)
        printf("   %s   %d\n",vartable[i].name,vartable[i].address);
    es=statement_list();
    if(es>0)    return(es);
    if(strcmp(token,"}"))   //�ж��Ƿ�'}'
    {
        es=2;
        return(es);
    }
    fprintf(fout,"    STOP\n");    //����ָֹͣ��
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
    if(strcmp(token,"ID"))  return(es=3);   //���Ǳ�ʶ��
    es=name_def(token1);                    //������ű�
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
    if(es==0 && strcmp(token,"if")==0)  es=if_stat();  //if���
    if(es==0 && strcmp(tokenm"while")==0)   es=while_stat();  //while���
    if(es==0 && strcmp(tokenm"for")==0)   es=for_stat();  //for���
    if(es==0 && strcmp(tokenm"read")==0)   es=read_stat();  //read���
    if(es==0 && strcmp(tokenm"write")==0)   es=write_stat();  //write���
    if(es==0 && strcmp(tokenm"{")==0)   es=compound_stat();  //�������
    if(es==0 && (strcmp(token,"ID")==0 )  //��ֵ���
    es=assignment_stat();
    return(es);
}

int if_stat()
{
    int es=0,label1,label2;                //if
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return(es=5);  //��������
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    if(strcmp(token,")"))   return(es=6);  //��������
    label1=labelp++;   //��label1��ס����Ϊ��ʱҪת��ı��
    fprintf(fout,"      BRF LABEL %d\n",label1);  //�����ת��ָ��
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    label2=labelp++;  //��label2��סҪת��ı��
    fprintf(fout,"      BR LABEL %d\n",label2);  //���������ת��ָ��
    fprintf(fout,"LABEL%d:\n",label1);  //����label1Ҫ��ǵķ���
    if(strcmp(token,"else")==0)  //else���ִ���
    {
        fscanf(fp,"%s %s\n",&token,&token1);
        printf("%s %s\n",token,token1);
        es=statement();
        if(es>0)    return(es);
    }
    fprintf(fout,"LABEL%d:\n",label2);  //����label2��ס�ı��
    return(es);
}

int while_stat()
{
    int es=0,label1,label2;
    label1=labelp++;
    fprintf(fout,"LABEL%d:\n",label1);//����label1���
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return(es=5);//��������
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    if(strcmp(token,")"))   return(es=6);//��������
    label2=labelp++;
    fprintf(fout,"      BRF LABEL %d\n",label2);//�����ת��ָ��
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    fprintf(fout,"      BR LABEL %d\n",label1);//���������ת��ָ��
    fprintf(fout,"LABEL%d:\n",label2);//����label2���
    return(es);
}

int for_stat()
{
    int es=0,label1,label2,label3,label4;
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))   return (es=5);//��������
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=assignment_expression();
    if(es>0)    return(es);
    if(strcmp(token,";"))   return(es=4);//�ٷֺ�
    label1=labelp++;
    fprintf(fout,"LABEL%d:\n",label1);//����label1���
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=bool_expression();
    if(es>0)    return(es);
    label2=labelp++;
    fprintf(fout,"      BRF LABEL%d\n",label2);//���������ת��ָ��
    label3=labelp++;
    fprintf(fout,"      BR LABEL%d\n",label3);//���������ת��ָ��
    if(strcmp(token,";"))   return(es=4);//�ٷֺ�
    label4=labelp++;
    fprintf(fout,"LABEL%d\n",label4);//����label4���
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=assignment_ expression();
    if(es>0)    return(es);
    fprintf(fout,"      POP\n");//�����ջָ��
    fprintf(fout,"      BR LABEL%d\n",label1);//���������ת��ָ��
    if(strcmp(token,")"))   return(es=6);//ȱ��������
    fprintf(fout,"LABEL%d:\n",label3);//����label3���
    fscanf(fp,"%s %s\n",&token,&token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0)    return(es);
    fprintf(fout,"      BR LABEL%d\n",label4);//���������ת��ָ��
    fprintf(fout,"LABEL%d:\n",label2);//����label2���
    return(es);

}

int write_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    es=arithmetic_expression();
    if(es>0)    return(es);
    if(strcmp(token,";"))   return(es=4);//�ٷֺ�
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
    if(strcmp(token,"ID"))  return(es=3);//�ٱ�ʶ��
    es=lookup(token1,&address);
    if(es>0)    return(es);
    fprintf(fout,"      IN  \n");//���ָ��
    fprintf(fout,"      STO  %d\n",address);//���STOָ��
    fprintf(fout,"      POP  \n");
    fscanf(fp,"%s %s\n",token,&token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,";"))   return(es=4);//�ٷֺ�
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
	if (strcmp(token,��=")) return es =10;
    fscanf(fp,"%s %s\n",&token,&token1);
    es = arithmetic_expression ();
	if (es > 0)return es;
	fprintf(fout,�� STO %d\n",address);
	fprintf(fout,�� POP\n");
}
int assignment_stat()
{
//����Ӵ���
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
        strcpy(token2,token);//���������
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
        strcpy(token2,token);//���������
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
        if(strcmp(token,")"))   return(es=6);//ȱ��������
        fscanf(fp,"%s %s\n",token,&token1);
        printf("%s %s\n",token,token1);
    }else
    {
        if(strcmp(token,"ID")==0)
        {
            int address;
            es=lookup(token1,&address);//����ű���ȡ������ַ
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
            es=7;//ȱ�ٲ�����
            return(es);
        }
    }
    return(es);
}





