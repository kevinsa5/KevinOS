void sh_help(char*);
void sh_rand(char*);
void sh_hexTable(char*);
void sh_null(char*);

char *shCommandList[] = {"help","rand","hexTable","null"};
void (*shFunctionList[])(char*) = {sh_help, sh_rand, sh_hexTable, sh_null};

void sh_handler(char* command){
	int i=0;
	while(command[i]!=' ' && command[i]!=0) i++;
	char program[i+1];
	memCopy(command,program,i);
	program[i] = 0;
	char params[strLen(command)-i];
	memCopy(command+i+1,params,strLen(command)-i+1);
	params[strLen(command)-i-1] = 0;
	/*ttprint("program:`");
	ttprint(program);
	ttprintln("`");
	ttprint("params: `");
	ttprint(params);
	ttprintln("`");
	ttprint("param length:");
	ttprintIntln(strLen(params));*/
	i = 0;
	while(!strEquals(shCommandList[i],program) && !strEquals(shCommandList[i],"null")) i++;
	if(!strEquals(shCommandList[i],"null")) (*shFunctionList[i])(params);
	else {
		ttprint("Function not found: ");
		ttprintln(program);
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
void sh_null(char* params){
	ttprintln("null function was called-- wtf?");
}
