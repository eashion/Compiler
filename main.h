#include <iostream>
#include <string>
#include <fstream>
using namespace std;
 
//using ::isalpha; //是否字母 
//using ::iscntrl; //是否控制符 
//using ::isdigit; //是否是数字 
//using ::isspace; //是否空格
  
//32个关键字,sting key[]  等同于  char *key[]
string key[32]={"auto","break","case","char","const","continue","default","do","double","else","enum","extern"
                 ,"float","for","goto","if","int","long","register","return","short","signed","sizeof","static"
                 ,"struct","switch","typedef","union","unsigned","void","volatile","while"};
//分界符
string boundary[9]={",",";","'","\"","(",")","{","}","#"};
//运算符
string operators[40]={"+","-","*","/","%","++","--",//算术运算符(0-6)
                    ">","<","==",">=","<=","!=",//关系运算符(7-12)
                    "!","&&","||",//逻辑运算符(13-15)
                    "<<",">>","~","|","^","&",//位运算符(16-21)
                    "=","+=","-=","*=","/=","%=",">>=","<<=","&=","^=","|=",//赋值运算符(22-32)
                    "?",":",//条件运算符(33-34)
                    ".","->",//分量运算符(35-36)
                    "[","]"//下标运算符(37-38)
                    ,"\\"};                  //转义运算符(39)
#define BO 9   //分界符总数                   
#define OP 40  //运算符总数
#define OP1 6  //运算符分类
#define OP2 12
#define OP3 15
#define OP4 21
#define OP5 32
#define OP6 34
#define OP7 36
#define OP8 38
 
void initdat();//初始化各表
void alpha(char);//字符串处理
void digit(char);//数字串处理
void other(char);//其它字符处理
void error(char,int);//出错处理(字符)
void error(string,int);//出错处理(提示)
void output(string,int);//类型记号输出
void output(string,int,int);//类型记号输出
void setcategory(int);//运算符类型设置
int findid(string);//求标识符在标识符表中的位置