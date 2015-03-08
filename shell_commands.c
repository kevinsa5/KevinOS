void sh_readKFS(char*);
void sh_hexDump(char*);
void sh_help(char*);
void sh_null(char*);
void sh_cat(char*);
void sh_head(char*);
void sh_ls(char*);
void sh_edit(char*);
void sh_read(char*);
void sh_glissando(char*);
void sh_mario(char*);
void sh_delay(char*);
void sh_beep(char*);
void sh_htoi(char*);
void sh_int(char*);
void sh_rand(char*);
void sh_colorTable(char*);
void sh_charTable(char*);
void sh_hexTable(char*);
void sh_dataTypes(char*);
void sh_sti(char*);
void sh_cli(char*);
void sh_buildData(char*);
void sh_memalloc(char*);
void sh_poke(char*);

#include "sh_exec.c"
#include "buildData.c"

char *shCommandList[] = {"read", "exec", "edit", "ls", "head","cat", "hexDump", "help", "glissando", "mario","delay", "beep", "htoi", "int", "rand", "colorTable", "charTable", "hexTable", "dataTypes", "sti", "cli", "buildData", "memalloc", "poke", "null"};
void (*shFunctionList[])(char*) = {sh_read, sh_exec, sh_edit, sh_ls, sh_head, sh_cat, sh_hexDump, sh_help, sh_glissando, sh_mario, sh_delay, sh_beep, sh_htoi, sh_int, sh_rand, sh_colorTable, sh_charTable, sh_hexTable, sh_dataTypes, sh_sti, sh_cli, sh_buildData, sh_memalloc, sh_poke, sh_null};

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


void sh_edit(char* params){
	terminalMode = EDITOR;
	clearScreen(0x0F);
	int x = 0;
	int y = 0;
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			printChar(x,y,fileBuffer[y*width + x],0x0F);
		}
	}
	setCursor(0,0);
}

void sh_ls(char* params){
	int i;
	char firstTime = 1;
    char* pointer = KFS;
    char* KFS_sig = "KFS Begin";
    for(i=0; i<strLen(KFS_sig)-1; i++){
            if(pointer[i] != KFS_sig[i]){
                    ttprint("Error! File system does not start with:");
                    ttprint(KFS_sig);
                    return;
            }
    }
    pointer += 25;
    // 25 is filesystem header, 94 is max # files
    for(i=0; i < 94; i++){
		if(pointer[0] == 0) continue;
		if(!firstTime){
			ttprintln("\n");
		} else {
			firstTime = 0;
		}
	    int c;
		for(c = 0; c < 10 && pointer[c] != 0; c++){
			ttprintChar(pointer[c]);
		}
        pointer += 16;
    }
}
void sh_head(char* params){
	if(strLen(params) == 1){
		ttprintln("Must pass a filename.");
		return;
	}
	int i,numLines = 0;
	char** pointer;
	int len = getFilePointer(params, pointer);
	for(i = 0; i < len && numLines < 10; i++){
		ttprintChar((*pointer)[i]);
		if((*pointer)[i] == '\n') numLines++;
	}
}

void sh_read(char* params){
	int i;
	char** pointer;
	int len = getFilePointer(params,pointer);
	
	if(len > width*height){
		ttprintln("err: file too big");
		terminalMode = TERMINAL;
		return;
	}

	// for some reason, a loop is necessary or else it crashes.
	// WTF.
	for(i = 0; i < 0;i++){}

	memFill(fileBuffer, 0, width*height);
	int bufferIndex = 0;
	int fileIndex = 0;
	while(fileIndex < len){
		if((*pointer)[fileIndex] != '\n'){
			fileBuffer[bufferIndex] = (*pointer)[fileIndex];
			bufferIndex++;
		} else {
			fileBuffer[bufferIndex] = '\n';
			while(bufferIndex % width != 0) bufferIndex++;
		}
		fileIndex++;
	}
}

void sh_cat(char* params){
	int i;
	char** pointer;
	if(strEquals(params,"buf")){
		for(i = 0; i < width*height; i++)
			ttprintChar(fileBuffer[i]);
	} else {
		int len = getFilePointer(params,pointer);
		for(i = 0; i < len; i++){
			ttprintChar((*pointer)[i]);
		}
	}
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
		return;
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
	int i;
	sh_beep("659 100");
	delay(100);
	sh_beep("659 100");
	delay(150);
	sh_beep("659 100");
	delay(200);
	sh_beep("523 100");
	delay(75);
	sh_beep("659 100");
	delay(200);
	sh_beep("784 100");
	delay(500);
	sh_beep("392 100");
}
void sh_delay(char* params){
	delay(strToInt(params));
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
	delay(dur);
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
	ttprintInt(sum);
}
void sh_int(char* params){
	ttprintIntln(strToInt(params));
}

void sh_hexDump(char* params){
	// hexDump pointer length
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
	if(strLen(params) == 1) ttprintInt(rand(10));
	else ttprintInt(rand(strToInt(params)));
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
}
void sh_charTable(char* params){
	char i;
	for(i = -128; i < 127; i++){
		ttprintChar(i);
	}
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
}
void sh_memalloc(char* params){
	char* ptr = (char*) malloc(strToInt(params));
	ttprintIntln((int)ptr);
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
void sh_null(char* params){
	ttprintln("wtf");
}
