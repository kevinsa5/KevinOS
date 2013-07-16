void sh_sti();
void sh_cli();
void sh_htoi(char*);
void sh_int(char*);
void sh_charTable(char*);
void sh_hexDump(char*);
void sh_help(char*);
void sh_rand(char*);
void sh_hexTable(char*);
void sh_null(char*);

char *shCommandList[] = {"sti", "cli", "htoi", "int", "hexDump", "help","rand","charTable", "hexTable","null"};
void (*shFunctionList[])(char*) = {sh_sti, sh_cli, sh_htoi, sh_int, sh_hexDump, sh_help, sh_rand, sh_charTable, sh_hexTable, sh_null};

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
		ttprint("Function not found: ");
		ttprintln(program);
	}
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
	char* pointer = (char*) strToInt(substr);
	char substr2[strLen(params)-i];
	memCopy(params+i+1,substr2,strLen(params)-i);
	int length = strToInt(substr2);
	char hex[5];
	for(i = 0; i < length; i++){
		charToString(pointer[i],hex);
		ttprint(hex);
		ttprintChar(' ');
	}
}
void sh_help(char* params){
	if(strLen(params) > 1){
		ttprintIntln(strLen(params));
		ttprintln("help does not accept parameters");
	}
	int i = 0;
	ttprint("Available commands are:");
	while(!strEquals(shCommandList[i],"null")){
		ttprint(" ");
		ttprint(shCommandList[i]);
		i++;
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
void sh_charTable(char* params){
	char i;
	for(i = 0; i < 127; i++){
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
void sh_sti(){
	enableInterrupts();
}
void sh_cli(){
	disableInterrupts();
}
void sh_null(char* params){
	ttprintln("null function was called-- wtf?");
}
