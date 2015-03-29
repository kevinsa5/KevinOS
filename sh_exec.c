#include "util.h"
#include "kernel.h"
#include "malloc.h"

int iregisters[10];
char cregisters[10];
int cmp1, cmp2;

void exec_sleep(char*);
void exec_seti(char*);
void exec_setc(char*);
void exec_printi(char*);
void exec_printc(char*);
void exec_addi(char*);
void exec_multi(char*);
void exec_null(char*);
void exec_randi(char*);
void exec_inc(char*);
void exec_dec(char*);
void exec_setcmp1(char*);
void exec_setcmp2(char*);
void exec_status(char*);
struct StringListNode * exec_findLine(int);

char *execCommandList[] =           {    "sleep", "seti",    "setc",    "printi",    "printc",    "addi",    "multi",    "randi",    "inc",    "dec",    "setcmp1",  "setcmp2",     "status"   ,  ""};
void (*execFunctionList[])(char*) = {exec_sleep, exec_seti, exec_setc, exec_printi, exec_printc, exec_addi, exec_multi, exec_randi, exec_inc, exec_dec, exec_setcmp1, exec_setcmp2, exec_status, exec_null};

void sh_exec(char* unused_params){
	terminalMode = INTERPRETER;
	char line[width+1];
	line[width] = 0;
	int index = 0;
	for(index = 0; index < 10; index++){
		iregisters[index] = 0;
		cregisters[index] = 0;
	}
	//iterate through the buffer
	struct StringListNode *execLine = fileBuffer->firstLine;
	while(execLine){
		if((execLine->str)[0] == 0 || (execLine->str)[0] == '#'){
			execLine = execLine->next;
			continue;
		}
		int len = strLen(execLine->str);
		int j = 0;
		while(j < len && (execLine->str)[j] != ' ' && (execLine->str)[j] != ';') j++;
		char *function = (char*) malloc(j+1);
		char *params = (char*) malloc(len-j);
		memCopy(execLine->str, function, j);
		function[j] = 0;
		int k = j;
		while(k < len && (execLine->str)[k] != ';') k++;
		memCopy(execLine->str + j + 1, params, k-j-1);

		if(strEquals(function, "jeq")){
			if(cmp1 == cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
			continue;
		}
		if(strEquals(function, "jne")){
			if(cmp1 != cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
			continue;
		}
		if(strEquals(function, "jge")){
			if(cmp1 >= cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
			continue;
		}
		if(strEquals(function, "jle")){
			if(cmp1 <= cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
		   	continue;
		}
		if(strEquals(function, "jlt")){
			if(cmp1 < cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
			continue;
		}
		if(strEquals(function, "jgt")){
			if(cmp1 > cmp2) execLine = exec_findLine(strToInt(params) - 1);
			else execLine = execLine->next;
			continue;
		}
		j = 0;
		while(!strEquals(execCommandList[j],function) && !strEquals(execCommandList[j],"")) j++;
		if(!strEquals(execCommandList[j],"")) (*execFunctionList[j])(params);
		else {
			ttprint("err: no such function: `");
			ttprint(function);
			ttprint("` of length:");
			ttprintIntln(strLen(function));
			break;
		}
		execLine = execLine->next;
	}
	terminalMode = TERMINAL;
	return;
}

struct StringListNode* exec_findLine(int lineNum){
	struct StringListNode *iter = fileBuffer->firstLine;
	while(lineNum-- > 1 && iter) iter = iter->next;
	return iter;
}

// `sleep milliseconds`
void exec_sleep(char* params){
	sleep(strToInt(params));
}

// `seti registerIndex value`
void exec_seti(char* params){
	int index = params[0] - '0';
	iregisters[index] = strToInt( (params + 2) );
}

// `setc registerIndex value`
void exec_setc(char* params){
	int index = params[0] - '0';
	cregisters[index] = params[2];
}

// `printi registerIndex`
void exec_printi(char* params){
	//printf("%d\n", iregisters[params[0] - '0']);
	ttprintIntln(iregisters[params[0] - '0']);
}

// `printc registerIndex`
void exec_printc(char* params){
	//printf("%c\n", cregisters[params[0] - '0']);
	ttprintChar(cregisters[params[0] - '0']);
	ttprintChar('\n');
}

// `addi operand operand destination
void exec_addi(char* params){
	char a = params[0] - '0';
	char b = params[2] - '0';
	char c = params[4] - '0';
	iregisters[c] = iregisters[a] + iregisters[b];
}

// `multi operand operand destination
void exec_multi(char* params){
	char a = params[0] - '0';
	char b = params[2] - '0';
	char c = params[4] - '0';
	iregisters[c] = iregisters[a] * iregisters[b];
}

// `randi limit destination
void exec_randi(char* params){
	int i;
	int limit = 10;
	char index = 0; 
	for(i = 0; i < strLen(params); i++){
		if(params[i] == ' '){
			params[i] = 0;
			limit = strToInt(params);
			params[i] = ' ';
			index = params[i+1] - '0';
			break;
		}
	}
	iregisters[index] = (int)rand(limit);
}

void exec_inc(char* params){
	iregisters[params[0] - '0'] += 1;
}

void exec_dec(char* params){
	iregisters[params[0] - '0'] -= 1;
}

void exec_setcmp1(char* params){
	cmp1 = iregisters[params[0] - '0'];
}
void exec_setcmp2(char* params){
	cmp2 = iregisters[params[0] - '0'];
}

void exec_status(char* params){
	int i;
	ttprint("ireg:");
	for(i = 0; i < 10; i++){
		ttprintInt(iregisters[i]);
		ttprintChar(',');
	}
	ttprintln("");
	ttprint("creg:");
	for(i = 0; i < 10; i++){
		ttprintChar(cregisters[i]);
		ttprintChar(',');
	}
	ttprintln("");
	ttprint("cmp:");
	ttprintInt(cmp1);
	ttprintChar(',');
	ttprintInt(cmp2);
	ttprintln("");
}


void exec_null(char * params){
	
}

