#include<iostream>
#include<iomanip>
#include<cmath>
#define debug
using namespace std;

template<typename T>class queue{
private:
	static const int BLOCKSIZE=1024;
	template<typename S>struct queueNode{
		S data[BLOCKSIZE];
		queueNode<S>* prev;
		queueNode<S>* next;
	};
	queueNode<T>* frontNode;
	queueNode<T>* tailNode;
	T* frontPtr;
	T* tailPtr;
public:
	queue(){
		tailNode=new queueNode<T>;
		frontNode=tailNode;
		tailNode->prev=NULL;
		tailNode->next=NULL;
		frontPtr=(frontNode->data)-1;
		tailPtr=(tailNode->data)-1;
	}
	~queue(){
		queueNode<T>* ptr;
		while(tailNode!=NULL){
			ptr=tailNode;
			tailNode=tailNode->prev;
			delete ptr;
		}
	}
	void push(const T& x){
		if(tailPtr-(tailNode->data)==BLOCKSIZE-1){
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
	T pop(){
		if(frontPtr==(tailNode->data)-1)return NULL;
		T x=*frontPtr;
		if(frontPtr==tailPtr){
			frontPtr=(frontNode->data)-1;
			tailPtr=(tailNode->data)-1;
		}
		else if(frontPtr-(frontNode->data)==BLOCKSIZE-1){
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
	bool isEmpty(){
		if(tailNode->prev==NULL&&tailPtr==(tailNode->data)-1)return true;
		return false;
	}
};

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
int Mod(int a,int b){
	return a%b;
}
int Factorial(int a){
	if(a<=0){
		return 1;
	}
	else{
		return a*Factorial(a-1);
	}
}
double Fail(double a,double b){
	return -1;
}
bool isNumber(char ch){
	if(ch>='0'&&ch<='9')return true;
	return false;
}
bool isAlphabet(char ch){
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))return true;
	return false;
}
bool isOperator(char ch){
	if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^'||ch=='!'||ch=='%')return true;
	return false;
}
const int precision=10;
bool equal(double a,double b){
	if(abs(a-b)<=0.000000000001)return true;
	return false;
}
char* substr(char* ch,int begin,int end){
	char* subCh;
	if(end-begin-1>0){
		subCh=new char[end-begin];
		for(int i=0;i<end-begin-1;i++){
			subCh[i]=ch[begin+i+1];
		}
		subCh[end-begin-1]='\0';
	}
	else{
		subCh=new char[1];
		subCh[0]='\0';
	}
	return subCh;
}
const int nameLength=10;

void tagCopy(char* dest,char* src,int n=nameLength){
	for(int i=0;i<n-1;i++){
		dest[i]=src[i];
	}
	dest[n-1]='\0';
}
class calculator{
private:
	struct errorInfo{
		int position;
		int errCode;
	};
	struct term{
		union{
			int ival;
			double dval;
			double(*ptr1)(double,double);
			double(*ptr2)(double);
			int(*ptr3)(int,int);
			int(*ptr4)(int);
		}val;
		enum{
			Int,Float,biDouble,uniDouble,biInt,uniInt
		}attribute;
		enum{
			None,L,R,LR,RR
		}category;
		int position;
		void print(){
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
				default:
					cout<<"Error";
			}
		}
	};

	struct termNode{
		term data;
		termNode* prev;
		termNode* next;
	};

	struct termList{
		termNode* front;
		termNode* tail;
		unsigned int count;
		termList(){
			front=NULL;
			tail=NULL;
			count=0;
		}
		~termList(){
			termNode* p;
			while(tail!=NULL){
				p=tail;
				tail=tail->prev;
				delete p;
			}
		}
		void push(const term& x){
			if(tail==NULL){
				tail=new termNode;
				tail->data=x;
				front=tail;
				tail->prev=NULL;
				tail->next=NULL;
			}
			else{
				tail->next=new termNode;
				tail->next->data=x;
				tail->next->prev=tail;
				tail->next->next=NULL;
				tail=tail->next;
			}
			++count;
		}
		errorInfo mergeAdjacent(termNode* t){
			termNode* begin;
			termNode* end;
			termNode* del;
			termNode* del2=NULL;
			double val_double;
			int val_int;
			int i1,i2;
			double d1,d2;
			switch((t->data).category){
				case term::L:
					del=t;
					begin=t->prev;
					end=t->next;
					if((t->data).attribute==term::uniInt){
						if(begin->data.attribute==term::Int){
							i1=begin->data.val.ival;
							val_int=((t->data).val.ptr4)(i1);
						}
						else{
							errorInfo tmp;
							tmp.errCode=8;
							tmp.position=begin->data.position;
							return tmp;
						}
						begin->data.category=term::None;
						begin->data.attribute=term::Int;
						begin->data.val.ival=val_int;
					}
					else if((t->data).attribute==term::uniDouble){
						if(begin->data.attribute==term::Int){
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
				case term::R:
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
			begin->next=end;
			if(end!=NULL){
				end->prev=begin;
			}
			else{
				tail=begin;
			}
			delete del;
			--count;
			if(del2!=NULL){
				delete del2;
				--count;
			}
			errorInfo tmp;
			tmp.errCode=-1;
			return tmp;
		}
		void print(){
			termNode* p=front;
			while(p!=NULL){
				p->data.print();
				p=p->next;
			}
		}
	};

	struct OprQueue{
		queue<termNode*> zeroOrder;
		queue<termNode*> firstOrder;
		queue<termNode*> secondOrder;
		queue<termNode*> thirdOrder;
	};

	struct expression{
		termList expr;
		OprQueue opr;
		void push(int val,int pos){
			term* tmp=new term;
			tmp->attribute=term::Int;
			tmp->val.ival=val;
			tmp->category=term::None;
			tmp->position=pos;
			expr.push(*tmp);
		}
		void push(double val,int pos){
			term* tmp=new term;
			tmp->attribute=term::Float;
			tmp->val.dval=val;
			tmp->category=term::None;
			tmp->position=pos;
			expr.push(*tmp);
		}
		void push(char ch,int pos){
			term* tmp=new term;
			switch(ch){
				case '+':
					tmp->attribute=term::biDouble;
					tmp->val.ptr1=Add;
					tmp->category=term::LR;
					tmp->position=pos;
					expr.push(*tmp);
					opr.thirdOrder.push(expr.tail);
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
		void push(double(*p)(double,double),int pos){
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
		errorInfo calculate(){
			errorInfo err;
			err.errCode=-1;
			while(!opr.zeroOrder.isEmpty()){
				err=expr.mergeAdjacent(opr.zeroOrder.pop());
				if(err.errCode!=-1)return err;
			}
			while(!opr.firstOrder.isEmpty()){
				err=expr.mergeAdjacent(opr.firstOrder.pop());
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
		void print(){
			expr.print();
		}
	};

	struct result{
		double value;
		bool error;
	};
	
	struct function{
		char tag[nameLength];
		term opr;
	};
	
	
	struct matchTree{
		struct treeNode{
			function func;
			treeNode* lch;
			treeNode* rch;
		};
		treeNode* root;
		void createNode(treeNode*& t,function f[],int lower,int upper){
			int mid;
			if(lower<=upper){
				t=new treeNode;
				mid=(lower+upper)/2;
				tagCopy(t->func.tag,f[mid].tag);
				t->func.opr.attribute=f[mid].opr.attribute;
				t->func.opr.category=f[mid].opr.category;
				switch(t->func.opr.attribute){
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
						break;
					default:
						cout<<"Failed to construct the function table.";
				}
				createNode(t->lch,f,lower,mid-1);
				createNode(t->rch,f,mid+1,upper);
			}
			else{
				t=NULL;
			}
		}
		void releaseNode(treeNode*& t){
			if(t!=NULL){
				releaseNode(t->lch);
				releaseNode(t->rch);
				delete t;
			}
		}
		matchTree(function f[],int n){
			createNode(root,f,0,n-1);
		}
		~matchTree(){
			releaseNode(root);
		}
	};
	bool err;
	int errCode;
	function functionTable[100];
	matchTree *fTree;
	char errMsg[20][100]={
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
	void readNumber(char* ch,int& i,expression& e,bool omit=false){
		int ival=0;
		int orig;
		if(!omit){
			orig=i;
			ival=ch[i]-'0';
			while(isNumber(ch[++i])){
				ival=ival*10+ch[i]-'0';
			}
			if(ch[i]=='\0'){
				e.push(ival,orig);
				return;
			}
			else if(isOperator(ch[i])){
				e.push(ival,orig);
				return;
			}
			else if(isAlphabet(ch[i])){
				e.push(ival,orig);
				e.push('*',i);
				return;
			}
			else if(ch[i]==','){
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
			else if(ch[i]=='.'){
				;
			}
			else{
				err=true;
				errCode=2;
				return;
			}	
		}
		if(isNumber(ch[++i])){
			if(omit)orig=i;
			double dval=0;
			double deci=1.0/10;
			do{
				dval+=(ch[i++]-'0')*deci;
				deci=deci/10;
			}while(isNumber(ch[i]));
			dval+=ival;
			e.push(dval,orig);
			if(ch[i]=='\0'){
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
	int readFunction(char* ch,int& i,expression& e,int offset){
		int orig=i;
		term func=match(ch,i);
		if(func.val.ptr1==Fail){
			i=func.position;
			return -1;
		}
		int rlt=readParameter(ch,i,e,&func,orig,offset);
		if(rlt==-1){
			return -1;
		}
		else if(rlt==-2){
			return -2;
		}
		return 0;
	}
	void readOperator(char* ch,int& i,expression& e){
		if(e.expr.tail==NULL){
			if(ch[i]!='+'&&ch[i]!='-'){
				err=true;
				errCode=5;
				return;
			}
			else if(ch[i]=='+'){
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
			else{
				++i;
				if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
					e.push(0,i-1);
					e.push('-',i-1);
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
		}
		else{
			if(ch[i]!='!'){
				if(isNumber(ch[i+1])||isAlphabet(ch[i+1])||ch[i+1]=='.'||ch[i+1]=='('){
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
			else{
				++i;
				if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
					e.push('!',i-1);
					e.push('*',i-1);
					return;
				}
				else if(ch[i]=='\0'){
					e.push('!',i-1);
					return;
				}
				else if(isOperator(ch[i])){
					if(ch[i]=='!'){
						err=true;
						errCode=7;
						return;
					}
					else{
						if(isNumber(ch[i+1])||isAlphabet(ch[i+1])||ch[i+1]=='.'||ch[i+1]=='('){
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
	term match(char* ch,int& i){
		int j=0;
		int orig=i;
		matchTree::treeNode* node=fTree->root;
		while(isAlphabet(ch[i])&&node->func.tag[j]!='\0'){
			while(node!=NULL){
				if(ch[i]>node->func.tag[j]){
					i=orig;
					j=0;
					node=node->rch;
				}
				else if(ch[i]<node->func.tag[j]){
					i=orig;
					j=0;
					node=node->lch;
				}
				else if(ch[i]==node->func.tag[j]){
					++i;
					++j;
					break;
				}
			}
			if(node==NULL)break;
		}
		if(node==NULL){
			err=true;
			errCode=13;
			term tmp;
			tmp.val.ptr1=Fail;
			tmp.attribute=term::biDouble;
			tmp.category=term::RR;
			tmp.position=orig;
			return tmp;
		}
		else if(!isAlphabet(ch[i])&&node->func.tag[j]=='\0'){
			term tmp;
			tmp.attribute=node->func.opr.attribute;
			tmp.category=node->func.opr.category;
			tmp.position=orig;
			switch(node->func.opr.attribute){
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
					break;
				default:
					cout<<"Exception";
			}
			return tmp;
		}
		else{
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
	int readParameter(char* ch,int& i,expression& e,term* t,int begin,int offset){
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
			int lbr=0;
			int rbr=0;
			int paraNum;
			if(t->attribute==term::uniDouble||t->attribute==term::uniInt){
				paraNum=1;
			}
			else if(t->attribute==term::biDouble||t->attribute==term::biInt){
				paraNum=2;
			}
			double para[paraNum];
			int paraCnt;
			if(ch[i+1]==')'){
				paraCnt=0;
			}
			else{
				int orig=i;
				paraCnt=1;
				while(rbr<=lbr){
					++i;
					if(ch[i]=='('){
						++lbr;
					}
					else if(ch[i]==')'){
						++rbr;
					}
					if(ch[i]=='\0'){
						err=true;
						errCode=19;
						return -1;
					}
					if(lbr==rbr&&ch[i]==','){
						++paraCnt;
						if(paraCnt>paraNum){
							err=true;
							errCode=17;
							return -1;
						}
						else{
							result r=calculate(substr(ch,orig,i),offset+orig+1);
							if(!r.error){
								para[paraCnt-2]=r.value;
							}
							else{
								return -2;
							}
							orig=i;
						}
					}
				}
				result r=calculate(substr(ch,orig,i),offset+orig+1);
				if(!r.error){
					para[paraCnt-1]=r.value;
				}
				else{
					return -2;
				}
			}
			if(paraCnt<paraNum){
				err=true;
				errCode=16;
				return -1;
			}
			switch(t->attribute){
				case term::biDouble:
					if(equal(t->val.ptr1(para[0],para[1]),(int)t->val.ptr1(para[0],para[1]))){
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
				default:cout<<"Error";
			}
			++i;
			if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
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
	calculator(){
		tagCopy(functionTable[0].tag,"abs");
		tagCopy(functionTable[1].tag,"acos");
		tagCopy(functionTable[2].tag,"asin");
		tagCopy(functionTable[3].tag,"atan");
		tagCopy(functionTable[4].tag,"cos");
		tagCopy(functionTable[5].tag,"exp");
		tagCopy(functionTable[6].tag,"ln");
		tagCopy(functionTable[7].tag,"log");
		tagCopy(functionTable[8].tag,"pow");
		tagCopy(functionTable[9].tag,"sin");
		tagCopy(functionTable[10].tag,"sqrt");
		tagCopy(functionTable[11].tag,"tan");
		functionTable[0].opr.val.ptr2=fabs;
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
		functionTable[4].opr.val.ptr2=cos;
		functionTable[4].opr.attribute=term::uniDouble;
		functionTable[4].opr.category=term::R;
		functionTable[5].opr.val.ptr2=exp;
		functionTable[5].opr.attribute=term::uniDouble;
		functionTable[5].opr.category=term::R;
		functionTable[6].opr.val.ptr2=log;
		functionTable[6].opr.attribute=term::uniDouble;
		functionTable[6].opr.category=term::R;
		functionTable[7].opr.val.ptr2=log10;
		functionTable[7].opr.attribute=term::uniDouble;
		functionTable[7].opr.category=term::R;
		functionTable[8].opr.val.ptr1=pow;
		functionTable[8].opr.attribute=term::biDouble;
		functionTable[8].opr.category=term::RR;
		functionTable[9].opr.val.ptr2=sin;
		functionTable[9].opr.attribute=term::uniDouble;
		functionTable[9].opr.category=term::R;
		functionTable[10].opr.val.ptr2=sqrt;
		functionTable[10].opr.attribute=term::uniDouble;
		functionTable[10].opr.category=term::R;
		functionTable[11].opr.val.ptr2=tan;
		functionTable[11].opr.attribute=term::uniDouble;
		functionTable[11].opr.category=term::R;
		fTree=new matchTree(functionTable,12);
	}
	~calculator(){
		delete fTree;
	}
	result calculate(char* ch,int offset=0){
		err=false;
		errCode=-1;
		if(ch[0]=='\0'){
			result tmp;
			tmp.value=0;
			tmp.error=false;
			return tmp;
		}
		int i=0;
		expression* e=new expression;
		while(ch[i]!='\0'){
			if(isNumber(ch[i])){
				readNumber(ch,i,*e);
			}
			else if(ch[i]=='.'){
				readNumber(ch,i,*e,true);
			}
			else if(isOperator(ch[i])){
				readOperator(ch,i,*e);
			}
			else if(isAlphabet(ch[i])){
				int rlt=readFunction(ch,i,*e,offset);
				if(rlt==-2){
					result tmp;
					tmp.value=0;
					tmp.error=true;
					return tmp;
				}
			}
			else if(ch[i]=='('){
				int lbr=0;
				int rbr=0;
				int orig=i;
				while(rbr<=lbr){
					++i;
					if(ch[i]=='('){
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
					result r=calculate(substr(ch,orig,i),orig+1);
					if(!r.error){
						if(equal(r.value,(int)r.value)){
							e->push((int)r.value,orig+1);
						}
						else{
							e->push(r.value,orig+1);
						}
						++i;
						if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='('||ch[i]=='.'){
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
			if(err){
				cout<<"Position:"<<offset+i+1<<" Error Code:"<<errCode<<endl<<errMsg[errCode];
				result tmp;
				tmp.value=0;
				tmp.error=true;
				return tmp;
			}
		}
		errorInfo eInfo=e->calculate();
		if(eInfo.errCode!=-1){
			cout<<"Position:"<<offset+eInfo.position+1<<" Error Code:"<<eInfo.errCode<<endl<<errMsg[eInfo.errCode];
			result tmp;
			tmp.value=0;
			tmp.error=true;
			return tmp;
		}
		if(offset==0){
			cout<<"=";
			e->print();
		}
		result tmp;
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
	cout<<"Scientific Calculator"<<endl;
	cout<<"Operators supported: + - * / ^ % ! ()"<<endl;
	cout<<"Functions supported: abs acos asin atan cos exp ln log pow sin sqrt tan"<<endl;
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
