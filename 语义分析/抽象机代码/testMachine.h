#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int TESTmachine();

int TESTmachine()
{
    int es=0,i,k=0,codecount,stack[1000],stacktop=0;
    char Codein[300],code[1000][20],data[1000];//用于接收输入文件名
    int label[100]= {0};
    char lno[4];
    FILE *fin;//用于指向输入文件的指针
    if((fin = fopen("lex.txt","rt"))==NULL)
    {
        printf("\n打开%s错误！\n",code);
        es=10;
        return(es);
    }
    codecount=0;
    i=fscanf(fin,"%s",&code[codecount]);

    while(!feof(fin))
    {
        printf("0000  %d  %s  \n",codecount,code[codecount]);
        i=strlen(code[codecount])-1;
        if(code[codecount][i]==':')
        {
            i=i-5;
            strncpy(lno,&code[codecount][5],i);
            lno[i]='\0';
            label[atoi(lno)]=codecount;//用label数组记住每个标号的地址
            printf("label[ %d]=   %d  \n",atoi(lno),label[atoi(lno)]);
            code[codecount][0]=':';
            code[codecount][1]='\0';
            strcat(code[codecount],lno);
            k++;
        }
        codecount++;
        print("到这里--1");
        i=fscanf(code[codecount],lno);
        print("到这里--2");
//i=fscanf(fin,"%s",&code[codecount]);
    }
    fclose(fin);
    for(i=0; i<10; i++)
        printf("label%d  %d  \n",i,label[i]);


    for(i=0; i<codecount; i++)
    {
        int l;
        l=strlen(code[i]);
        printf("label[2]=%d  %d  %s  %d\n",label[2],i,code[i],l);

        if((l>1) && (code[i][1]=='A'))
        {
            strncpy(lno,&code[i][5],l-5);
            printf("1111111111label[2]=%d lno=%s\n",label[2],lno);
            lno[l-5]='\0';
            itoa(label[atoi(lno)],code[i],10);
        }
    }

    for(k=0; k<5; k++)
        printf("label%d  %d  \n",k,label[k]);

    i=0;
    while(i<codecount)
    {
        printf("code %d  %s\n",i,code[i]);
        if(strcmp(code[i],"LOAD")==0) //LOAD D将D中的内容加载到操作数栈
        {
            i++;
            stack[stacktop]=data[atoi(code[i])];
            stacktop++;
        }
        if(strcmp(code[i],"LOADI")==0) //LOADI a将常量a压入操作数栈
        {
            i++;
            stack[stacktop]=atoi(code[i]);
            stacktop++;
        }
        //STO D将操作数栈栈顶单元内容存入D，且栈顶单元内容保持不变
        if(strcmp(code[i],"STO")==0)
        {
            i++;
            data[atoi(code[i])]=stack[stacktop-1];
            printf("sto  stack %d\n",stack[stacktop-1]);
            printf("sto data %d\n",data[atoi(code[i])]);
        }
        //POP栈顶单元内容出栈
        if(strcmp(code[i],"POP")==0)
        {
            stacktop--;
        }
        //ADD将次栈顶单元与栈顶单元内容出栈并相加，和置于栈顶
        if(strcmp(code[i],"ADD")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]+stack[stacktop-1];
            printf("add   %d\n",stack[stacktop-1]);
            stacktop--;
        }
        //SUB 将次栈顶单元减去栈顶单元内容并出栈，差置于栈顶
        if(strcmp(code[i],"SUB")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]-stack[stacktop-1];
            stacktop--;
        }
        //MULT 将次栈顶单元与栈顶单元内容出栈并相乘
        if(strcmp(code[i],"MULT")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]*stack[stacktop-1];
            stacktop--;
        }
        //DIV 将次栈顶单元与栈顶单元内容出栈并相除
        if(strcmp(code[i],"DIV")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]/stack[stacktop-1];
            stacktop--;
        }
        //BR lab 无条件转移到lab
        if(strcmp(code[i],"BR")==0)
        {
            i++;
            i=atoi(code[i]);
        }
        //BRF lab 检查栈顶单元逻辑值并出栈,若为假(0)则转移到lab
        if(strcmp(code[i],"BRF")==0)
        {
            i++;
            if(stack[stacktop-1]==0)
                i=atoi(code[i]);
            stacktop--;
        }
        //EQ 将栈顶两单元做等于比较并出栈，并将结果真或假(1或0)置于栈顶
        if(strcmp(code[i],"EQ")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]==stack[stacktop-1];
            stacktop--;
        }
        //NOTEQ将栈顶两单元做不等于比较并出栈，并将结果真或假(1或0)置于栈顶
        if(strcmp(code[i],"NOTEQ")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]!=stack[stacktop-1];
            stacktop--;
        }
        //GT 次栈顶大于栈顶操作数并出栈，则栈顶置1，否则置0
        if(strcmp(code[i],"GT")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]>stack[stacktop-1];
            stacktop--;
        }
        //LES 次栈顶小于栈顶操作数并出栈，则栈顶置1，否则置0
        if(strcmp(code[i],"LES")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]<stack[stacktop-1];
            stacktop--;
        }
        //GE 次栈顶大于等于栈顶操作数并出栈，则栈顶置1，否则置0
        if(strcmp(code[i],"GE")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]>=stack[stacktop-1];
            stacktop--;
        }
        //LE 次栈顶小于等于栈顶操作数并出栈，则栈顶置1，否则置0
        if(strcmp(code[i],"LE")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]<=stack[stacktop-1];
            stacktop--;
        }
        //AND 将栈顶两单元做逻辑与运算并出栈，并将结果真或假(1或0)置于栈顶
        if(strcmp(code[i],"AND")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]&&stack[stacktop-1];
            stacktop--;
        }
        //OR 将栈顶两单元做逻辑或运算并出栈，并将结果真或假(1或0)置于栈顶
        if(strcmp(code[i],"OR")==0)
        {
            stack[stacktop-2]=stack[stacktop-2]||stack[stacktop-1];
            stacktop--;
        }
        //NOT 将栈顶的逻辑值去反
        if(strcmp(code[i],"NOT")==0)
        {
            stack[stacktop-1]!=stack[stacktop-1];
        }
        //IN 从标准输入设备(键盘)读入一个整型数据，并入栈
        if(strcmp(code[i],"IN")==0)
        {
            printf("请输入数据:");
            scanf("%d",&stack[stacktop]);
            stacktop++;
        }
        //OUT 将栈顶单元内容出栈，并输出到标准输出设备上(显示器)
        if(strcmp(code[i],"OUT")==0)
        {
            printf("程序输出%d\n",stack[stacktop-1]);
            stacktop--;
        }
        //STOP 停止执行
        if(strcmp(code[i],"STOP")==0)
        {
            break;
        }
        i++;
        //显示栈顶内容
        //for(k=stacktop-1;k>=0;k--)
            //printf("  %d\n",stack[k]);
        printf("   %d\n   %d\n",data[0],data[1]);
    }
    return(es);
}





















