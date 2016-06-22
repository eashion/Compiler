#include "rule.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>

using namespace std;


int RuleNum=0;														//��¼�������Ŀ
int VtNum=0;														//��¼�ռ�����Ŀ
int VnNum=0;														//��¼���ռ�����Ŀ
char S[256];														//����ջ
int mm[256];														//���VnVt,1=Vn,2=Vt
char vt[MaxVtNum];													//��¼�����ս��
Rule rules[MaxRuleNum];												//����
FirstVT firstVT[MaxVnNum];
LastVT lastVT[MaxVnNum];
int CPG[MaxVtNum][MaxVtNum];										//������ȷ�����,1��ʾ<,2��ʾ=,3��ʾ>,0��ʾ�����ڹ�ϵ


void addRule(char* buffer);											//��ӹ���
void iniFVT();														//����FVT
void iniLVT();														//����LVT
void unionFVT(char A,char a);										//FVT���ϲ�����
void unionFVT(char A,char a[]);
void unionLVT(char A,char a);										//LVT���ϲ�����
void unionLVT(char A,char a[]);
int getVnFVTPos(char Q);											//�ҵ����ռ���FVT/LVTλ��
int getVnLVTPos(char Q);
int getVtPos(char Q);												//�ҵ��ս��λ��
void reIniVt();														//��������vt��
void iniCPG();														//��ʼ��������ȷ�����
int ChaPriAnalysis(char* buffer);									//������ȷ����㷨
int cmp(char a,char b);												//�Ƚ�����Vt���ȼ�
bool checkBuffer(char* buffer);										//��������Ƿ���Ч
int operate(int x,int y);											//��Լ����
void printFVT();													//��ӡFVT
void printLVT();													//��ӡLVT
void printOperation(int cnt,int len,char* buffer,int st,int pos);	//��ӡ����
void printCPG();													//��ӡCPG����


//���庯��
//S->id=E
void function_01();
//E->E1+T
void function_02();
//E->T
void function_03();
//T->T1*F
void function_04();
//T->F
void function_05();
//F->-P
void function_06();
//F->P
void function_07();
//P->(E)
void function_08();
//P->id
void function_09();


int main(){
	char buffer[256];
	ifstream rulesIn("rules.in");
	if( !rulesIn.is_open() ){
		cout<<"Error opening file"<<endl;
		exit(1);
	}
	cout<<"Rules:"<<endl;
	while( !rulesIn.eof() ){
		rulesIn.getline(buffer,200);
		addRule(buffer);
		cout<<buffer<<endl;
	}
	rulesIn.close();
	char firstRule[256] = "S->#S#";
	firstRule[4] = rules[0].leftCharacter;
	cout<<firstRule<<endl;
	addRule(firstRule);
	cout<<"----------------------------------------------------------"<<endl;
	reIniVt();
	iniFVT();
	printFVT();
	iniLVT();
	printLVT();
	iniCPG();
	printCPG();
	ifstream inputString("inputString.in");
	if( !inputString.is_open() ){
		cout<<"Error opening file"<<endl;
		exit(1);
	}
	while( !inputString.eof() ){
		inputString.getline(buffer,200);
	}
	inputString.close();
	if( checkBuffer(buffer) == true ){
		int res = ChaPriAnalysis(buffer);
		if( res == -1 ){
			cout<<"���Ϸ��ַ���!"<<endl;
		}
		else{
			cout<<"�����ɹ���"<<endl;
		}
	}
	else{
		cout<<"��������Ƿ��ַ�!"<<endl;
	}
	system("pause");
	return 0;
}



//��ӹ���
void addRule(char* buffer){
	
	Rule tmp;
	tmp.leftCharacter = buffer[0];
	if( mm[buffer[0]] != 1){
		mm[buffer[0]] = 1;
		firstVT[VnNum].A = buffer[0];
		firstVT[VnNum].len = 0;
		lastVT[VnNum].A = buffer[0];
		lastVT[VnNum].len = 0;
		VnNum++;
	}
	tmp.rightLength = strlen(buffer)-3;
	for( int i = 0; i < tmp.rightLength; i++ ){
		tmp.rightCharacter[i] = buffer[i+3];
		if( mm[buffer[i+3]]!=1 && mm[buffer[i+3]]!=2 ){
			mm[buffer[i+3]] = 2;
			vt[VtNum] = buffer[i+3];
			VtNum++;
		}
	}
	tmp.rightCharacter[tmp.rightLength] = '\0';
	rules[RuleNum] = tmp;
	RuleNum++;

}

//����FVT
void iniFVT(){
	
	stack<char> stk;
	for( int i = 0; i < RuleNum; i++ ){
		Rule tmp = rules[i];
		if( mm[tmp.rightCharacter[0]]==2 ){
			unionFVT(tmp.leftCharacter,tmp.rightCharacter[0]);
			stk.push(tmp.leftCharacter);
		}
		if( mm[tmp.rightCharacter[1]]==2 ){
			unionFVT(tmp.leftCharacter,tmp.rightCharacter[1]);
			stk.push(tmp.leftCharacter);
		}
	}
	while( !stk.empty() ){
		char Q = stk.top();
		stk.pop();
		for( int i = 0; i < RuleNum; i++ ){
			Rule tmp = rules[i];
			if( tmp.rightCharacter[0]==Q && tmp.leftCharacter!=Q ){
				int pos = getVnFVTPos(Q);
				unionFVT(tmp.leftCharacter,firstVT[pos].FVT);
				stk.push(tmp.leftCharacter);
			}
		}
	}
}

//FVT���һ���ַ�
void unionFVT(char A,char a){
	int pos = getVnFVTPos(A);
	int len = firstVT[pos].len;
	firstVT[pos].FVT[len] = a;
	firstVT[pos].len++;
}

//FVT���м��Ϻϲ�
void unionFVT(char A,char a[]){
	char tmp[MaxVtNum];
	int cnt = 0;//��ʱ������ĸ����
	int pos = getVnFVTPos(A);//A FVT�±�
	int len = strlen(a);
	for( int i = 0; i < firstVT[pos].len; i++ ){
		tmp[cnt++] = firstVT[pos].FVT[i];
	}
	for( int i = 0; i < len; i++ ){
		tmp[cnt++] = a[i];
	}
	std::sort(tmp,tmp+cnt);
	len = 0;
	for( int i=0; i < cnt; i++ ){
		if( i==0 || tmp[i]!=tmp[i-1] ){
			firstVT[pos].FVT[len++] = tmp[i];
		}
	}
	firstVT[pos].len = len;
}

//��ȡQ��FVT�±�
int getVnFVTPos(char Q){
	for( int i = 0; i < VnNum; i++ ){
		if( firstVT[i].A == Q ){
			return i;
		}
	}
	return -1;
}

//����LVT
void iniLVT(){
	
	stack<char> stk;
	for( int i = 0; i < RuleNum; i++ ){
		Rule tmp = rules[i];
		int len = tmp.rightLength;
		if( mm[tmp.rightCharacter[len-1]]==2 ){
			unionLVT(tmp.leftCharacter,tmp.rightCharacter[len-1]);
			stk.push(tmp.leftCharacter);
		}
		if( mm[tmp.rightCharacter[len-2]]==2 ){
			unionLVT(tmp.leftCharacter,tmp.rightCharacter[len-2]);
			stk.push(tmp.leftCharacter);
		}
	}
	while( !stk.empty() ){
		char Q = stk.top();
		stk.pop();
		for( int i = 0; i < RuleNum; i++ ){
			Rule tmp = rules[i];
			int len = tmp.rightLength;
			if( tmp.rightCharacter[len-1]==Q && tmp.leftCharacter!=Q ){
				int pos = getVnLVTPos(Q);
				unionLVT(tmp.leftCharacter,lastVT[pos].LVT);
				stk.push(tmp.leftCharacter);
			}
		}
	}



}

//�ҵ����ռ���LVTλ��
int getVnLVTPos(char Q){
	for( int i = 0; i < VnNum; i++ ){
		if( lastVT[i].A == Q ){
			return i;
		}
	}
	return -1;
}

//LVT���һ���ַ�
void unionLVT(char A,char a){
	int pos = getVnLVTPos(A);
	int len = lastVT[pos].len;
	lastVT[pos].LVT[len] = a;
	lastVT[pos].len++;
}

//FVT���м��Ϻϲ�
void unionLVT(char A,char a[]){
	char tmp[MaxVtNum];
	int cnt = 0;//��ʱ������ĸ����
	int pos = getVnLVTPos(A);//A FVT�±�
	int len = strlen(a);
	for( int i = 0; i < lastVT[pos].len; i++ ){
		tmp[cnt++] = lastVT[pos].LVT[i];
	}
	for( int i = 0; i < len; i++ ){
		tmp[cnt++] = a[i];
	}
	sort(tmp,tmp+cnt);
	len = 0;
	for( int i=0; i < cnt; i++ ){
		if( i==0 || tmp[i]!=tmp[i-1] ){
			lastVT[pos].LVT[len++] = tmp[i];
		}
	}
	lastVT[pos].len = len;
}

//��ʼ��������ȷ�����
void iniCPG(){
	for( int i = 0; i < MaxVtNum; i++ ){
		memset(CPG[i],0,sizeof(CPG[i]));
	}
	for( int i = 0; i < RuleNum; i++ ){
		Rule tmp = rules[i];
		int len = tmp.rightLength;
		for( int j = 0; j < len-1; j++ ){
			//A->.....ab.....
			if( mm[tmp.rightCharacter[j]]==2 && mm[tmp.rightCharacter[j+1]]==2 ){
				int p1 = getVtPos(tmp.rightCharacter[j]);
				int p2 = getVtPos(tmp.rightCharacter[j+1]);
				CPG[p1][p2] = 2;
				//CPG[p2][p1] = 2;
			}
			//A->.....aQb.....
			if( j<len-2 && mm[tmp.rightCharacter[j]]==2 && mm[tmp.rightCharacter[j+1]]==1 && mm[tmp.rightCharacter[j+2]]==2 ){
				int p1 = getVtPos(tmp.rightCharacter[j]);
				int p2 = getVtPos(tmp.rightCharacter[j+2]);
				CPG[p1][p2] = 2;
				//CPG[p2][p1] = 2;
			}
			//A->.....aB.....
			if( mm[tmp.rightCharacter[j]]==2 && mm[tmp.rightCharacter[j+1]]==1 ){
				int pos = getVnFVTPos(tmp.rightCharacter[j+1]);
				int p1 = getVtPos(tmp.rightCharacter[j]);
				for( int t = 0; t < firstVT[pos].len; t++ ){
					int p2 = getVtPos(firstVT[pos].FVT[t]);
					CPG[p1][p2] = 1;
					//CPG[p2][p1] = 3;
				}
			}
			//A->.....Ab.....
			if( mm[tmp.rightCharacter[j]]==1 && mm[tmp.rightCharacter[j+1]]==2 ){
				int pos = getVnLVTPos(tmp.rightCharacter[j]);
				int p2 = getVtPos(tmp.rightCharacter[j+1]);
				for( int t = 0; t < lastVT[pos].len; t++ ){
					int p1 = getVtPos(lastVT[pos].LVT[t]);
					CPG[p1][p2] = 3;
					//CPG[p2][p1] = 1;
				}
			}
		}
	}
}

//�ҵ��ս��λ��
int getVtPos(char Q){
	for( int i = 0; i < VtNum; i++ ){
		if( Q == vt[i] ){
			return i;
		}
	}
	return -1;
}

//��������vt��
void reIniVt(){
	int pos = 0;
	char tmp[MaxVtNum] = "\0";
	for( int i = 0; i < VtNum; i++ ){
		if( mm[vt[i]] == 2 ){
			tmp[pos] = vt[i];
			pos++;
		}
	}
	VtNum = pos;
	for( int i = 0; i < VtNum; i++ ){
		vt[i] = tmp[i];
	}
	vt[VtNum] = '\0';
}

//������ȷ����㷨
int ChaPriAnalysis(char* buffer){

	cout<<"�������̣�"<<endl;
	cout<<"����"<<'\t'<<"����ջ"<<'\t'<<"���봮"<<'\t'<<"���"<<endl;

	int len = strlen(buffer);
	buffer[len++] = '#';
	buffer[len] = '\0';
	char Q;
	int res;//�����洢�ȽϽ��
	int top = 1;//����ջ�±�
	int cnt = 1;//��¼��������
	S[top] = '#';

	printOperation(cnt++,top,buffer,0,-1);

	for( int i = 0; i < len; i++ ){
		int j = -1;
		//��S[top]����Vt
		if( mm[S[top]] == 2 ){
			j = top;
		}
		else{
			j = top-1;
		}
		while( cmp(S[j],buffer[i]) == 3 ){
			do{
				Q = S[j];
				if( mm[S[j-1]] == 2 ){
					j = j-1;
				}
				else{
					j = j-2;
				}
				res = cmp(S[j],Q);
			}while( res==2 || res==3 );
			//��Լ����
			int pos = operate(j+1,top);
			//������Ӧ���ҵ������Ҳ���ƥ�䴮
			if( pos == -1 ){
				return -1;
			}
			top = j+1;
			S[top] = rules[pos].leftCharacter;
			printOperation(cnt++,top,buffer,i,pos);
		}
		res = cmp(S[j],buffer[i]);
		if( res==2 || res==1 ){
			top=top+1;
			S[top]=buffer[i];
			printOperation(cnt++,top,buffer,i+1,-1);
		}
		else{
			return -1;//����
		}
	}
	return 1;
}

//�ȽϷ������ȼ�
int cmp(char a,char b){
	int p1 = getVtPos(a);
	int p2 = getVtPos(b);
	return CPG[p1][p2];
}

//������봮�Ƿ�Ϸ�
bool checkBuffer( char* buffer ){
	int len = strlen(buffer);
	for( int i = 0; i < len; i++ ){
		if( mm[buffer[i]]!=1 && mm[buffer[i]]!=2 ){
			return false;
		}
	}
	return true;
}

//��Լ����
int operate(int x,int y){
	int len = y-x+1;
	int st;
	for( int i = 0; i < RuleNum; i++ ){
		st = x;
		if( rules[i].rightLength == len ){
			bool check = true;
			for( int j = 0; j < rules[i].rightLength; j++,st++ ){
				if( mm[S[st]]==1 && mm[rules[i].rightCharacter[j]]==1 ){
					continue;
				}
				if( S[st] != rules[i].rightCharacter[j] ){
					check = false;
					break;
				}
			}
			if( check == true ){
				return i;
			}
		}
	}
	return -1;
}

//��ӡFVT
void printFVT(){
	cout<<"FVT:"<<endl;
	for( int i = 0; i < VnNum; i++ ){
		FirstVT tmp = firstVT[i];
		cout<<tmp.A<<'\t';
		for( int j = 0; j < tmp.len; j++ ){
			cout<<tmp.FVT[j];
			if( j != tmp.len-1 ){
				cout<<" ";
			}
			else{
				cout<<endl;
			}
		}
	}
	cout<<"----------------------------------------------------------"<<endl;
}
//��ӡLVT
void printLVT(){
	cout<<"LVT:"<<endl;
	for( int i = 0; i < VnNum; i++ ){
		LastVT tmp = lastVT[i];
		cout<<tmp.A<<'\t';
		for( int j = 0; j < tmp.len; j++ ){
			cout<<tmp.LVT[j];
			if( j != tmp.len-1 ){
				cout<<" ";
			}
			else{
				cout<<endl;
			}
		}
	}
	cout<<"----------------------------------------------------------"<<endl;
}

//��ӡCPG
void printCPG(){
	cout<<"CPG:"<<endl;
	cout<<' '<<'\t';
	for( int i = 0; i < VtNum; i++ ){
		cout<<vt[i]<<'\t';
	}
	cout<<endl;
	for( int i = 0; i < VtNum; i++ ){
		cout<<vt[i]<<'\t';
		for( int j = 0; j < VtNum; j++ ){
			cout<<CPG[i][j]<<'\t';
		}
		cout<<endl;
	}
	cout<<"----------------------------------------------------------"<<endl;
}

//��ӡ����
void printOperation(int cnt,int len,char* buffer,int st,int pos){

	//��������
	cout<<cnt<<'\t';

	//����ջ
	for( int i = 1; i <= len; i++ ){
		cout<<S[i];
	}
	cout<<'\t';

	//���봮
	int bufferLen = strlen(buffer);
	for( int i = st; i < bufferLen; i++ ){
		cout<<buffer[i];
	}
	cout<<'\t';

	//���
	if( pos != -1 ){
		cout<<rules[pos].leftCharacter<<"->"<<rules[pos].rightCharacter;
	}
	cout<<'\t';

	cout<<endl;
}

