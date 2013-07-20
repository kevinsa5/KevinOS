void sh_readKFS(char*);
void sh_glissando(char*);
void sh_mario(char*);
void sh_delay(char*);
void sh_beep(char*);
void sh_sti(char*);
void sh_cli(char*);
void sh_htoi(char*);
void sh_int(char*);
void sh_charTable(char*);
void sh_hexDump(char*);
void sh_help(char*);
void sh_rand(char*);
void sh_hexTable(char*);
void sh_null(char*);

char *shCommandList[] = {"readKFS", "glissando", "mario", "delay", "beep", "sti", "cli", "htoi", "int", "hexDump", "help","rand","charTable", "hexTable","null"};
void (*shFunctionList[])(char*) = {sh_readKFS, sh_glissando, sh_mario, sh_delay, sh_beep, sh_sti, sh_cli, sh_htoi, sh_int, sh_hexDump, sh_help, sh_rand, sh_charTable, sh_hexTable, sh_null};

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
void sh_readKFS(char* params){
	// readKFS offset length
	int i;
	for(i=0;i<strLen(params);i++){
		if(params[i] == ' ') break;
	}
	char offset[i+1];
	memCopy(params,offset,i);
	offset[i] = 0;
	int iOffset = strToInt(offset);
	
	char length[strLen(params)-i];
	memCopy(params+i+1,length,strLen(params)-i);
	int iLength = strToInt(length);
	
	char* pointer = KFS;
	pointer += iOffset;
	
	for(i=0;i<iLength;i++){
		ttprintChar(pointer[i]);
	}
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
	sh_beep("660 100");
	delay(150);
	sh_beep("660 100");
	delay(300);
	sh_beep("660 100");
	delay(300);
	sh_beep("510 100");
	delay(100);
	sh_beep("660 100");
	delay(300);
	sh_beep("770 100");
	delay(550);
	sh_beep("380 100");
	delay(575);
	int i;
	for(i = 0; i < 2; i++){
		sh_beep("510 100");
		delay(450);
		sh_beep("380 100");
		delay(400);
		sh_beep("320 100");
		delay(500);
		sh_beep("440 100");
		delay(300);
		sh_beep("480 80");
		delay(330);
		sh_beep("450 100");
		delay(150);
		sh_beep("430 100");
		delay(300);
		sh_beep("380 100");
		delay(200);
		sh_beep("660 80");
		delay(200);
		sh_beep("760 50");
		delay(150);
		sh_beep("860 100");
		delay(300);
		sh_beep("700 80");
		delay(150);
		sh_beep("760 50");
		delay(350);
		sh_beep("660 80");
		delay(300);
		sh_beep("520 80");
		delay(150);
		sh_beep("580 80");
		delay(150);
		sh_beep("480 80");
		delay(500);
	}
	
	sh_beep("500 100");
	delay(300);
	
	sh_beep("760 100");
	delay(100);
	sh_beep("720 100");
	delay(150);
	sh_beep("680 100");
	delay(150);
	sh_beep("620 150");
	delay(300);
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
	/*
	char hex[5];
	for(i = 0; i < length; i++){
		charToString(pointer[i],hex);
		ttprint(hex);
		ttprintChar(' ');
	}
	*/
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
void sh_sti(char* params){
	enableInterrupts();
}
void sh_cli(char* params){
	disableInterrupts();
}
void sh_null(char* params){
	ttprintln("null function was called-- wtf?");
}
