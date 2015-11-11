#include "util.h"
#include "malloc.h"
#include "kernel.h"

#define T_EXPR 0
#define T_INT 1
#define T_FLOAT 2
#define T_STRING 3
#define T_BOOL 4

struct ExprListNode
{
    struct expr *expr;
    struct ExprListNode *next;
    struct ExprListNode *prev;
};


struct expr
{
    int type;
    void* data;
};

int atomic(struct expr *e){
	return e->type == T_INT ||
		   e->type == T_FLOAT ||
		   e->type == T_STRING ||
		   e->type == T_BOOL;
}

struct expr* makeIntExpr(int i){
	struct expr* ex = (struct expr*) malloc(sizeof(struct expr));
	ex->type = T_INT;
	int *p = (int*)malloc(sizeof(int));
	*p = i;
	ex->data = p;
	return ex;
}
struct expr* makeFloatExpr(double d){
	struct expr* ex = (struct expr*) malloc(sizeof(struct expr));
	ex->type = T_FLOAT;
	double *p = (double*)malloc(sizeof(double));
	*p = d;
	ex->data = p;
	return ex;
}
struct expr* makeStringExpr(char *str){
	struct expr* ex = (struct expr*) malloc(sizeof(struct expr));
	ex->type = T_STRING;
	char *p = (char*)malloc(strLen(str));
	memCopy(str,p,strLen(str));
	ex->data = p;
	return ex;
}
struct expr* makeBoolExpr(char b){
	struct expr* ex = (struct expr*) malloc(sizeof(struct expr));
	ex->type = T_BOOL;
	char *p = (char*)malloc(1);
	*p = b;
	ex->data = p;
	return ex;
}

void lispError(char *message){
	ttprintln("-------------------------");
	ttprintln(message);
	ttprintln("-------------------------");
}

char* toString(struct expr *e){
	if(atomic(e)){
		if(e->type == T_INT){
			int i = (int) *((int*)e->data);
			char *str = (char*)malloc(numDigits(i)+1);
			intToString(i, str);
			str[numDigits(i)] = 0;
			return str;
		} else if(e->type == T_FLOAT){
			// print 5 decimal points
			double d = (double) *((double*)e->data);
			int intlen = numDigits((int)d);
			char *str = (char*)malloc(intlen + 5 + 1 + 1);
			intToString((int)d, str);
			str[intlen] = '.';
			int i;
			double rem = d - (int)d;
			for(i = intlen+1; i < intlen+6; i++){
				rem *= 10;
				str[i] = '0' + (int)rem;
				rem = rem - (int)rem;
			}
			str[i] = 0;
			return str;
		} else if(e->type == T_STRING){
			char* from = (char*) e->data;
			int len = strLen(from);
			char* to   = (char*) malloc(len);
			memCopy(from,to,len);
			return to;
		} else if(e->type == T_BOOL){
			char* str = malloc(3);
			str[0] = '#';
			str[1] = *((char*) e->data) ? 't' : 'f';
			str[2] = 0;
			return str;
		} else {
			char msg[] = "unknown type:";
			int len = strLen(msg);
			int typelen = numDigits(e->type);
			char *str = (char*) malloc(len + typelen);
			memCopy(msg,str,len-1);
			char *temp = (char*) malloc(typelen+1);
			intToString(e->type, temp);
			memCopy(temp, str+len-1, typelen);
			free(temp, typelen + 1);
			return str;
		}
	} else {
		if(e->type == T_EXPR){
			struct StringListNode *listHead = (struct StringListNode*) malloc(sizeof(struct StringListNode));
			struct StringListNode *iter = listHead;
			struct ExprListNode * n = (struct ExprListNode*) e->data;
			int totalLen = 0;
			
			while(n){
				char* tempString = toString(n->expr);
				totalLen += strLen(tempString) - 1;
				iter->str = tempString;
				
				if(n->next){
					struct StringListNode* next = (struct StringListNode*) malloc(sizeof(struct StringListNode));
					iter->next = next;
					next->prev = iter;
					iter = next;
				}
				n = n->next;
			}
			
			char* str = (char*) malloc(totalLen+1);
			iter = listHead;
			int i = 0; 
			while(iter){
				int len = strLen(iter->str);
				memCopy(iter->str, str+i, len-1);
				//free(iter->str, strLen(iter->str));
				iter = iter->next;
				i += len;
			}
			str[totalLen] = 0;
			return str;
		} else {
			char msg[] = "unknown type:";
			int len = strLen(msg);
			int typelen = numDigits(e->type);
			char *str = (char*) malloc(len + typelen);
			memCopy(msg,str,len-1);
			char *temp = (char*) malloc(typelen+1);
			intToString(e->type, temp);
			memCopy(temp, str+len-1, typelen);
			free(temp, typelen + 1);
			return str;
		}
	}
}

struct StringListNode* tokenize(struct StringListNode* list){
	char** ptr;
	int totalLength = flattenStringList(list, ptr);
	char* str = *ptr;
	if(*str != '('){
		lispError("Improper format:  code must begin with `(`");
		return (struct StringListNode*) 0;
	}
	char insideString = 0;
	int numLeft = 1, numRight = 0, i = 1;
	struct StringListNode *tokenHead = (struct StringListNode*)malloc(sizeof(struct StringListNode));
	tokenHead->prev = 0;
	tokenHead->next = 0;
	
	tokenHead->str = (char*) malloc(2);
	*(tokenHead->str) = '(';
	*(tokenHead->str+1) = 0;
	struct StringListNode *tokenTail = tokenHead;

	while(i < totalLength){
		sleep(100);
		ttprint("current char:");
		ttprintChar(str[i]);
		ttprintChar('\n');
		if(str[i] == '('){
			ttprintln("found (");
			// necessary or else it crashes:
			ttprintln("hello");
			for(;0 == 1;);
			ttprintln("here");
			// very strange.
			numLeft++;
			struct StringListNode *temp = (struct StringListNode*)malloc(sizeof(struct StringListNode));
			temp->str = (char*) malloc(2);
			*(temp->str) = '(';
			*(temp->str+1) = 0;
			temp->prev = tokenTail;
			temp->next = 0;
			tokenTail->next = temp;
			tokenTail = temp;
		} else if(str[i] == ')'){
			ttprintln("found )");
			numRight++;
			struct StringListNode *temp = (struct StringListNode*)malloc(sizeof(struct StringListNode));
			temp->str = (char*) malloc(2);
			*(temp->str) = ')';
			*(temp->str+1) = 0;
			temp->prev = tokenTail;
			temp->next = 0;
			tokenTail->next = temp;
			tokenTail = temp;
		} else if(str[i] == ' '){
			ttprintln("found space");
		} else {
			ttprintln("found otherrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrawefwaefwaefwaefawfewaferrrrrrrrrrrrrrrr");
			ttprintln("current other char:");
			ttprintChar(str[i]);
			ttprintChar('\n');
			int start = i;
			while(str[i] != ' ' && str[i] != '(' && str[i] != ')'){
				i++; 
				ttprint("current other char:");
				ttprintChar(str[i]);
				ttprintChar('\n');
				if(i >= totalLength) {
					lispError("Improper format:  code must end with `)`");
					return (struct StringListNode*) 0;
				}
			}
			i--;
			int len = i - start + 1;
			struct StringListNode *temp = (struct StringListNode*)malloc(sizeof(struct StringListNode));
			temp->str = (char*)malloc(len+1);
			memCopy(str+start, temp->str, len);
			(temp->str)[len] = 0;
			temp->prev = tokenTail;
			temp->next = 0;
			tokenTail->next = temp;
			tokenTail = temp;
		}
		ttprint("tokenTail:  ");
		ttprintln(tokenTail->str);
		i++;
	}
	if(numRight != numRight){
		lispError("Improper format:  open and close parenthesis counts must match");
		return (struct StringListNode*) 0;
	}
	return tokenHead;
}
