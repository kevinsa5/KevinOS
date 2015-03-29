#include "util.h"
#include "malloc.h"

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

char* toString(struct expr *e){
	if(atomic(e)){
		ttprintln("here");
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
			str[1] = *((int*) e->data) ? 't' : 'f';
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

