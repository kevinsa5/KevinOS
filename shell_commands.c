
struct File *readFile(char*);

#include "shell_commands.h"
#include "kernel.h"
#include "sh_exec.c"
#include "util.h"
#include "sh_lisp.c"
#include "buildData.h"
#include "driver.h"
#include "editor.h"

char *shCommandList[] = {"shell", "read", "exec", "edit", "ls", "head","cat", "textdump", "memdump", "help", "glissando", "mario","sleep", "beep", "htoi", "itoh", "int", "rand", "colortable", "chartable", "hextable", "datatypes", "sti", "cli", "builddata", "memalloc", "memfree", "poke", "history", "memusg","millis","echo","foo","msg","null"};
void (*shFunctionList[])(char*) = {sh_shell, sh_read, sh_exec, sh_edit, sh_ls, sh_head, sh_cat, sh_textDump, sh_memDump, sh_help, sh_glissando, sh_mario, sh_sleep, sh_beep, sh_htoi, sh_itoh, sh_int, sh_rand, sh_colorTable, sh_charTable, sh_hexTable, sh_dataTypes, sh_sti, sh_cli, sh_buildData, sh_memalloc, sh_memfree, sh_poke, sh_history, sh_memusg,sh_millis, sh_echo, sh_foo, sh_msg, sh_null};

void sh_foo(char* command){
	struct expr * testexpr;
	testexpr = makeIntExpr((int) millis());
	ttprintln(toString(testexpr));
	testexpr = makeFloatExpr(3.14159265358979);
	ttprintln(toString(testexpr));
	testexpr = makeStringExpr("Hello, world!");
	ttprintln(toString(testexpr));
	testexpr = makeBoolExpr(1);
	ttprintln(toString(testexpr));
	testexpr = makeBoolExpr(0);
	ttprintln(toString(testexpr));
	/*
	struct StringListNode* tokens = tokenize(fileBuffer->firstLine);
	ttprintln("printing tokens:");
	while(tokens){
		ttprintln(tokens->str);
		tokens = tokens->next;
	}*/
}

void sh_handler(char* command){
	int i=0;
	while(command[i]!=' ' && command[i]!=0) i++;
	if(i == 0) return;
	char program[i+1];
	memCopy(command,program,i);
	program[i] = 0;
	char params[strLen(command)-i];
	memCopy(command+i+1,params,strLen(command)-i+1);
	params[strLen(command)-i-1] = 0;
	i = 0;
	while(!strEquals(shCommandList[i],program) && !strEquals(shCommandList[i],"null")) i++;
	if(!strEquals(shCommandList[i],"null")) (*shFunctionList[i])(params);
	else {
		ttprint("Command not found: ");
		ttprintln(program);
	}
}

void sh_echo(char* params){
	ttprintln(params);
}

void sh_msg(char* params){
	setMessage(params);
}


// run sh_handler on each line of a file (or the buffer)
void sh_shell(char* params){
	struct StringListNode *temp;
	if(strEquals(params,"buffer")){
		temp = fileBuffer->firstLine;
	} else {
		struct File *f = readFile(params);
		temp = f->firstLine;
	}
	while(temp){
		int len = strLen(temp->str)-1;
		if((temp->str)[len-1] == '\n'){ // lines ends with \n
			char* line = (char*) malloc(len);
			memCopy(temp->str, line, len);
			line[len-1] = 0;
			sh_handler(line);
			free(line, len);
		} else { // line doesn't end with \n (last line in file)
			sh_handler(temp->str);
		}
		temp = temp->next;
	}
}

void sh_edit(char* params){
	terminalMode = EDITOR;
	editor_initialize();
	editor_updateScreen();
}

void sh_ls(char* params){
	int i;
	//char firstTime = 1;
    char* pointer = KFS;
    char* KFS_sig = "KFS Begin";
    for(i=0; i<strLen(KFS_sig)-1; i++){
            if(pointer[i] != KFS_sig[i]){
                    ttprint("Error! File system does not start with:");
                    ttprintln(KFS_sig);
                    return;
            }
    }
    pointer += 25;
    // 25 is filesystem header, 94 is max # files
    for(i=0; i < 94; i++){
		if(pointer[0] == 0) continue;
	    int c;
		for(c = 0; c < 10 && pointer[c] != 0; c++){
			ttprintChar(pointer[c]);
		}
        pointer += 16;
        ttprint("\n");
    }
}
void sh_head(char* params){
	if(strLen(params) == 1){
		ttprintln("Must pass a filename.");
		return;
	}
	int lines = 10;
	struct File *f = readFile(params);
	struct StringListNode* line = f->firstLine;
	while(line && lines > 0){
		ttprint(line->str);
		line = line->next;
		lines--;
	}
}


void sh_read(char* params){
	fileBuffer = readFile(params);
}

void sh_cat(char* params){
	struct StringListNode *temp;
	if(strEquals(params,"buffer")){
		temp = fileBuffer->firstLine;
	} else {
		struct File *f = readFile(params);
		temp = f->firstLine;
	}
	while(temp){
		ttprint(temp->str);
		temp = temp->next;
	}
}

void clearBuffer(){
	struct StringListNode *temp = fileBuffer->firstLine;
	struct StringListNode *next;
	while(temp){
		free(temp->str, strLen(temp->str));
		next = temp->next;
		free(temp, sizeof(struct StringListNode));
		temp = next;
	}
	free(fileBuffer->filename, strLen(fileBuffer->filename));
	fileBuffer->filename = 0;
	fileBuffer->filesize = 0;	
}

/*
	parse a file into a linked list of lines separated by \n
*/

struct File *readFile(char* params){
	char** pointer;
	int len = getFilePointer(params,pointer);
	struct File *file;
	file = (struct File*)malloc(sizeof(struct File));
	file->filename = (char*)malloc(strLen(params));
	memCopy(params,file->filename,strLen(params));
	file->filesize = len;
	int i;
	struct StringListNode *line = (struct StringListNode*) malloc(sizeof(struct StringListNode));
	file->firstLine = line;
	line->prev = 0;
	line->next = 0;
	int lineStart = 0;
	// split the file into separate lines
	for(i = 0; i < len; i++){
		if((*pointer)[i] == '\n' || i == len-1){
			line->str = (char*) malloc(i-lineStart+2);
			memCopy((*pointer+lineStart), line->str, i-lineStart+1);
			(line->str)[i-lineStart+1] = 0;
			//set up the next line
			struct StringListNode *tline;
			tline = (struct StringListNode*) malloc(sizeof(struct StringListNode));
			tline->prev = line;
			line->next = tline;
			line = tline;
			lineStart = i+1;
		}
		if(i == len-1){
			//nix the next line, since there isn't one
			line = line->prev;
			free(line->next, sizeof(struct StringListNode));
			line->next = 0;
		}
	}
	return file;
}

/* 
* stores pointer to file in param2, returns filesize
*/

int getFilePointer(char* params, char** pointerParam){

	int i;
	*pointerParam = KFS;
	unsigned char** pointer = (unsigned char**) pointerParam;
	char* KFS_sig = "KFS Begin";
	for(i=0; i<strLen(KFS_sig)-1; i++){
		if((*pointer)[i] != KFS_sig[i]){
			ttprint("Error! KFS does not start with:");
			ttprint(KFS_sig);
			return -1;
		}
	}

	*pointer += 25;
	// 25 is filesystem header, 94 is max # files
	for(i=0; i < 94; i++){
		//ttprintln(*pointer);
		if(strEquals(params,*pointer) || strLen(params) == 11 && strBeginsWith(*pointer,params)) break;
		*pointer += 16;
	}
	if(i == 94){
		ttprint("File not found:");
		ttprint(params);
		return -2;
	}
	i = 0;
	*pointer += 10;
	int sec,off,len;
	sec = (*pointer)[i++];

	off = (*pointer)[i++] << 8 ;
	off |= (*pointer)[i++];
	
	len = (*pointer)[i++] << 16;
	len |= ((*pointer)[i++] << 8);
	len |= (*pointer)[i++];

	*pointer = KFS + (sec+3)*512 + off;
	return len;
}

void sh_glissando(char* params){
	int i = 220;
	//xxxx 50
	char param[8];
	while(i < 220*8){
		intToString(i,param);
		if(i < 1000){
			param[3] = ' ';
			param[4] = '5';
			param[5] = '0';
			param[6] = 0;
		} else {
			param[4] = ' ';
			param[5] = '5';
			param[6] = '0';
			param[7] = 0;
		}
		sh_beep(param);
		i = i + 20;
	}
	sh_beep("0 0");
}

void sh_mario(char* params){
	sh_beep("659 100");
	sleep(100);
	sh_beep("659 100");
	sleep(150);
	sh_beep("659 100");
	sleep(200);
	sh_beep("523 100");
	sleep(75);
	sh_beep("659 100");
	sleep(200);
	sh_beep("784 100");
	sleep(500);
	sh_beep("392 100");
}
void sh_sleep(char* params){
	sleep(strToInt(params));
}
void sh_beep(char* params){
	//"beep freq duration"
	int i,freq = 440, dur = 1000;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	char frequency[i+1];
	memCopy(params,frequency,i);
	frequency[i] = 0;
	freq = strToInt(frequency);
	if(i != strLen(params)){
		char duration[strLen(params)-i];
		memCopy(params+i+1,duration,strLen(params)-i);
		dur = strToInt(duration);
	}
	play_sound(freq);
	sleep(dur);
	play_sound(0);
}
void sh_htoi(char* params){
	int len = strLen(params)-1;
	if(len < 3 || params[0] != '0' || params[1] != 'x'){
		ttprintln("htoi param must start with `0x`");
		return;
	}
	reverseInPlace(params+2);
	len -= 2;
	int i;
	int sum = 0;
	for(i = 0; i < len; i++){
		if(isAlpha(params[i+2])){
			if(isLower(params[i+2]) && params[i+2] <= 'f'){
				sum += pow(16,i) * (params[i+2]-'a'+10);
			}
			else if(isUpper(params[i+2]) && params[i+2] <= 'F'){
				sum += pow(16,i) * (params[i+2]-'A'+10);
			}
			else {
				ttprintln("improperly formed hex");
				return;
			}
		}
		else sum += pow(16,i) * (params[i+2]-'0');
	}
	ttprintIntln(sum);
}
void sh_itoh(char* params){
	char s[3];
	s[2] = 0;
	itoh(strToInt(params),s);
	ttprintln(s);
}
void sh_int(char* params){
	ttprintIntln(strToInt(params));
}

void sh_textDump(char* params){
	// textDump pointer length
	int i;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	char substr[i+1];
	memCopy(params,substr,i);
	substr[i] = 0;
	char* pointer = (char*) ((int*)strToInt(substr));
	char substr2[strLen(params)-i];
	memCopy(params+i+1,substr2,strLen(params)-i);
	int length = strToInt(substr2);
	for(i=0;i<length;i++){
		ttprintChar(pointer[i]);
	}
	ttprint("\n");
}

void sh_memDump(char* params){
	// textDump pointer length
	int i;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	char substr[i+1];
	memCopy(params,substr,i);
	substr[i] = 0;
	char* pointer = (char*) ((int*)strToInt(substr));
	char substr2[strLen(params)-i];
	memCopy(params+i+1,substr2,strLen(params)-i);
	int length = strToInt(substr2);
	char hex[3];
	hex[2] = 0;
	for(i=0;i<length;i++){
		itoh(pointer[i], hex);
		ttprint(hex);
		ttprint(" ");
	}
	ttprint("\n");
}

void sh_rand(char* params){
	int i;
	for(i = 0; i < strLen(params)-1; i++){
		if(params[i] < '0' || params[i] > '9'){
			ttprint("parameter must be numeric:");
			ttprintChar(params[i]);
			return;
		}
	}
	if(strLen(params) == 1) ttprintIntln(rand(10));
	else ttprintIntln(rand(strToInt(params)));
	return;
}
void sh_colorTable(char* params){
	char a;
	ttprint("high byte:");
	for(a = 0; a < 16; a++){
		ttprintCharColor('+',a*16 + 15);
	}
	ttprintChar('\n');
	ttprint("low byte: ");
	for(a = 0; a < 16; a++){
		ttprintCharColor('+',15*16+a);
	}
	ttprint("\n");
}
void sh_charTable(char* params){
	char i;
	for(i = -128; i < 127; i++){
		ttprintChar(i);
	}
	ttprint("\n");
}
void sh_hexTable(char* params){
	char str[5];
	int a,b;
	for(a = 0; a < 16; a++){
		for(b = 0; b < 16; b++){
			charToString(a*16+b,str);
			ttprint(str);
			ttprint(" ");
		}
	}
}
void sh_dataTypes(char* params){
	ttprint("char:"); ttprintIntln(sizeof(char));
	ttprint("short:"); ttprintIntln(sizeof(short));
	ttprint("int:"); ttprintIntln(sizeof(int));
	ttprint("long:"); ttprintIntln(sizeof(long));
}
void sh_sti(char* params){
	enableInterrupts();
}
void sh_cli(char* params){
	disableInterrupts();
}
void sh_buildData(char* params){
	ttprint("Build ID:");
	ttprintIntln(getBuildID());
}
void sh_help(char* params){
	int i = 0;
	ttprint("Available commands are:");
	while(!strEquals(shCommandList[i],"null")){
		ttprint(" ");
		ttprint(shCommandList[i]);
		i++;
	}
	ttprint("\n");
}
void sh_memalloc(char* params){
	char* ptr = (char*) malloc(strToInt(params));
	ttprintIntln((int)ptr);
}
void sh_memfree(char* params){
	int i;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	params[i] = 0;
	void* pointer = (void*) ((int*)strToInt(params));
	int value = strToInt(params+i+1);
	free(pointer, value);
}
void sh_poke(char* params){
	// poke pointer byte(decimal)
	int i;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	char substr[i+1];
	memCopy(params,substr,i);
	substr[i] = 0;
	char* pointer = (char*) ((int*)strToInt(substr));
	
	char substr2[strLen(params)-i];
	memCopy(params+i+1,substr2,strLen(params)-i);
	int value = strToInt(substr2);
	*pointer = value;
}

void sh_history(char* params){
	struct StringListNode *conductor = historyHead;
	while(conductor->next){
		ttprintln(conductor->str);
		conductor = conductor->next;
	}
}
void sh_memusg(char* params){
	int bytes = getBytesAllocated();
	int avail = getAvailableMemory();
	ttprint("Allocated ");
	ttprintInt(bytes);
	ttprint(" of ");
	ttprintInt(avail);
	ttprintln(" bytes");
}
void sh_millis(char* params){
	ttprintIntln(millis());
}
void sh_null(char* params){
	ttprintln("wtf");
}
