# 词法分析

下载本项目， 使用Dev C++ 运行lexer.cpp即可得到结果

分析代码：
```
{
	/*This a test program.*/
	int abc;
	int 1203;
	int A$@;
	int i;
	int n;
	int b,c;
	int 2a;
	int a2;
	read n;
	n = 012345;
	for (i=1;i<=n; i= i+1)
	{
	abc=abc+i;
	}
	if(i!=n) n = n+i;
	if (!n) b = b+c;
	/*The loop ended
	write abc;
}
```
![报错信息](https://upload-images.jianshu.io/upload_images/13627346-6e4541184ff6fbbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![分析结果](https://upload-images.jianshu.io/upload_images/13627346-111272ff499551fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 源代码


设计词法分析程序
###  一、实验目的
1、根据 TEST 语言的词法规则，分别写出每条词法规则对应的正则表达式；
2、将每一个正则表达式转换为 NFA；
3、将多个 NFA 合并后进行确定化并化简；
4、根据化简后的 DFA 画出流程图； 
5、参阅在实验语言部分给出的 TEST 语言语法规则，确定单词分类、单词输出方案；
6、编写词法分析程序；
7、对下面的 TEST 语言源程序进行词法分析，将合法单词存入 lex.txt，并报告词法错	误及其位置。

###  二、实验设计
1、写出TEST语言每条词法规则对应的正则文法或者正则表达式
标识符 (a|…|z|A|…|Z)( a|…|z|A|…|Z|0|…|9)*
保留字 <if>|<else>|<for>|< while>|< do>|< int>|< write>|< read>
无符号整数 (1|…|9)(0|…|9)*|0
分界符 ( | ) | ; | { | }
运算符 +|-|*|/|=|<|>|>=|<=|!=|==
注释符 /* (其他符号)* */

2、对每个文法或者正则表达式分别构造NFA

![标识符](https://upload-images.jianshu.io/upload_images/13627346-497c2e3a17f9f44c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![保留字](https://upload-images.jianshu.io/upload_images/13627346-23e46698c018c868.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![无符号整数](https://upload-images.jianshu.io/upload_images/13627346-149ddc8824a16e68.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![分界符](https://upload-images.jianshu.io/upload_images/13627346-33586da570b4734f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![运算符](https://upload-images.jianshu.io/upload_images/13627346-ef153908621178da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

3.将NFA合并，确定化，化简得到最终的DFA。
![合并后的NFA](https://upload-images.jianshu.io/upload_images/13627346-9668bbcbbdf7cc1a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![状态转换表](https://upload-images.jianshu.io/upload_images/13627346-d0a73f7248c1c84e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1、设计的词法分析程序是否满足最长匹配原则？如果满足请给出实现方案。如果不满	足请给出改进方案。
最长匹配原则定义：指识别出的单词有混淆时，按长度最长的来决定。例如，“<=”是	一个符号串，而不是分开成“<”和“=”单个字符。
我组设计的词法分析程序符合最长匹配原则。
实现方案：画出DFA和状态表之后，按照表驱动进行编写程序，达到目的。详细实现方	案请看源码和状态转移图。

2、给出单词分类方案，并说明理由。 
根据TEST语言可将单词分为六类：
①　标识符：字母开头，后可接任意字母或数字； 
②　保留字：标识符的子集，包括：if、else、for、while、do、int、write、read;
③　无符号整数: 由数字组成，但最高位不能为0，允许一位的0；
④　分界符： (、 ) 、; 、{ 、}
⑤　运算符：+、-、*、/、=、<、>、>=、<=、!=、==
⑥　注释符： /* 、 */

3、构建词法分析程序一般过程是怎样的？
![分析过程](https://upload-images.jianshu.io/upload_images/13627346-cdaa491710e22860.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

