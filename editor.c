void editor_up();
void editor_down();

#include "editor.h"
#include "kernel.h"
#include "util.h"
#include "malloc.h"

// first line displayed in the editor window
struct StringListNode *highestLine;
// line currently being edited
struct StringListNode *activeLine;

void editor_initialize(){
	clearScreen(0x0F);
	highestLine = fileBuffer->firstLine;
	activeLine = fileBuffer->firstLine;
	setCursor(0,0);
}

void editor_updateScreen(){
	clearScreen(0x7F);
	int pushTTX = ttx, pushTTY = tty;
	ttx = 0;
	tty = 0;
	struct StringListNode *temp = highestLine;
	int i;
	for(i = 0; i < height -1 && temp; i++){
		ttprint(temp->str);
		temp = temp->next;
	}
	setCursor(pushTTX,pushTTY);
}

void editor_left(){
	ttx--;
	if(ttx < 0){
		if(activeLine->prev){
			ttx = strLen((activeLine->prev)->str)-2;
			editor_up();
		} else {
			ttx++;
		}
	} else {
		setCursor(ttx,tty);
		editor_updateScreen();
	}
}

void editor_right(){
	ttx++;
	if(ttx > strLen(activeLine->str)-2){
		if(activeLine->next){
			ttx = 0;
			editor_down();
		} else {
			ttx--;
		}
	} else {
		setCursor(ttx,tty);
		editor_updateScreen();
	}
}

void editor_up(){
	if(activeLine->prev){
		activeLine = activeLine->prev;
		if(tty > 0){
			tty--;
			int len = strLen(activeLine->str);
			if(ttx > len-2)
				ttx = len-2;
			setCursor(ttx,tty);
		} else {
			highestLine = highestLine->prev;
			editor_updateScreen();
		}
	}
}

void editor_down(){
	if(activeLine->next){
		activeLine = activeLine->next;
		if(tty < height-2){
			tty++;
			int len = strLen(activeLine->str);
			if(ttx > len-2)
				ttx = len-2;
			setCursor(ttx,tty);
		} else {
			highestLine = highestLine->next;
			editor_updateScreen();
		}
	}
}

void editor_backspace(){
	if(ttx < 0){
		setMessage("-1");
	} else if(ttx == 0){
		if(activeLine->prev){
			char* previous = activeLine->prev->str;
			int prevLength = strLen(previous);
			char* current  = activeLine->str; 
			int currLength = strLen(current); 
			// -2 to kill the newline and null
			char* combined = (char*) malloc(prevLength + currLength - 2);
			memCopy(previous, combined, prevLength-2);
			memCopy(current, combined + prevLength-2, currLength);
			struct StringListNode *next = activeLine->next;
			activeLine = activeLine->prev;
			free(current, currLength);
			free(activeLine->next, sizeof(struct StringListNode));
			free(previous, prevLength);
			activeLine->next = next;
			activeLine->str = combined;

			next->prev = activeLine;
			ttx = prevLength-2;
			tty--;
			setCursor(ttx,tty);
			editor_updateScreen();
		}
	} else {
		ttx--;
		int len = strLen(activeLine->str);
		char *new = (char*)malloc(len-1);
		memCopy(activeLine->str,new,ttx);
		memCopy(activeLine->str + ttx+1, new+ttx,(len-ttx-1));
		free(activeLine->str,len);
		activeLine->str = new;

		setCursor(ttx,tty);
		editor_updateScreen();
	}
}

void editor_keyPressed(unsigned char code, char c){
	char printable = 1;
	if(code == 0x0E){ // backspace
		editor_backspace();
		printable = 0;
	} else if(code == 0x4B){ // left arrow
		editor_left();
		printable = 0;
	} else if(code == 0x4D){ // right arrow
		editor_right();
		printable = 0;
	} else if(code == 0x48){ // up arrow
		editor_up();
		printable = 0;
	} else if(code == 0x50){ // down arrow
		editor_down();
		printable = 0;
	} else if(code == 0xCB){ // left arrow release
		printable = 0;
	} else if(code == 0xCD){ // right arrow release
		printable = 0;
	} else if(code == 0xC8){ // up arrow release
		printable = 0;
	} else if(code == 0xD0){ // down arrow release
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
		clearScreen(0x00);
		terminalMode = TERMINAL;
		ttx = 0; tty = 0;
		setCursor(ttx,tty);
		printPrompt();
		printStatus(0x00);
		return;
	}
	
	if(c == 0 || !printable) return;
	char str[2];
	str[0] = c;
	str[1] = 0;
	setMessage(str);
	
	if(c == '\n'){
		int lineLength = strLen(activeLine->str);
		char* new1 = (char*) malloc(ttx+2);
		char* new2 = (char*) malloc(lineLength-ttx);
		memCopy(activeLine->str,new1,ttx);
		new1[ttx] = '\n';
		new1[ttx+1] = 0;
		memCopy(activeLine->str+ttx,new2,lineLength-ttx);
		struct StringListNode *newline = (struct StringListNode*) malloc(sizeof(struct StringListNode));
		newline->prev = activeLine;
		newline->next = activeLine->next;
		newline->next->prev = newline;
		activeLine->next = newline;
		free(activeLine->str, lineLength);
		activeLine->str = new1;
		newline->str = new2;
		activeLine = newline;
		ttx = 0;
		tty++;
		setCursor(ttx,tty);
		editor_updateScreen();
		return;
	}

	
	if(modifier[INSERT]){
		// split the line in twain, reassemble.
		int len = strLen(activeLine->str);
		char *new = (char*)malloc(len+1);
		memCopy(activeLine->str,new,ttx);
		new[ttx] = c;
		memCopy(activeLine->str + ttx, new+ttx+1,(len-ttx));
		free(activeLine->str,len);
		activeLine->str = new;
	} else {
		(activeLine->str)[ttx] = c;
	}
	editor_right();
	editor_updateScreen();
}
