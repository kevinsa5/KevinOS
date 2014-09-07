
void sh_readKFS(char*);

void sh_hexDump(char*);
void sh_help(char*);
void sh_null(char*);
void sh_cat(char*);
void sh_head(char*);
void sh_ls(char*);
void sh_edit(char*);
void sh_read(char*);

#include "sh_exec.c"


char *shCommandList[] = {"read", "exec", "edit", "ls", "head","cat", "hexDump", "help","null"};
void (*shFunctionList[])(char*) = {sh_read, sh_exec, sh_edit, sh_ls, sh_head, sh_cat, sh_hexDump, sh_help, sh_null};

void sh_handler(char* command){
	int i=0;
	while(command[i]!=' ' && command[i]!=0) i++;
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
                int c;
		for(c = 0; c < 10; c++){
			ttprintChar(pointer[c]);
		}
		ttprintln("");
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
int getFilePointer(char* params, char** pointer){

	int i;
	*pointer = KFS;

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
void sh_help(char* params){
	int i = 0;
	ttprint("Available commands are:");
	while(!strEquals(shCommandList[i],"null")){
		ttprint(" ");
		ttprint(shCommandList[i]);
		i++;
	}
}
void sh_null(char* params){
	ttprintln("wtf");
}
