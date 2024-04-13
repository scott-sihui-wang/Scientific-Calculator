#include<iostream>
#include<iomanip>
#include<cmath>
#include<queue>
using namespace std;
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
	return (ch>='0'&&ch<='9');
}
bool isAlphabet(char ch){
	return ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'));
}
bool isOperator(char ch){
	return (ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='^'||ch=='!'||ch=='%');
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
		errorInfo(int code):errCode(code){}
		errorInfo(int code,int pos):errCode(code),position(pos){}
	};
	struct term{
		union termValue{
			int ival;
			double dval;
			double(*ptr1)(double,double);
			double(*ptr2)(double);
			int(*ptr3)(int,int);
			int(*ptr4)(int);
		}val;
		enum termType{
			Int,Float,biDouble,uniDouble,biInt,uniInt
		}attribute;
		enum oprType{
			None,L,R,LR,RR
		}category;
		int position;
		term(){}
		term(int value,int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.ival=value;
		}
		term(double value,int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.dval=value;
		}
		term(double(*p)(double,double),int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.ptr1=p;
		}
		term(double(*p)(double),int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.ptr2=p;
		}
		term(int(*p)(int,int),int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.ptr3=p;
		}
		term(int(*p)(int),int pos,termType term_type,oprType opr_type):attribute(term_type),category(opr_type),position(pos){
			val.ptr4=p;
		}
		void print(){
			switch(attribute){
				case Int:
					cout<<val.ival;
					break;
				case Float:
					cout<<setprecision(precision)<<val.dval;
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
		termList(){
			front=NULL;
			tail=NULL;
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
							return errorInfo(8,begin->data.position);
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
							return errorInfo(10,begin->data.position);
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
							return errorInfo(8,del->data.position);
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
							return errorInfo(10,del->data.position);
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
							if(begin->data.attribute!=term::Int&&begin->data.attribute!=term::Float){
								return errorInfo(11,begin->data.position);
							}
							else if(del2->data.attribute!=term::Int&&del2->data.attribute!=term::Float){
								return errorInfo(11,del2->data.position);
							}
							return errorInfo(11);
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
							if(begin->data.attribute!=term::Int){
								return errorInfo(9,begin->data.position);
							}
							else if(del2->data.attribute!=term::Int){
								return errorInfo(9,del2->data.position);
							}
							return errorInfo(9);
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
							if(del->data.attribute!=term::Int&&del->data.attribute!=term::Float){
								return errorInfo(11,del->data.position);
							}
							else if(del2->data.attribute!=term::Int&&del2->data.attribute!=term::Float){
								return errorInfo(11,del2->data.position);
							}
							return errorInfo(11);
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
							if(del->data.attribute!=term::Int){
								return errorInfo(9,del->data.position);
							}
							else if(del2->data.attribute!=term::Int){
								return errorInfo(9,del2->data.position);
							}
							return errorInfo(9);
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
			if(del2!=NULL){
				delete del2;
			}
			return errorInfo(-1);
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
		queue<termNode*> q[4];
	};
	struct expression{
		termList expr;
		OprQueue opr;
		void pushNode(term* tmp,int order=0){
			expr.push(*tmp);
			if(tmp->category!=term::None){
				opr.q[order].push(expr.tail);
			}
		}
		void push(int val,int pos){
			term* tmp=new term(val,pos,term::Int,term::None);
			pushNode(tmp);
		}
		void push(double val,int pos){
			term* tmp=new term(val,pos,term::Float,term::None);
			pushNode(tmp);
		}
		void push(double(*p)(double,double),int pos,term::oprType opr_type=term::RR,int order=0){
			term* tmp=new term(p,pos,term::biDouble,opr_type);
			pushNode(tmp,order);
		}
		void push(double(*p)(double),int pos,term::oprType opr_type=term::R,int order=0){
			term* tmp=new term(p,pos,term::uniDouble,opr_type);
			pushNode(tmp,order);
		}
		void push(int(*p)(int,int),int pos,term::oprType opr_type=term::RR,int order=0){
			term* tmp=new term(p,pos,term::biInt,opr_type);
			pushNode(tmp,order);
		}
		void push(int(*p)(int),int pos,term::oprType opr_type=term::R,int order=0){
			term* tmp=new term(p,pos,term::uniInt,opr_type);
			pushNode(tmp,order);
		}
		void push(char ch,int pos){
			term* tmp;
			switch(ch){
				case '+':
					push(Add,pos,term::LR,3);
					break;
				case '-':
					push(Minus,pos,term::LR,3);
					break;
				case '*':
					push(Multiply,pos,term::LR,2);
					break;
				case '/':
					push(Divide,pos,term::LR,2);
					break;
				case '^':
					push(pow,pos,term::LR,1);
					break;
				case '!':
					push(Factorial,pos,term::L);
					break;
				case '%':
					push(Mod,pos,term::LR,2);
					break;
				default:cout<<"Error: Unidentified operator."<<endl;
			}
		}
		errorInfo calculate(){
			errorInfo err(-1);
			for(int i=0;i<4;i++){
				while(!opr.q[i].empty()){
					err=expr.mergeAdjacent(opr.q[i].front());
					opr.q[i].pop();
					if(err.errCode!=-1)return err;
				}
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
		result(){}
		result(double val,bool err=true):value(val),error(err){}
		void set(double val,bool err){
			value=val;
			error=err;
		}
	};	
	struct function{
		char tag[nameLength];
		term opr;
		void set(char* funcName,double(*p)(double,double)){
			tagCopy(tag,funcName);
			opr.attribute=term::biDouble;
			opr.category=term::RR;
			opr.val.ptr1=p;
		}
		void set(char* funcName,double(*p)(double)){
			tagCopy(tag,funcName);
			opr.attribute=term::uniDouble;
			opr.category=term::R;
			opr.val.ptr2=p;
		}
		void set(char* funcName,int(*p)(int,int)){
			tagCopy(tag,funcName);
			opr.attribute=term::biInt;
			opr.category=term::RR;
			opr.val.ptr3=p;
		}
		void set(char* funcName,int(*p)(int)){
			tagCopy(tag,funcName);
			opr.attribute=term::uniInt;
			opr.category=term::R;
			opr.val.ptr4=p;
		}
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
	void setErr(int code,bool isErr=true){
		errCode=code;
		err=isErr;
	}
	void readNumber(char* ch,int& i,expression& e,bool omit=false){
		int ival=0;
		int orig;
		if(!omit){
			orig=i;
			ival=ch[i]-'0';
			while(isNumber(ch[++i])){
				ival=ival*10+ch[i]-'0';
			}
			if(ch[i]!='.'){
				if(ch[i]=='\0'){
					e.push(ival,orig);
				}
				else if(isOperator(ch[i])){
					e.push(ival,orig);
				}
				else if(isAlphabet(ch[i])){
					e.push(ival,orig);
					e.push('*',i);
				}
				else if(ch[i]==','){
					setErr(0);
				}
				else if(ch[i]==')'){
					setErr(1);
				}
				else if(ch[i]=='('){
					e.push(ival,orig);
					e.push('*',i);
				}
				else{
					setErr(2);
				}
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
			if(isAlphabet(ch[i])){
				e.push('*',i);
			}
			else if(ch[i]==','){
				setErr(0);
			}
			else if(ch[i]=='('){
				e.push('*',i);
			}
			else if(ch[i]==')'){
				setErr(1);
			}
			else if(ch[i]=='.'){
				setErr(3);
			}
			else if(ch[i]!='\0' && !isOperator(ch[i])){
				setErr(2);
			}
		}
		else{
			setErr(4);
		}
	}
	int readFunction(char* ch,int& i,expression& e,int offset){
		int orig=i;
		term func=match(ch,i);
		if(func.val.ptr1==Fail){
			i=func.position;
			return -1;
		}
		return readParameter(ch,i,e,&func,orig,offset);
	}
	void examineOpr(char* ch,int& i,expression& e,int Case){
		if(isNumber(ch[i])||isAlphabet(ch[i])||ch[i]=='.'||ch[i]=='('){
			if(Case==1){
				e.push(0,i-1);
			}
			if(Case==5){
				e.push('!',i-2);
			}
			if(Case==1 || Case==2 || Case==4 || Case==5){
				e.push(ch[i-1],i-1);
			}
			if(Case==4){
				e.push('*',i-1);
			}
		}
		else if(ch[i]=='\0'){
			if(Case==4){
				e.push('!',i-1);
			}
			else{
				setErr(12);		
			}
		}
		else{
			if(Case==4 && isOperator(ch[i])){
				if(ch[i]=='!'){
					setErr(7);
				}
				else{
					++i;
					examineOpr(ch,i,e,5);
				}
			}
			else{
				setErr(6);
			}
		}
	}
	void readOperator(char* ch,int& i,expression& e){
		if(e.expr.tail==NULL && ch[i]!='+' && ch[i]!='-'){
			setErr(5);
		}
		else{
			++i;
			if(e.expr.tail==NULL && ch[i-1]=='-'){
				examineOpr(ch,i,e,1);
			}
			else if(e.expr.tail!=NULL && ch[i-1]!='!'){
				examineOpr(ch,i,e,2);
			}
			else if(e.expr.tail==NULL && ch[i-1]=='+'){
				examineOpr(ch,i,e,3);
			}
			else{
				examineOpr(ch,i,e,4);
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
		if(node!=NULL && !isAlphabet(ch[i])&&node->func.tag[j]=='\0'){
			switch(node->func.opr.attribute){
				case term::biDouble:
					return term(node->func.opr.val.ptr1,orig,node->func.opr.attribute,node->func.opr.category);
				case term::uniDouble:
					return term(node->func.opr.val.ptr2,orig,node->func.opr.attribute,node->func.opr.category);
				case term::biInt:
					return term(node->func.opr.val.ptr3,orig,node->func.opr.attribute,node->func.opr.category);
				case term::uniInt:
					return term(node->func.opr.val.ptr4,orig,node->func.opr.attribute,node->func.opr.category);
				default:
					cout<<"Exception";
					setErr(13);
			}
		}
		else{
			setErr(13);
		}
		return term(Fail,orig,term::biDouble,term::RR);
	}
	int readParameter(char* ch,int& i,expression& e,term* t,int begin,int offset){
		if(ch[i]=='\0'){
			setErr(14);
			return -1;
		}
		else if(ch[i]!='('){
			setErr(15);
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
						setErr(19);
						return -1;
					}
					if(lbr==rbr&&ch[i]==','){
						++paraCnt;
						if(paraCnt>paraNum){
							setErr(17);
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
				setErr(16);
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
				setErr(18);
				return -1;
			}		
		}
	}
public:
	calculator(){
		functionTable[0].set("abs",fabs);
		functionTable[1].set("acos",acos);
		functionTable[2].set("asin",asin);
		functionTable[3].set("atan",atan);
		functionTable[4].set("cos",cos);
		functionTable[5].set("exp",exp);
		functionTable[6].set("ln",log);
		functionTable[7].set("log",log10);
		functionTable[8].set("pow",pow);
		functionTable[9].set("sin",sin);
		functionTable[10].set("sqrt",sqrt);
		functionTable[11].set("tan",tan);
		fTree=new matchTree(functionTable,12);
	}
	~calculator(){
		delete fTree;
	}
	result calculate(char* ch,int offset=0){
		setErr(-1,false);
		if(ch[0]=='\0'){
			return result(0,false);
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
					return result(0);
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
						setErr(1);
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
						return result(0,true);
					}
				}
			}
			else{
				setErr(2);
			}
			if(err){
				cout<<"Position:"<<offset+i+1<<" Error Code:"<<errCode<<endl<<errMsg[errCode];
				return result(0,true);
			}
		}
		errorInfo eInfo=e->calculate();
		if(eInfo.errCode!=-1){
			cout<<"Position:"<<offset+eInfo.position+1<<" Error Code:"<<eInfo.errCode<<endl<<errMsg[eInfo.errCode];
			return result(0,true);
		}
		if(offset==0){
			cout<<"=";
			e->print();
		}
		result tmp;
		if(e->expr.front->data.attribute==term::Float){
			tmp.set(e->expr.front->data.val.dval,false);
		}
		else if(e->expr.front->data.attribute==term::Int){
			tmp.set(e->expr.front->data.val.ival,false);
		}
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