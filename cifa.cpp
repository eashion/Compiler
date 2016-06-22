//**********************************************************
//C语言简易词法分析器
//编程环境VS2005
//程序语言C++
//程序编写：刘小桐
//记号输出说明：
//类型记号：10关键字，20标记符，31数字串字面量，32字符串字面量
//          40分界符，
//          51算术运算符，52关系运算符，53逻辑运算符
//          54位运算符，55赋值运算符，56条件运算符
//          57分量运算符，58下标运算符
//         
//**********************************************************
#include "main.h"
ifstream infile;//全局文件指针
int line=0;//记录总行数和错误行标
int errors=0;//错误行数总和
int category=0;//用于运算符分类，记录类型
int location=0;//用于运算符分类，记录位置
 
void initdat()//初始化各表
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
 
void alpha(char c)//字符串处理
{
    char a[255]={0};
    int i=0;
    while(isalpha(c)||isdigit(c)||c=='_')//字母、数字和下划线
    {
          a[i++]=c;
          infile.get(c);
          if(infile.eof()) break;//遇到文件结束标志,退出循环
    }
   int k=0;
   i=0;
   string b=a;//把a字符数组转换成string型
   for(;i<32;i++)        
     if(key[i]==b) 
     {  
        output(b,10,i+1);//关键字
        k=1;
        break;
     }//end if and end for
   if(k!=1)//不是关键字    
       output(b,20,findid(b));//标识符，findid(b)是返回字符串b在标识符表里位置
  infile.seekg(-1,ios::cur);//全局文件指针后移1个字节
}
 
 
void digit(char c)//数字串处理
{
    char a[255]={0};
    int i=0;    
    //首先判断16进制数(0xAFFF)等
    if(c=='0')//以0开头的字符串不是2、8、10、16进制数，就是非法字符串
    {
      a[i++]=c;
      infile.get(c);
      if(c=='x'||c=='X')//16进制数,后面应该是[0-9a-fA-F]
      {
        char tmepch=infile.get();
        if(isdigit(tmepch)||isalpha(tmepch)||tmepch=='_')//防止取到的是分界符和运算符或非法字符
         {
           infile.seekg(-1,ios::cur);
           while(1)
           {
            a[i++]=c;
            infile.get(c);  
            if(infile.eof()) break;//遇到文件结束标志,退出循环
            else if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))) break;//16进制数,后面应该是[0-9a-fA-F] 
           }
          }
        else infile.seekg(-1,ios::cur);//否则为错误串0x
       }
      else
      {
          while(isdigit(c))//取下一个字符直到非数字字符
          {
            a[i++]=c;
            infile.get(c);  
            if(infile.eof()) break;//遇到文件结束标志,退出循环
          }
         //开始判断字符串是否是正确的8进制数
         for(int j=1;j<i;j++)
             if(a[j]>='8')//串中含有8和9，则为错误的8进制数
             {
                 a[i]='\0';
                 error(a,line+1);//报错
                 goto enddigit;//退出该函数
             }  
          //判断结束
      }
    }
    while(isdigit(c))//取下一个字符直到非数字字符
    {
         
        a[i++]=c;
        infile.get(c);  
        if(infile.eof()) break;//遇到文件结束标志,退出循环
    }    
    if(c=='.')//考虑小数
    {
      a[i++]=c;//把小数点放到数组中，并使下标加1
      infile.get(c);
      char tm=c;
      while(isdigit(c))//判断小数点后是否是数字
      {
        a[i++]=c;
        infile.get(c);
        if(infile.eof()) break;//遇到文件结束标志,退出循环
      }
      if((c=='e'||c=='E')&&a[i-1]!='.') //指数形式
      {
eq:       char tempc=infile.get();
          if(isdigit(tempc))//检查下一个字符是否是数字
          {                 //若是
eq1:          a[i++]=c;     //则把原来没有放入数组a中的字符c重新放入
              a[i++]=tempc; //放入字符tempc
              infile.get(c);
              while(isdigit(c))
              {
                a[i++]=c;
                infile.get(c);
                if(infile.eof()) break;//遇到文件结束标志,退出循环
              }
          }
          else if(tempc=='+'||tempc=='-')//1.2e-1形式的数字串处理
          {   
              char nextch=infile.get();
              if(isdigit(nextch)) //检查下一个字符是否是数字
              { 
                  infile.seekg(-1,ios::cur);//文件指针后移1个字节，
                  goto eq1;//来防止跳到eq1行后nextch字符被去掉(因为eq1行后有语句：infile.get(c);)
              }
              else infile.seekg(-1,ios::cur);//文件指针后移1个字节
          }
          else if(isalpha(c)) goto eq2;//数字后接字母和下划线的错误处理
          else infile.seekg(-1,ios::cur);//文件指针后移1个字节
      }
      //否则做点号处理
      else
      {
       for(i=0;i<OP;i++)
           if(operators[i]==a)
            {
                setcategory(i);
                output(a,category,location);
                goto enddigit;//不再输出数字串a(绕过语句output(a,31);)
                //break;
            }       
      }
    }
    else if(c=='e'||c=='E') goto eq;//指数形式
    else if(isalpha(c))//数字后接字母和下划线的错误处理 
    {
eq2:    char b[255]={0};
        strcat_s(b,a);//复制字符数组a中所有字符到b字符数组中
        if(c=='f'||c=='l'||c=='F'||c=='L')//判断是否为合法的字符串:digit+'f'/digit+'l'以及大写形式
        {   //再判断下一个字符是否为非字符和非数字和非下划线
            //如果不是则输出合法串，并结束；例如  5f(%5.1f)等
            char nextc=infile.get();
            if(isalpha(nextc)||isdigit(nextc)||nextc=='_')//非法标识符
            {
                infile.seekg(-1,ios::cur);//此串为非法标识符，指针返回判断前
                goto er;//并跳到er行输出错误串
            }
            else a[i++]=c;//合法数字串,加入到数组中，并输出           
        }
        else
        {
          while(isalpha(c)||isdigit(c)||c=='_')//取出该错误串，以便报错输出
          {
er:       b[i++]=c;
          infile.get(c);
          if(infile.eof()) break;//遇到文件结束标志,退出循环
          }
            b[i]='\0';
            error(b,line+1);//输出错误串
           goto enddigit;//不再输出数字串a(绕过语句output(a,31);)
        }
    }
  output(a,31);//输出数字串a
enddigit:
  infile.seekg(-1,ios::cur);//全局文件指针后移1个字节
}
 
 
void other(char c)//其它字符处理
{
    string a="1";
    a[0]=c;
    int wh=infile.tellg();//记录下文件指针当前位置
    if(c==' '||c=='\t');//去掉空格和水平制表符
    else if(c=='.') digit(c);//以点开头的数字串，如果不是，在数字串处理函数中做相应处理
    else if(c=='\n'||c=='\r') line++;//行数加1，并去掉回车
    else if(c=='/')//判断是否是注释,若是则开始处理两种注释
    { 
        infile.get(c);
        if(c=='/')//字符串为"//"的注释
        {
            while(c!='\n'&&c!='\r'&&!infile.eof()) infile.get(c);//去掉该行注释       
            line++;//总行数加1
        }
        //end if
        else if(c=='*')//字符串为"/*"的注释
        {              //开始寻找最近一个"*/"与之配对，并去掉中间所有字符
            int tmpline=line;//记录当前行数，以便出错时报出
            string str="11";//如果不对其赋初值，那么在后面操作中不能对其中各值作更改
            str[0]=infile.get();//取文件的下两个字符来与字符串"*/"比较
            str[1]=infile.get();
            while(str!="*/")
            {                //如果字符串str中不是"*/"
               str[0]=str[1];//开始做字符前移操作，即去掉第一个字符，补充为str串中的后一个字符
               str[1]=infile.get();//再从文件中取一个字符来补充str串的最后一个字符，使之形成新串与"*/"比较
               if(str[0]=='\n'||str[0]=='\r') line++;//遇到回车，总行数加1，比较一个字符即可
               if(infile.eof()) //文件结束，但未找到"*/"与之配对，则报错
               {
                   error("Can not find matching string:\"*/\" !",tmpline+1);//报错
                   break;//退出循环
               }
            }
        }//end else if
     else
        {
            infile.seekg(wh);//不是注释，文件指针返回到判断前的位置
                             //并开始处理字符'/'
            for(int i=0;i<OP;i++)
                  if(operators[i]==a)//与operators表中的元素比较
                  {
                    setcategory(i);//设置该运算符的类型和位置
                    output(a,category,location);//并输出
                    break;
                  }//end if and end for
        }
    }//end if
    else if(c=='\"'||c=='\'')//以",'开头的字符串字面量
        {
            string tmp;
            char tmpc=c;
            int tmpline=line;//记录当前行数，以便出错报出
            int i=0,k=0;//k为标识是否找到，1为找到，0为找不到
            while(1)//开始寻找配对的",'号
            {
               tmp+=c;
               c=infile.get();
               if(c==tmpc&&tmp[tmp.size()-1]!='\\')//判断是否满足条件：前面一字符不能为转义运算符"\"
               {
                   tmp+=c;//把找到的配对符放到数组中
                   k=1;//找到
                   break;
               }
               else if(c=='\n'||c=='\r') line++;//遇到回车，总行数加1
               else if(infile.eof())
               {
                   error("Can not find matching string:\"\"\" or \"'\" !",tmpline+1);
                   k=0;//找不到
                   break;
               }
            }
            if(k) output(tmp,32);
        }
   else
   {
       int k=0;
       string str1="11",str2="111";//字符串类型变量，str1用来比较有2个字符的运算符，str2用来比较有3个字符的运算符
                                    
       str1[0]=str2[0]=a[0];
       str1[1]=str2[1]=infile.get();
       str2[2]=infile.get();
       //优先考虑3个字符的运算符
       for(int i=0;i<OP;i++)
       { 
           if(operators[i]==str2)//与operators表中的元素比较
           {
               setcategory(i);//设置该运算符的类型和位置
               output(str2,category,location);//并输出
               k=1;//把k置1,标记已找到
               break;
           }
         
       }
       if(k!=1)
       {  //开始考虑2个字符的运算符
          infile.seekg(-1,ios::cur);//文件指针后移1个字节,
                                    //因为在考虑3个字符的运算符时文件指针已后移一个字节                
          for(int i=0;i<OP;i++)
          {  
           if(operators[i]==str1)
           {
               setcategory(i);
               output(str1,category,location);
               k=1;//把k置1,标记已找到
               break;
           }        
          } 
          if(k!=1)
          { //开始考虑1个字符的运算符和分界符
           infile.seekg(wh);//文件指针返回到判断前的位置
            for(int i=0;i<BO;i++)
               if(boundary[i]==a)
               {
                   output(a,40,i+1);//分界符，输出
                   k=1;//把k置1,标记已找到
                   break;
               }
           if(k!=1)
              for(int i=0;i<OP;i++)
                  if(operators[i]==a)
                  {
                    setcategory(i);
                    output(a,category,location);
                    k=1;//把k置1,标记已找到
                    break;
                  }//end if
           if(k!=1)  error(c,line+1);//如果k值还是0(未找到)，则该字符为非法字符，报错
          }    
       }//end if
   }//end if
}
 
 
void setcategory(int i)//运算符类型设置
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
 
int findid(string s)//求标识符在标识符表中的位置
{
 int w=0;//记录位置
 int y=0;//是否找到，0未找到，1已找到
 char str[255]={0};
 ifstream idin("id.dat",ios::in);
 if(!idin) exit(-1);
 while(1)
 {
     idin.getline(str,255,'\n');//取一行字符给字符数组str，(255个，直到遇到回车)
     w++;
     if(idin.eof()) break;//文件结束，未找到，退出循环
     else if(str==s) //找到
     {
         y=1;//找到标识符置1
         break;//退出循环
     }   
 }
 idin.close();
 if(y!=1)//未找到，则写入文件
 {
     ofstream idout("id.dat",ios::app);
     if(!idout) exit(-1);
     idout<<s<<endl;
     idout.close();
 }
 return w;//返回位置值
}
 
void output(string s,int cate)//类型记号输出
{
  ofstream outfile("output.dat",ios::app);
  if(!outfile) exit(-1);
  outfile<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<s<<">"<<endl;
  //cout<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<s<<">"<<endl;
  outfile.close();
}
 
void output(string s,int cate,int loc)//类型记号输出
{
  ofstream outfile("output.dat",ios::app);
  if(!outfile) exit(-1);
  outfile<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<loc<<">"<<endl;
  //cout<<s<<'\t'<<'\t'<<'\t'<<"<"<<cate<<","<<loc<<">"<<endl;
  outfile.close();
}
 
void error(char c,int l)//报错
{  
   cout<<"Error! No."<<l<<" line:"<<c<<endl;//输出错误所在行数及错误字符
   errors++;//错误总数加1
}
 
void error(string s,int l)//报错
{
   cout<<"Error! No."<<l<<" line:"<<s<<endl;//输出错误所在行数及错误字符串
    
   errors++;//错误总数加1
}
 
 
void main()
{
  initdat();
  char filename[80]={"ll.txt"};
  cout<<"请输入文件名："<<endl;
  cin>>filename;
  char ch;
  infile.open(filename,ios::in);//以输入形式打开文件
  if(!infile)
  {
      cerr<<"Open the file error!"<<endl;
      exit(-1);
  }
  while(infile.get(ch))//取字符给ch
  {
       if(isalpha(ch)||ch=='_')
          alpha(ch);
       else if(isdigit(ch))
          digit(ch);
       else other(ch);     
   }
  if(errors)//报错  
    cout<<"A total of "<<errors<<" errors!"<<endl;
  else
    cout<<"没有发现错误！"<<endl;
  system("pause");
	infile.close();//关闭文件
}