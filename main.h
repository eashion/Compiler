#include <iostream>
#include <string>
#include <fstream>
using namespace std;
 
//using ::isalpha; //�Ƿ���ĸ 
//using ::iscntrl; //�Ƿ���Ʒ� 
//using ::isdigit; //�Ƿ������� 
//using ::isspace; //�Ƿ�ո�
  
//32���ؼ���,sting key[]  ��ͬ��  char *key[]
string key[32]={"auto","break","case","char","const","continue","default","do","double","else","enum","extern"
                 ,"float","for","goto","if","int","long","register","return","short","signed","sizeof","static"
                 ,"struct","switch","typedef","union","unsigned","void","volatile","while"};
//�ֽ��
string boundary[9]={",",";","'","\"","(",")","{","}","#"};
//�����
string operators[40]={"+","-","*","/","%","++","--",//���������(0-6)
                    ">","<","==",">=","<=","!=",//��ϵ�����(7-12)
                    "!","&&","||",//�߼������(13-15)
                    "<<",">>","~","|","^","&",//λ�����(16-21)
                    "=","+=","-=","*=","/=","%=",">>=","<<=","&=","^=","|=",//��ֵ�����(22-32)
                    "?",":",//���������(33-34)
                    ".","->",//���������(35-36)
                    "[","]"//�±������(37-38)
                    ,"\\"};                  //ת�������(39)
#define BO 9   //�ֽ������                   
#define OP 40  //���������
#define OP1 6  //���������
#define OP2 12
#define OP3 15
#define OP4 21
#define OP5 32
#define OP6 34
#define OP7 36
#define OP8 38
 
void initdat();//��ʼ������
void alpha(char);//�ַ�������
void digit(char);//���ִ�����
void other(char);//�����ַ�����
void error(char,int);//������(�ַ�)
void error(string,int);//������(��ʾ)
void output(string,int);//���ͼǺ����
void output(string,int,int);//���ͼǺ����
void setcategory(int);//�������������
int findid(string);//���ʶ���ڱ�ʶ�����е�λ��