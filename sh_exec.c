/*
#include "stdio.h"
#include "util.c"

#define width 80
#define height 18
*/

int iregisters[10];
char cregisters[10];
int cmp1, cmp2;

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

char *execCommandList[] =           {    "seti",    "setc",    "printi",    "printc",    "addi",    "multi",    "randi",    "inc",    "dec",    "setcmp1",  "setcmp2",     "status"   ,  ""};
void (*execFunctionList[])(char*) = {exec_seti, exec_setc, exec_printi, exec_printc, exec_addi, exec_multi, exec_randi, exec_inc, exec_dec, exec_setcmp1, exec_setcmp2, exec_status, exec_null};

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
	int lineNum;
	for(lineNum = 0; lineNum < height; lineNum++){
		// read line, removing null chars along the way:
		int i = 0;
		index = lineNum * width;
		// lines must not have the first char blank
		// also don't read comments
		if(fileBuffer[index] == ' ' || fileBuffer[index] == 0 || fileBuffer[index] == '#')
			continue;
		memFill(line,0,width);
		while(fileBuffer[index] != ';' && i < 80){
			if(fileBuffer[index] != 0){
				line[i] = fileBuffer[index];
				i++; 
			}
			index++;
		}
		if(strEquals(line,"")) continue;
		// now `line` contains the command (function + args) to be interpreted
		// isolate out the function part by splitting on the first space
		int j = 0;
		while(line[j] != ' ' && j < width) j++;
		char function[j+1];
		char params[width-j];
		memCopy(line, function, j);
		function[j] = 0;
		memCopy((line + j+1), params, (width-j));
		// macros:
		if(strEquals(function, "jeq")){
			if(cmp1 == cmp2) lineNum = strToInt(params) - 1;
			continue;
		}
		if(strEquals(function, "jne")){
			if(cmp1 != cmp2) lineNum = strToInt(params) - 1;
			continue;
		}
		if(strEquals(function, "jge")){
			if(cmp1 >= cmp2) lineNum = strToInt(params) - 1;
			continue;
		}
		if(strEquals(function, "jle")){
			if(cmp1 <= cmp2) lineNum = strToInt(params) - 1;
		   	continue;
		}
		if(strEquals(function, "jlt")){
			if(cmp1 < cmp2) lineNum = strToInt(params) - 1;
			continue;
		}
		if(strEquals(function, "jgt")){
			if(cmp1 > cmp2) lineNum = strToInt(params) - 1;
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
	}
	terminalMode = TERMINAL;
	return;
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
	iregisters[index] = rand(limit);
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

