//**********************************************************
//C���Լ��״ʷ�������
//��̻���VS2005
//��������C++
//�����д����Сͩ
//�Ǻ����˵����
//���ͼǺţ�10�ؼ��֣�20��Ƿ���31���ִ���������32�ַ���������
//          40�ֽ����
//          51�����������52��ϵ�������53�߼������
//          54λ�������55��ֵ�������56���������
//          57�����������58�±������
//         
//**********************************************************
#include "main.h"
ifstream infile;//ȫ���ļ�ָ��
int line=0;//��¼�������ʹ����б�
int errors=0;//���������ܺ�
int category=0;//������������࣬��¼����
int location=0;//������������࣬��¼λ��
 
void initdat()//��ʼ������
{
   char buffer[256];
   ifstream in("key.dat",ios::in);
   if( !in ){
	exit(-1);
   }
   int i = 0;
   while( !in.eof() ){
	in.getline(buffer,100);
	key[i++] = buffer;
   }
   in.close();
   in.open("boundary.dat",ios::in);
   if( !in ){
	exit(-1);
   }
   i = 0;
   while( !in.eof() ){
	   in.getline(buffer,100);
	   boundary[i++] = buffer;
   }
   in.close();
   in.open("operators.dat",ios::in);
   if( !in ){
	exit(-1);
   }
   i = 0;
   while( !in.eof() ){
	   in.getline(buffer,100);
	   operators[i++] = buffer;
   }
   in.close();

   ofstream out("output.dat",ios::out);
   if( !out ){
	exit(-1);
   }
   out.close();
}
 
void alpha(char c)//�ַ�������
{
    char a[255]={0};
    int i=0;
    while(isalpha(c)||isdigit(c)||c=='_')//��ĸ�����ֺ��»���
    {
          a[i++]=c;
          infile.get(c);
          if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
    }
   int k=0;
   i=0;
   string b=a;//��a�ַ�����ת����string��
   for(;i<32;i++)        
     if(key[i]==b) 
     {  
        output(b,10,i+1);//�ؼ���
        k=1;
        break;
     }//end if and end for
   if(k!=1)//���ǹؼ���    
       output(b,20,findid(b));//��ʶ����findid(b)�Ƿ����ַ���b�ڱ�ʶ������λ��
  infile.seekg(-1,ios::cur);//ȫ���ļ�ָ�����1���ֽ�
}
 
 
void digit(char c)//���ִ�����
{
    char a[255]={0};
    int i=0;    
    //�����ж�16������(0xAFFF)��
    if(c=='0')//��0��ͷ���ַ�������2��8��10��16�����������ǷǷ��ַ���
    {
      a[i++]=c;
      infile.get(c);
      if(c=='x'||c=='X')//16������,����Ӧ����[0-9a-fA-F]
      {
        char tmepch=infile.get();
        if(isdigit(tmepch)||isalpha(tmepch)||tmepch=='_')//��ֹȡ�����Ƿֽ�����������Ƿ��ַ�
         {
           infile.seekg(-1,ios::cur);
           while(1)
           {
            a[i++]=c;
            infile.get(c);  
            if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
            else if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))) break;//16������,����Ӧ����[0-9a-fA-F] 
           }
          }
        else infile.seekg(-1,ios::cur);//����Ϊ����0x
       }
      else
      {
          while(isdigit(c))//ȡ��һ���ַ�ֱ���������ַ�
          {
            a[i++]=c;
            infile.get(c);  
            if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
          }
         //��ʼ�ж��ַ����Ƿ�����ȷ��8������
         for(int j=1;j<i;j++)
             if(a[j]>='8')//���к���8��9����Ϊ�����8������
             {
                 a[i]='\0';
                 error(a,line+1);//����
                 goto enddigit;//�˳��ú���
             }  
          //�жϽ���
      }
    }
    while(isdigit(c))//ȡ��һ���ַ�ֱ���������ַ�
    {
         
        a[i++]=c;
        infile.get(c);  
        if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
    }    
    if(c=='.')//����С��
    {
      a[i++]=c;//��С����ŵ������У���ʹ�±��1
      infile.get(c);
      char tm=c;
      while(isdigit(c))//�ж�С������Ƿ�������
      {
        a[i++]=c;
        infile.get(c);
        if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
      }
      if((c=='e'||c=='E')&&a[i-1]!='.') //ָ����ʽ
      {
eq:       char tempc=infile.get();
          if(isdigit(tempc))//�����һ���ַ��Ƿ�������
          {                 //����
eq1:          a[i++]=c;     //���ԭ��û�з�������a�е��ַ�c���·���
              a[i++]=tempc; //�����ַ�tempc
              infile.get(c);
              while(isdigit(c))
              {
                a[i++]=c;
                infile.get(c);
                if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
              }
          }
          else if(tempc=='+'||tempc=='-')//1.2e-1��ʽ�����ִ�����
          {   
              char nextch=infile.get();
              if(isdigit(nextch)) //�����һ���ַ��Ƿ�������
              { 
                  infile.seekg(-1,ios::cur);//�ļ�ָ�����1���ֽڣ�
                  goto eq1;//����ֹ����eq1�к�nextch�ַ���ȥ��(��Ϊeq1�к�����䣺infile.get(c);)
              }
              else infile.seekg(-1,ios::cur);//�ļ�ָ�����1���ֽ�
          }
          else if(isalpha(c)) goto eq2;//���ֺ����ĸ���»��ߵĴ�����
          else infile.seekg(-1,ios::cur);//�ļ�ָ�����1���ֽ�
      }
      //��������Ŵ���
      else
      {
       for(i=0;i<OP;i++)
           if(operators[i]==a)
            {
                setcategory(i);
                output(a,category,location);
                goto enddigit;//����������ִ�a(�ƹ����output(a,31);)
                //break;
            }       
      }
    }
    else if(c=='e'||c=='E') goto eq;//ָ����ʽ
    else if(isalpha(c))//���ֺ����ĸ���»��ߵĴ����� 
    {
eq2:    char b[255]={0};
        strcat_s(b,a);//�����ַ�����a�������ַ���b�ַ�������
        if(c=='f'||c=='l'||c=='F'||c=='L')//�ж��Ƿ�Ϊ�Ϸ����ַ���:digit+'f'/digit+'l'�Լ���д��ʽ
        {   //���ж���һ���ַ��Ƿ�Ϊ���ַ��ͷ����ֺͷ��»���
            //�������������Ϸ�����������������  5f(%5.1f)��
            char nextc=infile.get();
            if(isalpha(nextc)||isdigit(nextc)||nextc=='_')//�Ƿ���ʶ��
            {
                infile.seekg(-1,ios::cur);//�˴�Ϊ�Ƿ���ʶ����ָ�뷵���ж�ǰ
                goto er;//������er���������
            }
            else a[i++]=c;//�Ϸ����ִ�,���뵽�����У������           
        }
        else
        {
          while(isalpha(c)||isdigit(c)||c=='_')//ȡ���ô��󴮣��Ա㱨�����
          {
er:       b[i++]=c;
          infile.get(c);
          if(infile.eof()) break;//�����ļ�������־,�˳�ѭ��
          }
            b[i]='\0';
            error(b,line+1);//�������
           goto enddigit;//����������ִ�a(�ƹ����output(a,31);)
        }
    }
  output(a,31);//������ִ�a
enddigit:
  infile.seekg(-1,ios::cur);//ȫ���ļ�ָ�����1���ֽ�
}
 
 
void other(char c)//�����ַ�����
{
    string a="1";
    a[0]=c;
    int wh=infile.tellg();//��¼���ļ�ָ�뵱ǰλ��
    if(c==' '||c=='\t');//ȥ���ո��ˮƽ�Ʊ��
    else if(c=='.') digit(c);//�Ե㿪ͷ�����ִ���������ǣ������ִ�������������Ӧ����
    else if(c=='\n'||c=='\r') line++;//������1����ȥ���س�
    else if(c=='/')//�ж��Ƿ���ע��,������ʼ��������ע��
    { 
        infile.get(c);
        if(c=='/')//�ַ���Ϊ"//"��ע��
        {
            while(c!='\n'&&c!='\r'&&!infile.eof()) infile.get(c);//ȥ������ע��       
            line++;//��������1
        }
        //end if
        else if(c=='*')//�ַ���Ϊ"/*"��ע��
        {              //��ʼѰ�����һ��"*/"��֮��ԣ���ȥ���м������ַ�
            int tmpline=line;//��¼��ǰ�������Ա����ʱ����
            string str="11";//��������丳��ֵ����ô�ں�������в��ܶ����и�ֵ������
            str[0]=infile.get();//ȡ�ļ����������ַ������ַ���"*/"�Ƚ�
            str[1]=infile.get();
            while(str!="*/")
            {                //����ַ���str�в���"*/"
               str[0]=str[1];//��ʼ���ַ�ǰ�Ʋ�������ȥ����һ���ַ�������Ϊstr���еĺ�һ���ַ�
               str[1]=infile.get();//�ٴ��ļ���ȡһ���ַ�������str�������һ���ַ���ʹ֮�γ��´���"*/"�Ƚ�
               if(str[0]=='\n'||str[0]=='\r') line++;//�����س�����������1���Ƚ�һ���ַ�����
               if(infile.eof()) //�ļ���������δ�ҵ�"*/"��֮��ԣ��򱨴�
               {
                   error("Can not find matching string:\"*/\" !",tmpline+1);//����
                   break;//�˳�ѭ��
               }
            }
        }//end else if
     else
        {
            infile.seekg(wh);//����ע�ͣ��ļ�ָ�뷵�ص��ж�ǰ��λ��
                             //����ʼ�����ַ�'/'
            for(int i=0;i<OP;i++)
                  if(operators[i]==a)//��operators���е�Ԫ�رȽ�
                  {
                    setcategory(i);//���ø�����������ͺ�λ��
                    output(a,category,location);//�����
                    break;
                  }//end if and end for
        }
    }//end if
    else if(c=='\"'||c=='\'')//��",'��ͷ���ַ���������
        {
            string tmp;
            char tmpc=c;
            int tmpline=line;//��¼��ǰ�������Ա������
            int i=0,k=0;//kΪ��ʶ�Ƿ��ҵ���1Ϊ�ҵ���0Ϊ�Ҳ���
            while(1)//��ʼѰ����Ե�",'��
            {
               tmp+=c;
               c=infile.get();
               if(c==tmpc&&tmp[tmp.size()-1]!='\\')//�ж��Ƿ�����������ǰ��һ�ַ�����Ϊת�������"\"
               {
                   tmp+=c;//���ҵ�����Է��ŵ�������
                   k=1;//�ҵ�
                   break;
               }
               else if(c=='\n'||c=='\r') line++;//�����س�����������1
               else if(infile.eof())
               {
                   error("Can not find matching string:\"\"\" or \"'\" !",tmpline+1);
                   k=0;//�Ҳ���
                   break;
               }
            }
            if(k) output(tmp,32);
        }
   else
   {
       int k=0;
       string str1="11",str2="111";//�ַ������ͱ�����str1�����Ƚ���2���ַ����������str2�����Ƚ���3���ַ��������
                                    
       str1[0]=str2[0]=a[0];
       str1[1]=str2[1]=infile.get();
       str2[2]=infile.get();
       //���ȿ���3���ַ��������
       for(int i=0;i<OP;i++)
       { 
           if(operators[i]==str2)//��operators���е�Ԫ�رȽ�
           {
               setcategory(i);//���ø�����������ͺ�λ��
               output(str2,category,location);//�����
               k=1;//��k��1,������ҵ�
               break;
           }
         
       }
       if(k!=1)
       {  //��ʼ����2���ַ��������
          infile.seekg(-1,ios::cur);//�ļ�ָ�����1���ֽ�,
                                    //��Ϊ�ڿ���3���ַ��������ʱ�ļ�ָ���Ѻ���һ���ֽ�                
          for(int i=0;i<OP;i++)
          {  
           if(operators[i]==str1)
           {
               setcategory(i);
               output(str1,category,location);
               k=1;//��k��1,������ҵ�
               break;
           }        
          } 
          if(k!=1)
          { //��ʼ����1���ַ���������ͷֽ��
           infile.seekg(wh);//�ļ�ָ�뷵�ص��ж�ǰ��λ��
            for(int i=0;i<BO;i++)
               if(boundary[i]==a)
               {
                   output(a,40,i+1);//�ֽ�������
                   k=1;//��k��1,������ҵ�
                   break;
               }
           if(k!=1)
              for(int i=0;i<OP;i++)
                  if(operators[i]==a)
                  {
                    setcategory(i);
                    output(a,category,location);
                    k=1;//��k��1,������ҵ�
                    break;
                  }//end if
           if(k!=1)  error(c,line+1);//���kֵ����0(δ�ҵ�)������ַ�Ϊ�Ƿ��ַ�������
          }    
       }//end if
   }//end if
}
 
 
void setcategory(int i)//�������������
{
    if(i<7) {category=51;location=i+1;}
    else if (i<13){ category=52;location=i-OP1;}
    else if (i<16){ category=53;location=i-OP2;}
    else if (i<22){ category=54;location=i-OP3;}
    else if (i<33){ category=55;location=i-OP4;}
    else if (i<35){ category=56;location=i-OP5;}
    else if (i<37){ category=57;location=i-OP6;}
    else if (i<39){ category=58;location=i-OP7;}
    else {category=59;location=i-OP8;}
}
 
int findid(string s)//���ʶ���ڱ�ʶ�����е�λ��
{
 int w=0;//��¼λ��
 int y=0;//�Ƿ��ҵ���0δ�ҵ���1���ҵ�
 char str[255]={0};
 ifstream idin("id.dat",ios::in);
 if(!idin) exit(-1);
 while(1)
 {
     idin.getline(str,255,'\n');//ȡһ���ַ����ַ�����str��(255����ֱ�������س�)
     w++;
     if(idin.eof()) break;//�ļ�������δ�ҵ����˳�ѭ��
     else if(str==s) //�ҵ�
     {
         y=1;//�ҵ���ʶ����1
         break;//�˳�ѭ��
     }   
 }
 idin.close();
 if(y!=1)//δ�ҵ�����д���ļ�
 {
     ofstream idout("id.dat",ios::app);
     if(!idout) exit(-1);
     idout<<s<<endl;
     idout.close();
 }
 return w;//����λ��ֵ
}
 
void output(string s,int cate)//���ͼǺ����
{
  ofstream outfile("output.dat",ios::app);
  if(!outfile) exit(-1);
  outfile<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<s<<">"<<endl;
  //cout<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<s<<">"<<endl;
  outfile.close();
}
 
void output(string s,int cate,int loc)//���ͼǺ����
{
  ofstream outfile("output.dat",ios::app);
  if(!outfile) exit(-1);
  outfile<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<loc<<">"<<endl;
  //cout<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<loc<<">"<<endl;
  outfile.close();
}
 
void error(char c,int l)//����
{  
   cout<<"Error! No."<<l<<" line:"<<c<<endl;//����������������������ַ�
   errors++;//����������1
}
 
void error(string s,int l)//����
{
   cout<<"Error! No."<<l<<" line:"<<s<<endl;//����������������������ַ���
    
   errors++;//����������1
}
 
 
void main()
{
  initdat();
  char filename[80]={"ll.txt"};
  cout<<"�������ļ�����"<<endl;
  cin>>filename;
  char ch;
  infile.open(filename,ios::in);//��������ʽ���ļ�
  if(!infile)
  {
      cerr<<"Open the file error!"<<endl;
      exit(-1);
  }
  while(infile.get(ch))//ȡ�ַ���ch
  {
       if(isalpha(ch)||ch=='_')
          alpha(ch);
       else if(isdigit(ch))
          digit(ch);
       else other(ch);     
   }
  if(errors)//����  
    cout<<"A total of "<<errors<<" errors!"<<endl;
  else
    cout<<"û�з��ִ���"<<endl;
  system("pause");
	infile.close();//�ر��ļ�
}