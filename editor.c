
void editor_keyPressed(unsigned char code, char c){
	char printable = 1;
	if(code == 0x0E){ // backspace
		if(ttx == 0 && tty == 0) return;
		cursorBackwards();
		int i = 0;
		while(getChar(ttx + i, tty) != 0){
			printChar(ttx+i,tty,getChar(ttx+i+1,tty),0x0F);
			i++;
		}
		printable = 0;
	} else if(code == 0x4B){ // left arrow
		ttx--;
		if(ttx == -1){
			ttx = 0;
			tty--;
			if(tty == -1) tty = 0;
		}
		setCursor(ttx, tty);
		printable = 0;
	} else if(code == 0x4D){ // right arrow
		ttx++;
		if(ttx == width){
			ttx = width-1;
			tty++;
			if(tty == height) tty = height-1;
		}
		setCursor(ttx, tty);
		printable = 0;
	} else if(code == 0x48){ // up arrow
		tty--;
		if(tty == -1) tty = 0;
		setCursor(ttx, tty);
		printable = 0;
	} else if(code == 0x50){ // down arrow
		tty++;
		if(tty == height) tty = height-1;
		setCursor(ttx, tty);
		printable = 0;
	} else if(code == 0xCB){ // left arrow release
		printable = 0;
	} else if(code == 0xCD){ // right arrow release
		printable = 0;
	} else if(code == 0xC8){ // up arrow release
		printable = 0;
	} else if(code == 0xCB){ // down arrow release
		printable = 0;
	} else if(code == 0x1D){ // control on
		modifier[CTRL] = 1;
		printable = 0;
	} else if(code == 0x9D){ //control off
		modifier[CTRL] = 0;
		printable = 0;
	} else if(code == 0x36 || code == 0x2A){ //shift on
		modifier[SHIFT] = 1;
		printable = 0;
	} else if(code == 0xB6 || code == 0xAA){ //shift off
		modifier[SHIFT] = 0;
		printable = 0;
	} else if(code == 0x38){ // alt on
		modifier[ALT] = 1;
		printable = 0;
	} else if(code == 0xB8){ // alt off
		modifier[ALT] = 0;
		printable = 0;
	} else if(code == 0x52){ // insert
		modifier[INSERT]++;
		modifier[INSERT] %= 2;
		printable = 0;
	} else if(code == 0x3A){ // capslock
		modifier[CAPSLOCK]++;
		modifier[CAPSLOCK] %= 2;
		printable = 0;
	}
	printStatus(code);

	if(modifier[CTRL] && c == 's'){
		int x = 0;
		int y = 0;
		for(y = 0; y < height; y++){
			for(x = 0; x < width; x++){
				fileBuffer[y*width + x] = getChar(x,y);
			}
		}
		printable = 0;
		clearScreen(0x00);
		terminalMode = TERMINAL;
		ttx = 0; tty = 0;
		setCursor(ttx,tty);
		printPrompt();
		printStatus(0x00);
		return;
	}

	if(c == 0) return;
	if(modifier[INSERT] && printable == 1){
		int i = 0;
		while(getChar(ttx + i, tty) != 0){
			i++;
		}
		while(i > 0){
			printChar(ttx + i,tty,getChar(ttx+i-1,tty),0x0F);
			i--;
		}
	}
	ttprintChar(c);
}
