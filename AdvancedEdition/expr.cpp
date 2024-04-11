#include<iostream>
#include<iomanip>
#include<cmath>
#define debug
using namespace std;
template<typename T>class stack{//自定义栈，一次性申请一段连续的内存空间，当栈溢出时，再申请一个新的节点 
private:
	static const int BLOCKSIZE=1024;
	template<typename S>struct stackNode{
		S data[BLOCKSIZE];//以顺序表为节点的链表 
		stackNode<S>* next;
		stackNode<S>* prev;
	};
	stackNode<T>* node;
	T* top;
public:
	stack(){//构造函数 
		node=new stackNode<T>;
		node->next=NULL;
		node->prev=NULL;
		top=(node->data)-1;
	}
	~stack(){//析构函数 
		stackNode<T>* ptr;
		while(node!=NULL){
			ptr=node;
			node=node->prev;
			delete ptr;
		}
	}
	void push(const T& x){//入栈函数 
		if(top-(node->data)==BLOCKSIZE-1){//本张顺序表满的情况下，开辟一个新的顺序表节点 
			node->next=new stackNode<T>;
			node->next->prev=node;
			node->next->next=NULL;
			node=node->next;
			top=node->data;
		}
		else{//否则操作同普通栈 
			++top;
		}
		*top=x;
	}
	T pop(){//将栈顶元素弹出 
		T x=*top;
		if(top==(node->data)&&node->prev!=NULL){//恰好尾节点为空时，删除节点 
			node=node->prev;
			delete node->next;
			node->next=NULL;
			top=(node->data)+BLOCKSIZE-1;
		}
		else{//否则操作同普通栈 
			--top;
		}
		return x;
	}
	T* Top(){//返回栈顶元素 
		return top;
	}
	bool isEmpty(){//判断栈是否为空 
		if(top==(node->data)-1&&node->prev==NULL)return true;
		return false;
	}
};

template<typename T>class queue{//自定义队列，也是用顺序表作为节点构成的链表实现的 
private:
	static const int BLOCKSIZE=1024;
	template<typename S>struct queueNode{//顺序表节点的定义 
		S data[BLOCKSIZE];
		queueNode<S>* prev;
		queueNode<S>* next;
	};
	queueNode<T>* frontNode;
	queueNode<T>* tailNode;
	T* frontPtr;
	T* tailPtr;
public:
	queue(){//构造函数 
		tailNode=new queueNode<T>;
		frontNode=tailNode;
		tailNode->prev=NULL;
		tailNode->next=NULL;
		frontPtr=(frontNode->data)-1;
		tailPtr=(tailNode->data)-1;
	}
	~queue(){//析构函数 
		queueNode<T>* ptr;
		while(tailNode!=NULL){
			ptr=tailNode;
			tailNode=tailNode->prev;
			delete ptr;
		}
	}
	void push(const T& x){//入队函数 
		if(tailPtr-(tailNode->data)==BLOCKSIZE-1){//需申请内存空间的情况 
			tailNode->next=new queueNode<T>;
			tailNode->next->prev=tailNode;
			tailNode->next->next=NULL;
			tailNode=tailNode->next;
			tailPtr=tailNode->data;
		}
		else{
			if(tailPtr==(frontNode->data)-1){
				++frontPtr;
			}
			++tailPtr;
		}
		*tailPtr=x;
	}
	T pop(){//出队函数 
		if(frontPtr==(tailNode->data)-1)return NULL;//空队列 
		T x=*frontPtr;
		if(frontPtr==tailPtr){//队列只有一个元素 
			frontPtr=(frontNode->data)-1;
			tailPtr=(tailNode->data)-1;
		}
		else if(frontPtr-(frontNode->data)==BLOCKSIZE-1){//需取消节点的情况 
			frontNode=frontNode->next;
			delete frontNode->prev;
			frontNode->prev=NULL;
			frontPtr=frontNode->data;
		}
		else{
			++frontPtr;
		}
		return x;
	}
	bool isEmpty(){//判断队列是否为空 
		if(tailNode->prev==NULL&&tailPtr==(tailNode->data)-1)return true;
		return false;
	}
};

//为提供函数指针，将实现计算器功能用到的一些函数定义如下 
double Add(double a,double b){
	return a+b;
}
double Minus(double a,double b){
	return a-b;
}
double Multiply(double a,double b){
	return a*b;
}
double Divide(double a,double b){
	return a*1.0/b;
}
int Mod(int a,int b){//取模 
	return a%b;
}
int Factorial(int a){//阶乘 
	if(a<=0){
		return 1;
	}
	else{
		return a*Factorial(a-1);
	}
}
double Sum(double a[],int n){//求和 
	double sum=0;
	for(int i=0;i<n;i++){
		sum+=a[i];
	}
	return sum;
}
double Average(double a[],int n){//求平均 
	double sum=0;
	for(int i=0;i<n;i++){
		sum+=a[i];
	}
	return sum*1.0/n;
}
double Var(double a[],int n){//求样本方差 
	double sum=0;
	double sqSum=0;
	for(int i=0;i<n;i++){
		sum+=a[i];
		sqSum+=a[i]*a[i];
	}
	return sqSum/(n-1)-sum*sum/(n*(n-1));
}
double Sd(double a[],int n){//求样本标准差 
	double sum=0;
	double sqSum=0;
	for(int i=0;i<n;i++){
		sum+=a[i];
		sqSum+=a[i]*a[i];
	}
	return sqrt(sqSum/(n-1)-sum*sum/(n*(n-1)));
}
double Fail(double a,double b){//自定义的Fail函数，计算失败时返回含有Fail函数指针的term对象 
	return -1;
}

//以下是文本分析时用到的字符分类分析函数 
bool isNumber(char ch){//是否是数字 
	if(ch>='0'&&ch<='9')return true;
	return false;
}
bool isAlphabet(char ch){//是否是字母 
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))return true;
	return false;
}
bool isOperator(char ch){//是否是运算符 
	if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^'||ch=='!'||ch=='%')return true;
	return false;
}


const int precision=10;//定义计算器的输出精度 
bool equal(double a,double b){//定义浮点数与整形数相等的阈值 
	if(abs(a-b)<=0.000000000001)return true;
	return false;
}

char* substr(char* ch,int begin,int end){//用于构造子字符串 
	char* subCh;
	if(end-begin-1>0){//左括号与右括号中至少有一个字符 
		subCh=new char[end-begin];
		for(int i=0;i<end-begin-1;i++){
			subCh[i]=ch[begin+i+1];
		}
		subCh[end-begin-1]='\0';
	}
	else{//空括号 
		subCh=new char[1];
		subCh[0]='\0';
	}
	return subCh;
}
const int nameLength=10;//定义函数名称的长度限制 

void tagCopy(char* dest,char* src,int n=nameLength){//字符串复制，用于function类对象的构造，将函数名字符串复制给function类的成员tag 
	for(int i=0;i<n-1;i++){
		dest[i]=src[i];
	}
	dest[n-1]='\0';
}

//计算器类 
class calculator{
private:
	struct errorInfo{//错误信息结构体，表明错误的定位与编码 
		int position;
		int errCode;
	};
	struct term{//操作符与操作数的结构体 
		union{
			int ival;//整形变量 
			double dval;//浮点型变量 
			double(*ptr1)(double,double);//各种类型的函数指针 
			double(*ptr2)(double);
			int(*ptr3)(int,int);
			int(*ptr4)(int);
			double(*ptr5)(double*,int);//参数个数不定的函数 
		}val;
		enum{//表明term对象的类别 
			Int,Float,biDouble,uniDouble,biInt,uniInt,Array
		}attribute;
		enum{//表明操作符应该与哪边的操作数进行结合，对于操作数，属性为None 
			None,L,R,LR,RR
		}category;
		int position;//操作符或操作数在字符串中的位置 
		void print(){//打印term对象的信息 
			switch(attribute){
				case Int:
					cout<<val.ival;
					break;
				case Float:
					cout<<setprecision(precision)<<val.dval;
					break;
				case biDouble:
					if(val.ptr1==Add){
						cout<<'+';
					}
					else if(val.ptr1==Minus){
						cout<<'-';
					}
					else if(val.ptr1==Multiply){
						cout<<'*';
					}
					else if(val.ptr1==Divide){
						cout<<'/';
					}
					else if(val.ptr1==pow){
						cout<<"pow";
					}
					else if(val.ptr1==Fail){
						cout<<"Error";
					}
					break;
				case uniDouble:
					if(val.ptr2==fabs){
						cout<<"abs";
					}
					else if(val.ptr2==acos){
						cout<<"acos";
					}
					else if(val.ptr2==asin){
						cout<<"asin";
					}
					else if(val.ptr2==atan){
						cout<<"atan";
					}
					else if(val.ptr2==cos){
						cout<<"cos";
					}
					else if(val.ptr2==exp){
						cout<<"exp";
					}
					else if(val.ptr2==log){
						cout<<"ln";
					}
					else if(val.ptr2==log10){
						cout<<"log";
					}
					else if(val.ptr2==sqrt){
						cout<<"sqrt";
					}
					else if(val.ptr2==sin){
						cout<<"sin";
					}
					else if(val.ptr2==tan){
						cout<<"tan";
					}
					break;
				case biInt:
					if(val.ptr3==Mod){
						cout<<'%';
					}
					break;
				case uniInt:
					if(val.ptr4==Factorial){
						cout<<'!';
					}
					break;
				case Array:
					if(val.ptr5==Sum){
						cout<<"sum";
					}
					else if(val.ptr5==Average){
						cout<<"avg";
					}
					else if(val.ptr5==Var){
						cout<<"var";
					}
					else if(val.ptr5==Sd){
						cout<<"SD";
					}
				default:
					cout<<"Error";
			}
		}
	};

	struct termNode{//termList双向链表的节点 
		term data;
		termNode* prev;
		termNode* next;
	};

	struct termList{//由term作为数据元素的双向链表 
		termNode* front;
		termNode* tail;
		unsigned int count;
		termList(){//构造函数 
			front=NULL;
			tail=NULL;
			count=0;
		}
		~termList(){//析构函数 
			termNode* p;
			while(tail!=NULL){
				p=tail;
				tail=tail->prev;
				delete p;
			}
		}
		void push(const term& x){//将新的term插入链表尾 
			if(tail==NULL){//对于空链表的情况 
				tail=new termNode;
				tail->data=x;
				front=tail;
				tail->prev=NULL;
				tail->next=NULL;
			}
			else{//非空链表 
				tail->next=new termNode;
				tail->next->data=x;
				tail->next->prev=tail;
				tail->next->next=NULL;
				tail=tail->next;
			}
			++count;
		}
		errorInfo mergeAdjacent(termNode* t){//将操作符t的节点与邻近的操作数节点合并并且求值 
			termNode* begin;//合并后断点前节点 
			termNode* end;//合并后断点后节点 
			termNode* del;//待删除左节点 
			termNode* del2=NULL;//待删除右节点 
			double val_double;//保存浮点型的运算结果 
			int val_int;//保存整形运算结果 
			int i1,i2;//读取整形参数 
			double d1,d2;//读取浮点型参数 
			switch((t->data).category){//操作符的category属性不同，合并方式不同 
				case term::L:
					del=t;
					begin=t->prev;
					end=t->next;//设置好断点位置和待删节点的位置 
					if((t->data).attribute==term::uniInt){//根据函数的类型 
						if(begin->data.attribute==term::Int){//对参数进行类型检查 
							i1=begin->data.val.ival;
							val_int=((t->data).val.ptr4)(i1);
						}
						else{
							errorInfo tmp;
							tmp.errCode=8;
							tmp.position=begin->data.position;
							return tmp;
						}
						begin->data.category=term::None;//将计算结果保存在合并后的节点中 
						begin->data.attribute=term::Int;
						begin->data.val.ival=val_int;
					}
					else if((t->data).attribute==term::uniDouble){
						if(begin->data.attribute==term::Int){//实函数允许整型参数 
							i1=begin->data.val.ival;
							val_double=((t->data).val.ptr2)(i1);
						}
						else if(begin->data.attribute==term::Float){
							d1=begin->data.val.dval;
							val_double=((t->data).val.ptr2)(d1);
						}
						else{
							errorInfo tmp;
							tmp.errCode=10;
							tmp.position=begin->data.position;
							return tmp;
						}
						begin->data.category=term::None;
						if(equal(val_double,(int)val_double)){//如果计算结果极其接近整数，认为其类型为整数 
							begin->data.attribute=term::Int;
							begin->data.val.ival=(int)val_double;
						}
						else{
							begin->data.attribute=term::Float;
							begin->data.val.dval=val_double;
						}
					}
					break;
				case term::R://对于其他类型操作符的处理，原理类似 
					begin=t;
					end=t->next->next;
					del=t->next;
					if((t->data).attribute==term::uniInt){
						if(del->data.attribute==term::Int){
							i1=del->data.val.ival;
							val_int=((t->data).val.ptr4)(i1);
						}
						else{
							errorInfo tmp;
							tmp.errCode=8;
							tmp.position=del->data.position;
							return tmp;
						}
						begin->data.category=term::None;
						begin->data.attribute=term::Int;
						begin->data.val.ival=val_int;
					}
					else if((t->data).attribute==term::uniDouble){
						if(del->data.attribute==term::Int){
							i1=del->data.val.ival;
							val_double=((t->data).val.ptr2)(i1);
						}
						else if(del->data.attribute==term::Float){
							d1=del->data.val.dval;
							val_double=((t->data).val.ptr2)(d1);
						}
						else{
							errorInfo tmp;
							tmp.errCode=10;
							tmp.position=del->data.position;
							return tmp;
						}
						begin->data.category=term::None;
						if(equal(val_double,(int)val_double)){
							begin->data.attribute=term::Int;
							begin->data.val.ival=(int)val_double;
						}
						else{
							begin->data.attribute=term::Float;
							begin->data.val.dval=val_double;
						}				
					}
					break;
				case term::LR:
					begin=t->prev;
					end=t->next->next;
					del=t;
					del2=t->next;
					if((t->data).attribute==term::biDouble){
						if(begin->data.attribute==term::Int&&del2->data.attribute==term::Int){
							i1=begin->data.val.ival;
							i2=del2->data.val.ival;
							val_double=((t->data).val.ptr1)(i1,i2);
						}
						else if(begin->data.attribute==term::Int&&del2->data.attribute==term::Float){
							i1=begin->data.val.ival;
							d1=del2->data.val.dval;
							val_double=((t->data).val.ptr1)(i1,d1);
						}
						else if(begin->data.attribute==term::Float&&del2->data.attribute==term::Int){
							d1=begin->data.val.dval;
							i1=del2->data.val.ival;
							val_double=((t->data).val.ptr1)(d1,i1);
						}
						else if(begin->data.attribute==term::Float&&del2->data.attribute==term::Float){
							d1=begin->data.val.dval;
							d2=del2->data.val.dval;
							val_double=((t->data).val.ptr1)(d1,d2);
						}
						else{
							errorInfo tmp;
							tmp.errCode=11;
							if(begin->data.attribute!=term::Int&&begin->data.attribute!=term::Float){
								tmp.position=begin->data.position;
							}
							else if(del2->data.attribute!=term::Int&&del2->data.attribute!=term::Float){
								tmp.position=del2->data.position;
							}
							return tmp;
						}
						begin->data.category=term::None;
						if(equal(val_double,(int)val_double)){
							begin->data.attribute=term::Int;
							begin->data.val.ival=(int)val_double;
						}
						else{
							begin->data.attribute=term::Float;
							begin->data.val.dval=val_double;
						}		
					}
					else if((t->data).attribute==term::biInt){
						if(begin->data.attribute==term::Int&&del2->data.attribute==term::Int){
							i1=begin->data.val.ival;
							i2=del2->data.val.ival;
							val_int=((t->data).val.ptr3)(i1,i2);
						}
						else{
							errorInfo tmp;
							tmp.errCode=9;
							if(begin->data.attribute!=term::Int){
								tmp.position=begin->data.position;
							}
							else if(del2->data.attribute!=term::Int){
								tmp.position=del2->data.position;
							}
							return tmp;
						}
						begin->data.category=term::None;
						begin->data.attribute=term::Int;
						begin->data.val.ival=val_int;
					}
					break;
				case term::RR:
					begin=t;
					end=t->next->next->next;
					del=t->next;
					del2=t->next->next;
					if((t->data).attribute==term::biDouble){
						if(del->data.attribute==term::Int&&del2->data.attribute==term::Int){
							i1=del->data.val.ival;
							i2=del2->data.val.ival;
							val_double=((t->data).val.ptr1)(i1,i2);
						}
						else if(del->data.attribute==term::Int&&del2->data.attribute==term::Float){
							i1=del->data.val.ival;
							d1=del2->data.val.dval;
							val_double=((t->data).val.ptr1)(i1,d1);
						}
						else if(del->data.attribute==term::Float&&del2->data.attribute==term::Int){
							d1=del->data.val.dval;
							i1=del2->data.val.ival;
							val_double=((t->data).val.ptr1)(d1,i1);
						}
						else if(del->data.attribute==term::Float&&del2->data.attribute==term::Float){
							d1=del->data.val.dval;
							d2=del2->data.val.dval;
							val_double=((t->data).val.ptr1)(d1,d2);
						}
						else{
							errorInfo tmp;
							tmp.errCode=11;
							if(del->data.attribute!=term::Int&&del->data.attribute!=term::Float){
								tmp.position=del->data.position;
							}
							else if(del2->data.attribute!=term::Int&&del2->data.attribute!=term::Float){
								tmp.position=del2->data.position;
							}
							return tmp;
						}
						begin->data.category=term::None;
						if(equal(val_double,(int)val_double)){
							begin->data.attribute=term::Int;
							begin->data.val.ival=(int)val_double;
						}
						else{
							begin->data.attribute=term::Float;
							begin->data.val.dval=val_double;
						}
					}
					else if((t->data).attribute==term::biInt){
						if(del->data.attribute==term::Int&&del2->data.attribute==term::Int){
							i1=del->data.val.ival;
							i2=del2->data.val.ival;
							val_int=((t->data).val.ptr3)(i1,i2);
						}
						else{
							errorInfo tmp;
							tmp.errCode=9;
							if(del->data.attribute!=term::Int){
								tmp.position=del->data.position;
							}
							else if(del2->data.attribute!=term::Int){
								tmp.position=del2->data.position;
							}
							return tmp;
						}
						begin->data.category=term::None;
						begin->data.attribute=term::Int;
						begin->data.val.ival=val_int;
					}
					break;
				default:;
			}
			begin->next=end;//完成求值后，将断点前后节点相接 
			if(end!=NULL){//若断点后节点是实体节点 
				end->prev=begin;
			}
			else{//否则，说明尾节点是待删节点，此时重设尾节点 
				tail=begin;
			}
			delete del;//删除待删节点 
			--count;
			if(del2!=NULL){//若有两个待删节点 
				delete del2;
				--count;
			}
			errorInfo tmp;
			tmp.errCode=-1;
			return tmp;
		}
		void print(){//逐次打印每个term元素的信息 
			termNode* p=front;
			while(p!=NULL){
				p->data.print();
				p=p->next;
			}
		}
	};

	struct OprQueue{//算符优先级队列 
		queue<termNode*> zeroOrder;
		queue<termNode*> firstOrder;
		queue<termNode*> secondOrder;
		queue<termNode*> thirdOrder;
	};

	struct expression{//表达式结构体 
		termList expr;//操作符与操作数构成的链表 
		OprQueue opr;//操作符指针构成的四级优先级队列 
		
		//将各种语法单元加入到表达式中 
		void push(int val,int pos){//整型变量的加入 
			term* tmp=new term;//建立临时变量 
			tmp->attribute=term::Int;//设定其值与类型信息 
			tmp->val.ival=val;
			tmp->category=term::None;
			tmp->position=pos;//设定定位信息 
			expr.push(*tmp);//插入到链表中 
		}
		void push(double val,int pos){//浮点型变量的加入 
			term* tmp=new term;
			tmp->attribute=term::Float;
			tmp->val.dval=val;
			tmp->category=term::None;
			tmp->position=pos;
			expr.push(*tmp);
		}
		void push(char ch,int pos){//数学运算符的加入 
			term* tmp=new term;
			switch(ch){
				case '+':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=Add;//设定函数指针 
					tmp->category=term::LR;//设定函数类型 
					tmp->position=pos;
					expr.push(*tmp);
					opr.thirdOrder.push(expr.tail);//操作符除了插入链表外，还需要加入队列 
					break;
				case '-':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=Minus;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.thirdOrder.push(expr.tail);
					break;
				case '*':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=Multiply;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.secondOrder.push(expr.tail);
					break;
				case '/':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=Divide;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.secondOrder.push(expr.tail);
					break;
				case '^':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=pow;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.firstOrder.push(expr.tail);
					break;
				case '!':
					tmp->attribute=term::uniInt;
					tmp->val.ptr4=Factorial;
					tmp->category=term::L;
					tmp->position=pos;
					expr.push(*tmp);
					opr.zeroOrder.push(expr.tail);
					break;
				case '%':
					tmp->attribute=term::biInt;
					tmp->val.ptr3=Mod;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.secondOrder.push(expr.tail);
					break;
				default:cout<<"Error: Unidentified operator."<<endl;
			}
		}
		void push(double(*p)(double,double),int pos){//对于各类函数指针的处理 
			term* tmp=new term;
			tmp->attribute=term::biDouble;
			tmp->val.ptr1=p;
			tmp->category=term::RR;
			tmp->position=pos;
			expr.push(*tmp);
			opr.zeroOrder.push(expr.tail);
		}
		void push(double(*p)(double),int pos){
			term* tmp=new term;
			tmp->attribute=term::uniDouble;
			tmp->val.ptr2=p;
			tmp->category=term::R;
			tmp->position=pos;
			expr.push(*tmp);
			opr.zeroOrder.push(expr.tail);
		}
		void push(int(*p)(int,int),int pos){
			term* tmp=new term;
			tmp->attribute=term::biInt;
			tmp->val.ptr3=p;
			tmp->category=term::RR;
			tmp->position=pos;
			expr.push(*tmp);
			opr.zeroOrder.push(expr.tail);
		}
		void push(int(*p)(int),int pos){
			term* tmp=new term;
			tmp->attribute=term::uniInt;
			tmp->val.ptr4=p;
			tmp->category=term::R;
			tmp->position=pos;
			expr.push(*tmp);
			opr.zeroOrder.push(expr.tail);
		}
		void push(double(*p)(double[],int),int pos){
			term* tmp=new term;
			tmp->attribute=term::Array;
			tmp->val.ptr5=p;
			tmp->category=term::R;
			tmp->position=pos;
			expr.push(*tmp);
			opr.zeroOrder.push(expr.tail);
		}
		errorInfo calculate(){//表达式的计算 
			errorInfo err;
			err.errCode=-1;
			while(!opr.zeroOrder.isEmpty()){//首先读取最高优先级运算 
				err=expr.mergeAdjacent(opr.zeroOrder.pop());//对于最高优先级运算从左到右进行 
				if(err.errCode!=-1)return err;
			}
			while(!opr.firstOrder.isEmpty()){
				err=expr.mergeAdjacent(opr.firstOrder.pop());//依照优先级从高到低，顺序从左到右的方式进行运算 
				if(err.errCode!=-1)return err;
			}
			while(!opr.secondOrder.isEmpty()){
				err=expr.mergeAdjacent(opr.secondOrder.pop());
				if(err.errCode!=-1)return err;
			}
			while(!opr.thirdOrder.isEmpty()){
				err=expr.mergeAdjacent(opr.thirdOrder.pop());
				if(err.errCode!=-1)return err;
			}
			return err;
		}
		void print(){//打印表达式，即顺序打印termList中的元素 
			expr.print();
		}
	};

	struct result{//用于表示运算结果的值及是否出错 
		double value;
		bool error;
	};
	
	struct function{//函数结构体 
		char tag[nameLength];//函数名 
		term opr;//操作符 
	};
	
	
	struct matchTree{//变量名匹配树，本质是平衡二叉排序树 
		struct treeNode{//树节点 
			function func;
			treeNode* lch;
			treeNode* rch;
		};
		treeNode* root;//根节点 
		void createNode(treeNode*& t,function f[],int lower,int upper){
			int mid;
			if(lower<=upper){
				t=new treeNode;
				mid=(lower+upper)/2;//对于排好序的数组，递归地用数组中值作为根节点的值，即可建立平衡二叉排序树 
				tagCopy(t->func.tag,f[mid].tag);//设定当前节点的function对象的函数名tag 
				t->func.opr.attribute=f[mid].opr.attribute;//设定function对象的操作符属性 
				t->func.opr.category=f[mid].opr.category;
				switch(t->func.opr.attribute){//为function对象分配函数指针 
					case term::biDouble:
						t->func.opr.val.ptr1=f[mid].opr.val.ptr1;
						break;
					case term::uniDouble:
						t->func.opr.val.ptr2=f[mid].opr.val.ptr2;
						break;
					case term::biInt:
						t->func.opr.val.ptr3=f[mid].opr.val.ptr3;
						break;
					case term::uniInt:
						t->func.opr.val.ptr4=f[mid].opr.val.ptr4;
					case term::Array:
						t->func.opr.val.ptr5=f[mid].opr.val.ptr5;
						break;
					default:
						cout<<"Failed to construct the function table.";
				}
				createNode(t->lch,f,lower,mid-1);//递归地建立节点 
				createNode(t->rch,f,mid+1,upper);
			}
		}
		void releaseNode(treeNode*& t){//递归地删除以t为根的子树 
			if(t!=NULL){
				releaseNode(t->lch);
				releaseNode(t->rch);
				delete t;
			}
		}
		matchTree(function f[],int n){//构造函数递归调用createNode 
			createNode(root,f,0,n-1);
		}
		~matchTree(){//析构函数 
			releaseNode(root);
		}
	};
	
	bool err;//计算器的错误标志 
	int errCode;//错误码标志 
	function functionTable[100];//计算器支持的函数表 
	matchTree *fTree;//待构建的函数名匹配树 
	char errMsg[20][100]={//计算器错误信息表 
		"Unexpected ','.",
		"')' not matched.",
		"Unidentified token.",
		"Unexpected '.'.",
		"Digit numbers required after '.'.",
		"Only '+' and '-' allowed at the beginning of the expression.",
		"Illegal character after an operator.",
		"'!!' is not supported in this program.",
		"Function int (int) can not take a parameter of type double.",
		"Function int (int,int) can not take a parameter of type double.",
		"Function double (double) taking a parameter of wrong type: implication of syntax error.",
		"Function double (double,double) taking a parameter of wrong type: implication of syntax error.",
		"Operands missing.",
		"Function not defined.",
		"Parameter(s) missing.",
		"'()' required after a function.",
		"Too few parameter(s).",
		"Too many parameters.",
		"Illegal characters.",
		"Expression not finished."
	};
	
	struct val{//isSet表示该元素是否被初始化或者是否被赋过值，value保存赋值，用于函数的积分参数的保存 
		bool isSet=false;
		double value;
	};
	
	void readNumber(char* ch,int& i,expression& e,bool omit=false){//将字符串归约为数字，插入到表达式e中，omit为真表示整数部分为零且被省略 
		int ival=0;
		int orig;
		if(!omit){//整数部分未省略的情况 
			orig=i;
			ival=ch[i]-'0';
			while(isNumber(ch[++i])){
				ival=ival*10+ch[i]-'0';//归约整数值 
			}
			if(ch[i]=='\0'){//对于数字后面的字符进行语法分析 
				e.push(ival,orig);//合法者将数值插入表达式 
				return;//返回calculate函数 
			}
			else if(isOperator(ch[i])){
				e.push(ival,orig);
				return;
			}
			else if(isAlphabet(ch[i])){
				e.push(ival,orig);
				e.push('*',i);//根据上下文补充省略的乘号 
				return;
			}
			else if(ch[i]==','){//对于非法字符，分类返回错误信息 
				err=true;
				errCode=0;
				return;
			}
			else if(ch[i]==')'){
				err=true;
				errCode=1;
				return;
			}
			else if(ch[i]=='('){
				e.push(ival,orig);
				e.push('*',i);
				return;
			}
			else if(ch[i]=='.'){//读到小数点，什么也不做，执行下面的小数部分处理代码 
				;
			}
			else{
				err=true;
				errCode=2;
				return;
			}	
		}
		if(isNumber(ch[++i])){//小数点后必须有数字 
			if(omit)orig=i;
			double dval=0;
			double deci=1.0/10;
			do{
				dval+=(ch[i++]-'0')*deci;
				deci=deci/10;//归约小数部分 
			}while(isNumber(ch[i]));
			dval+=ival;//将整数部分与小数部分相加 
			e.push(dval,orig);//将结果加入表达式e中 
			if(ch[i]=='\0'){//检查后续字符是否合法 
				return;
			}
			else if(isOperator(ch[i])){
				return;
			}
			else if(isAlphabet(ch[i])){
				e.push('*',i);
				return;
			}
			else if(ch[i]==','){
				err=true;
				errCode=0;
				return;
			}
			else if(ch[i]=='('){
				e.push('*',i);
				return;
			}
			else if(ch[i]==')'){
				err=true;
				errCode=1;
				return;
			}
			else if(ch[i]=='.'){
				err=true;
				errCode=3;
				return;
			}
			else{
				err=true;
				errCode=2;
				return;
			}
		}
		else{
			err=true;
			errCode=4;
			return;
		}
	}
	int readFunction(char* ch,int& i,expression& e,int offset){//对于函数字符串进行归约 
		int orig=i;
		term func=match(ch,i);//匹配函数名 
		if(func.val.ptr1==Fail){
			i=func.position;
			return -1;
		}
		int rlt=readParameter(ch,i,e,&func,orig,offset);//读取参数 
		if(rlt==-1){//返回值-1表示readParameter在未嵌套调用calculate函数的情况下发生了错误，该错误信息未打印，需要返回上级calculate函数进行处理 
			return -1;
		}
		else if(rlt==-2){//表明readParameter嵌套调用calculate函数时发生了错误，该错误信息已经打印过，返回上级calculate函数后应该直接退出 
			return -2;
		}
		return 0;
	}
	void readOperator(char* ch,int& i,expression& e){//归约分析运算符 
		if(e.expr.tail==NULL){//开头的运算符只能是+ - 
			if(ch[i]!='+'&&ch[i]!='-'){
				err=true;
				errCode=5;
				return;
			}
			else if(ch[i]=='+'){//+ 后面若为合法字符，直接跳过 + 
				++i;
				if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
					return;
				}
				else if(ch[i]=='\0'){
					err=true;
					errCode=12;
					return;
				}
				else{
					err=true;
					errCode=6;
					return;
				}
			}
			else{//将开头出现的减号理解为前面有一个省略的零 
				++i;
				if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
					e.push(0,i-1);//补足省略 
					e.push('-',i-1);//再将减号添加到表达式中 
					return;
				}
				else if(ch[i]=='\0'){//不合语法的情况的分类处理 
					err=true;
					errCode=12;
					return;
				}
				else{
					err=true;
					errCode=6;
					return;
				}
			}
		}
		else{//不是开头出现的运算符，根据是不是!进行分类处理 
			if(ch[i]!='!'){
				if(isNumber(ch[i+1])||isAlphabet(ch[i+1])||ch[i+1]=='.'||ch[i+1]=='('){//双目运算符后面必须存在数字类的语法单元 
					e.push(ch[i],i);
					++i;
					return;
				}
				else if(ch[i+1]=='\0'){//否则出错 
					err=true;
					errCode=12;
					return;
				}
				else{
					err=true;
					errCode=6;
					return;
				}
			}
			else{//!后面可以出现数字型语法单元 
				++i;
				if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
					e.push('!',i-1);
					e.push('*',i-1);
					return;
				}
				else if(ch[i]=='\0'){//可以直接结尾 
					e.push('!',i-1);
					return;
				}
				else if(isOperator(ch[i])){
					if(ch[i]=='!'){//不能写!! 
						err=true;
						errCode=7;
						return;
					}
					else{//可以接双目运算符 
						if(isNumber(ch[i+1])||isAlphabet(ch[i+1])||ch[i+1]=='.'||ch[i+1]=='('){//但双目运算符后面必须有数字类的语法单元才合法 
							e.push('!',i-1);
							e.push(ch[i],i);
							++i;
							return;
						}
						else if(ch[i+1]=='\0'){
							err=true;
							errCode=12;
							return;
						}
						else{
							err=true;
							errCode=6;
							return;
						}
					}
				}
				else{
					err=true;
					errCode=6;
					return;
				}
			}
		}
	}
	term match(char* ch,int& i){//函数名匹配 
		int j=0;
		int orig=i;
		matchTree::treeNode* node=fTree->root;//从匹配树的根节点开始搜索 
		while(isAlphabet(ch[i])||node->func.tag[j]!='\0'){//读取的字符串与匹配树的tag中至少有一个没有匹配完毕 
			while(node!=NULL){
				if(!isAlphabet(ch[i])){//如果字母串已经读取完毕，tag的字符串却没有读取完毕，有可能字母串是sin而tag为sinh，这时把字母串视为******\0，\0小于其他任何字符，因此应当在左子树中继续匹配查找 
					i=orig;
					j=0;
					node=node->lch;
				}
				else if(ch[i]>node->func.tag[j]){//节点跳转后，i,j均需回溯到最初位置 
					i=orig;
					j=0;
					node=node->rch;
				}
				else if(ch[i]<node->func.tag[j]){
					i=orig;
					j=0;
					node=node->lch;
				}
				else if(ch[i]==node->func.tag[j]){//当前字符匹配成功，继续匹配下一字符 
					++i;
					++j;
					break;
				}
			}
			if(node==NULL)break;
		}
		if(node==NULL){//匹配失败，返回Fail函数指针 
			err=true;
			errCode=13;
			term tmp;
			tmp.val.ptr1=Fail;
			tmp.attribute=term::biDouble;
			tmp.category=term::RR;
			tmp.position=orig;
			return tmp;
		}
		else if(!isAlphabet(ch[i])&&node->func.tag[j]=='\0'){//字母串刚好结束，tag字符串也刚好结束，说明匹配成功 
			term tmp;
			tmp.attribute=node->func.opr.attribute;
			tmp.category=node->func.opr.category;
			tmp.position=orig;
			switch(node->func.opr.attribute){//将function节点的函数指针赋给term对象 
				case term::biDouble:
					tmp.val.ptr1=node->func.opr.val.ptr1;
					break;
				case term::uniDouble:
					tmp.val.ptr2=node->func.opr.val.ptr2;
					break;
				case term::biInt:
					tmp.val.ptr3=node->func.opr.val.ptr3;
					break;
				case term::uniInt:
					tmp.val.ptr4=node->func.opr.val.ptr4;
				case term::Array:
					tmp.val.ptr5=node->func.opr.val.ptr5;
					break;
				default:
					cout<<"Exception";
			}
			return tmp;
		}
		else{//否则说明匹配失败，返回Fail函数指针 
			err=true;
			errCode=13;
			term tmp;
			tmp.val.ptr1=Fail;
			tmp.attribute=term::biDouble;
			tmp.category=term::RR;
			tmp.position=orig;
			return tmp;
		}
	}
	int readParameter(char* ch,int& i,expression& e,term* t,int begin,int offset){//读取函数参数 
		if(ch[i]=='\0'){
			err=true;
			errCode=14;
			return -1;
		}
		else if(ch[i]!='('){
			err=true;
			errCode=15;
			return -1;
		}
		else{
			int lbr=0;//对子字符串中的左右括号进行计数 
			int rbr=0;
			int paraNum;
			if(t->attribute==term::uniDouble||t->attribute==term::uniInt){//根据函数名匹配得到的函数的类型信息，设置参数个数变量 
				paraNum=1;
			}
			else if(t->attribute==term::biDouble||t->attribute==term::biInt){
				paraNum=2;
			}
			else if(t->attribute==term::Array){//参数个数不定的函数的最大参数个数为1024 
				paraNum=1024;
			}
			double para[paraNum];//建立参数数组 
			int paraCnt;//统计实际出现的参数个数 
			bool integral=false;//根据语法判断用户是否要求积分 
			val intVal[paraNum][3];//对积分下限、上限、步长进行保存 
			if(ch[i+1]==')'){//参数括号为空，实际参数个数为0 
				paraCnt=0;
			}
			else{
				int orig=i;
				paraCnt=1;
				int iParaCnt=0;
				while(rbr<=lbr){//右括号计数多于左括号时，参数归约结束 
					++i;
					if(ch[i]=='('){//对括号进行计数 
						++lbr;
					}
					else if(ch[i]==')'){
						++rbr;
					}
					if(ch[i]=='\0'){//表达式不完整 
						err=true;
						errCode=19;
						return -1;
					}
					if(lbr==rbr&&ch[i]==':'){//中间出现: 表明用户要进行积分操作 
						integral=true;
						intVal[paraCnt-1][iParaCnt].value=calculate(substr(ch,orig,i),offset+orig+1).value;//将括号、冒号、逗号隔开的字符串归约求值，保存在积分限数组中 
						intVal[paraCnt-1][iParaCnt].isSet=true;
						++iParaCnt;
						orig=i;
					}
					if(lbr==rbr&&ch[i]==','){//遇到逗号分隔多个参数 
						++paraCnt;
						if(paraCnt>paraNum){//检查参数个数是否超出应有值 
							err=true;
							errCode=17;
							return -1;
						}
						else{
							if(!integral){//非积分情况下，将前一个参数求值保存在参数数组中 
								result r=calculate(substr(ch,orig,i),offset+orig+1);
								if(!r.error){
									para[paraCnt-2]=r.value;
								}
								else{
									return -2;
								}
								orig=i;
							}
							else{//积分情况下，把前一个参数的步长信息保存在积分限数组中 
								intVal[paraCnt-2][iParaCnt].value=calculate(substr(ch,orig,i),offset+orig+1).value;
								intVal[paraCnt-2][iParaCnt].isSet=true;
								orig=i;
								iParaCnt=0;
							}
						}
					}
				}
				//跳出括号后 
				if(!integral){//非积分条件下，对最后一个参数进行求值 
					result r=calculate(substr(ch,orig,i),offset+orig+1);
					if(!r.error){
						para[paraCnt-1]=r.value;
					}
					else{
						return -2;//若嵌套的calculate求值发生错误，返回值为-2 
					}
				}
				else{//积分条件下，将最后一个参数的步长信息保存在积分限数组中 
					intVal[paraCnt-1][iParaCnt].value=calculate(substr(ch,orig,i),offset+orig+1).value;
					intVal[paraCnt-1][iParaCnt].isSet=true;
					orig=i;
					double fSum=0.0;
					if(paraNum==1){//根据应有参数的个数进行积分近似求值 
						for(double d=intVal[0][0].value;d<=intVal[0][1].value;d+=intVal[0][2].value){
							fSum+=t->val.ptr2(d);
						}
						fSum=fSum*intVal[0][2].value;
						e.push(fSum,begin);
					}
					else if(paraNum==2){
						for(double d1=intVal[0][0].value;d1<=intVal[0][1].value;d1+=intVal[0][2].value){
							for(double d2=intVal[1][0].value;d2<=intVal[1][1].value;d2+=intVal[1][2].value){
								fSum+=t->val.ptr1(d1,d2);
							}
						}
						fSum=fSum*intVal[0][2].value*intVal[1][2].value;
						e.push(fSum,begin);
					}
					++i;
					if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){//处理函数后面的字符是否合法，是否需要补足缩略 
						e.push('*',i);
						return 0;
					}
					else if(isOperator(ch[i])||ch[i]=='\0'){
						return 0;
					}
					else{
						err=true;
						errCode=18;
						return -1;
					}
				}	
			}
			if(paraCnt<paraNum&&paraNum!=1024){//对于参数个数固定的函数，如果参数个数统计多于应有参数个数 
				err=true;
				errCode=16;
				return -1;
			}
			switch(t->attribute){//将读取得到的参数代入到函数指针中进行计算 
				case term::biDouble:
					if(equal(t->val.ptr1(para[0],para[1]),(int)t->val.ptr1(para[0],para[1]))){//结果近似为整数的，视为整型变量 
						e.push((int)t->val.ptr1(para[0],para[1]),begin);
					}
					else{
						e.push(t->val.ptr1(para[0],para[1]),begin);
					}
					break;
				case term::uniDouble:
					if(equal(t->val.ptr2(para[0]),(int)t->val.ptr2(para[0]))){
						e.push((int)t->val.ptr2(para[0]),begin);
					}
					else{
						e.push(t->val.ptr2(para[0]),begin);
					}
					break;
				case term::biInt:
					e.push(t->val.ptr3(para[0],para[1]),begin);
					break;
				case term::uniInt:
					e.push(t->val.ptr4(para[0]),begin);
					break;
				case term::Array:
					if(equal(t->val.ptr5(para,paraCnt),(int)t->val.ptr5(para,paraCnt))){//参数个数不固定的函数的操作 
						e.push((int)t->val.ptr5(para,paraCnt),begin);
					}
					else{
						e.push(t->val.ptr5(para,paraCnt),begin);
					}
					break;
				default:cout<<"Error";
			}
			++i;
			if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){//处理后序字符的合法性 
				e.push('*',i);
				return 0;
			}
			else if(isOperator(ch[i])||ch[i]=='\0'){
				return 0;
			}
			else{
				err=true;
				errCode=18;
				return -1;
			}		
		}
	}
public:
	calculator(){//calculator的构造函数 
		tagCopy(functionTable[0].tag,"abs");//建立函数表，其中tag字符串按照字典序升序排列 
		tagCopy(functionTable[1].tag,"acos");
		tagCopy(functionTable[2].tag,"asin");
		tagCopy(functionTable[3].tag,"atan");
		tagCopy(functionTable[4].tag,"avg");
		tagCopy(functionTable[5].tag,"cbrt");
		tagCopy(functionTable[6].tag,"cos");
		tagCopy(functionTable[7].tag,"cosh");
		tagCopy(functionTable[8].tag,"exp");
		tagCopy(functionTable[9].tag,"ln");
		tagCopy(functionTable[10].tag,"log");
		tagCopy(functionTable[11].tag,"pow");
		tagCopy(functionTable[12].tag,"sd");
		tagCopy(functionTable[13].tag,"sin");
		tagCopy(functionTable[14].tag,"sinh");
		tagCopy(functionTable[15].tag,"sqrt");
		tagCopy(functionTable[16].tag,"sum");
		tagCopy(functionTable[17].tag,"tan");
		tagCopy(functionTable[18].tag,"tanh");
		tagCopy(functionTable[19].tag,"var");
		functionTable[0].opr.val.ptr2=fabs;//为计算器支持的函数赋予相应的属性和函数指针 
		functionTable[0].opr.attribute=term::uniDouble;
		functionTable[0].opr.category=term::R;
		functionTable[1].opr.val.ptr2=acos;
		functionTable[1].opr.attribute=term::uniDouble;
		functionTable[1].opr.category=term::R;
		functionTable[2].opr.val.ptr2=asin;
		functionTable[2].opr.attribute=term::uniDouble;
		functionTable[2].opr.category=term::R;
		functionTable[3].opr.val.ptr2=atan;
		functionTable[3].opr.attribute=term::uniDouble;
		functionTable[3].opr.category=term::R;
		functionTable[4].opr.val.ptr5=Average;
		functionTable[4].opr.attribute=term::Array;
		functionTable[4].opr.category=term::R;
		functionTable[5].opr.val.ptr2=cbrt;
		functionTable[5].opr.attribute=term::uniDouble;
		functionTable[5].opr.category=term::R;
		functionTable[6].opr.val.ptr2=cos;
		functionTable[6].opr.attribute=term::uniDouble;
		functionTable[6].opr.category=term::R;
		functionTable[7].opr.val.ptr2=cosh;
		functionTable[7].opr.attribute=term::uniDouble;
		functionTable[7].opr.category=term::R;
		functionTable[8].opr.val.ptr2=exp;
		functionTable[8].opr.attribute=term::uniDouble;
		functionTable[8].opr.category=term::R;
		functionTable[9].opr.val.ptr2=log;
		functionTable[9].opr.attribute=term::uniDouble;
		functionTable[9].opr.category=term::R;
		functionTable[10].opr.val.ptr2=log10;
		functionTable[10].opr.attribute=term::uniDouble;
		functionTable[10].opr.category=term::R;
		functionTable[11].opr.val.ptr1=pow;
		functionTable[11].opr.attribute=term::biDouble;
		functionTable[11].opr.category=term::RR;
		functionTable[12].opr.val.ptr5=Sd;
		functionTable[12].opr.attribute=term::Array;
		functionTable[12].opr.category=term::R;
		functionTable[13].opr.val.ptr2=sin;
		functionTable[13].opr.attribute=term::uniDouble;
		functionTable[13].opr.category=term::R;
		functionTable[14].opr.val.ptr2=sinh;
		functionTable[14].opr.attribute=term::uniDouble;
		functionTable[14].opr.category=term::R;
		functionTable[15].opr.val.ptr2=sqrt;
		functionTable[15].opr.attribute=term::uniDouble;
		functionTable[15].opr.category=term::R;
		functionTable[16].opr.val.ptr5=Sum;
		functionTable[16].opr.attribute=term::Array;
		functionTable[16].opr.category=term::R;
		functionTable[17].opr.val.ptr2=tan;
		functionTable[17].opr.attribute=term::uniDouble;
		functionTable[17].opr.category=term::R;
		functionTable[18].opr.val.ptr2=tanh;
		functionTable[18].opr.attribute=term::uniDouble;
		functionTable[18].opr.category=term::R;
		functionTable[19].opr.val.ptr5=Var;
		functionTable[19].opr.attribute=term::Array;
		functionTable[19].opr.category=term::R;
		fTree=new matchTree(functionTable,20);//有上面构建好的数组构造平衡二叉排序树 
	}
	~calculator(){//析构函数 
		delete fTree;
	}
	result calculate(char* ch,int offset=0){//解析表达式字符串并求值的主体函数，时间复杂度一般为O(n)，最坏为O(n^2)，空间复杂度一般为O(n)，最坏为O(n^2) 
		err=false;
		errCode=-1;//重置错误码 
		if(ch[0]=='\0'){//空字符串返回0 
			result tmp;
			tmp.value=0;
			tmp.error=false;
			return tmp;
		}
		int i=0;
		expression* e=new expression;//为字符串构造表达式 
		while(ch[i]!='\0'){
			if(isNumber(ch[i])){//归约数字 
				readNumber(ch,i,*e);
			}
			else if(ch[i]=='.'){//归约省略整数部分的小数 
				readNumber(ch,i,*e,true);
			}
			else if(isOperator(ch[i])){//归约操作符 
				readOperator(ch,i,*e);
			}
			else if(isAlphabet(ch[i])){//归约函数 
				int rlt=readFunction(ch,i,*e,offset);
				if(rlt==-2){//嵌套调用的错误，直接退出程序执行过程 
					result tmp;
					tmp.value=0;
					tmp.error=true;
					return tmp;
				}
			}
			else if(ch[i]=='('){//对于括号中子字符串的处理 
				int lbr=0;
				int rbr=0;
				int orig=i;
				while(rbr<=lbr){
					++i;
					if(ch[i]=='('){//对括号进行计数 
						++lbr;
					}
					if(ch[i]==')'){
						++rbr;
					}
					if(ch[i]=='\0'){
						err=true;
						errCode=1;
						i=orig;
						break;
					}
				}
				if(!err){
					result r=calculate(substr(ch,orig,i),orig+1);//子字符串求值 
					if(!r.error){
						if(equal(r.value,(int)r.value)){
							e->push((int)r.value,orig+1);
						}
						else{
							e->push(r.value,orig+1);
						}
						++i;
						if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='('||ch[i]=='.'){//补充省略的乘号 
							e->push('*',i);
						}
					}
					else{
						result tmp;
						tmp.value=0;
						tmp.error=true;
						return tmp;
					}
				}
			}
			else{
				err=true;
				errCode=2;
			}
			if(err){//检查err是否被归约函数设置为true，打印错误信息 
				cout<<"Position:"<<offset+i+1<<" Error Code:"<<errCode<<endl<<errMsg[errCode];
				result tmp;
				tmp.value=0;
				tmp.error=true;
				return tmp;
			}
		}
		errorInfo eInfo=e->calculate();//遍历字符串结束后 
		if(eInfo.errCode!=-1){//若对于表达式的calculate方法执行出错 
			cout<<"Position:"<<offset+eInfo.position+1<<" Error Code:"<<eInfo.errCode<<endl<<errMsg[eInfo.errCode];
			result tmp;
			tmp.value=0;
			tmp.error=true;
			return tmp;
		}
		if(offset==0){//否则，如果offset为0，说明该calculate函数归约的是整个表达式，此时整个表达式归约完毕，输出结果。否则，子表达式归约完毕，不输出结果 
			cout<<"=";
			e->print();
		}
		result tmp;//构造返回值对象，为递归和嵌套调用服务 
		if(e->expr.front->data.attribute==term::Float){
			tmp.value=e->expr.front->data.val.dval;
		}
		else if(e->expr.front->data.attribute==term::Int){
			tmp.value=e->expr.front->data.val.ival;
		}
		tmp.error=false;
		delete e;
		return tmp;
	}
};

int main(){
	calculator c;
	char expr[1000];
	cout<<"Scientific Calculator"<<endl;//输出功能介绍界面 
	cout<<"Operators supported: + - * / ^ % ! ()"<<endl;
	cout<<"Functions supported: abs acos asin atan avg cbrt cos cosh exp ln log pow sd sin sinh sqrt sum tan tanh var"<<endl;
	cout<<"Enter '@' to quit this program."<<endl;
	while(1){
		cin>>expr;
		if(expr[0]=='@'){
			break;
		}
		else{
			c.calculate(expr);
			cout<<endl;
		}
	}
	return 0;
}
