if(strEquals(command,"help")){
	ttprintln("rand");
	ttprintln("hex table");
}
if(strEquals(command,"rand") || strBeginsWith(command,"rand ")){
	int n;
	if(strLen(command) > 5){
		n = rand(strToInt((command+5)));
	} else {
		n = rand(10);
	}
	ttprintIntln(n);
}
if(strEquals(command,"hex table")){
	ttprintln("");
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
if(command[0] >= '0' && command[0] <= '9'){
	ttprintIntln(strToInt(command));
}

