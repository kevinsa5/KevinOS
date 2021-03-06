extern char _binary_KFS_bin_start[];
char* vidmem;
char prompt;
int promptColor;

volatile unsigned long int ticks;
char counter;

#include "kernel.h"
#include "malloc.h"
#include "driver.h"
#include "util.h"
#include "IDT.c"
#include "shell_commands.h"
#include "editor.h"
#include "sh_exec.h"
#include "buildData.h"

char modifier[6];

int ttx;
int tty;
int terminalMode;
char* KFS;
struct File *fileBuffer;
struct StringListNode *historyHead;
struct StringListNode *historyTemp;

char* message;

void main(){
	vidmem = (char*) 0xb8000;
	clearAllocationTable();
	
	historyHead = (struct StringListNode *) malloc(sizeof(struct StringListNode));
	
	historyHead->prev = 0;
	historyHead->next = 0;
	historyHead->str = (char*) malloc(2);
	historyHead->str[0] = 0;
	historyTemp = historyHead;
	KFS = _binary_KFS_bin_start;
	ttx = 0;
	tty = 0;
	ticks = 0;
	setSeed(5);
	modifier[5] = 0;
	message = (char*) malloc(3);
	memCopy("OK",message,3);
	int i = 0;

	counter = '0';
	memFill(modifier, 0, 5);
	modifier[INSERT] = 1;
	modifier[CAPSLOCK] = 0;
	terminalMode = TERMINAL;
	prompt = 21;
	promptColor = 0x02;
	clearScreen(0x0F);
	//ttprintln("I am a computer! I am running Kevin's OS!"); 
	ttprint("KevinOS build ");
	ttprintIntln(getBuildID());
	ttprint("Setting up interrupt descriptor table...");
	idt_init();
	setTimerFreq(1000);
	ttprintln("done");
	
	//turn on bit 6 of register B:
	disableInterrupts();
	writeByteToPort(0x70,0x8B);
	char prev=readByteFromPort(0x71);
	writeByteToPort(0x70,0x8B);
	writeByteToPort(0x71, prev | 0x40);	
	enableInterrupts();

	sh_shell("startup.sh");
	
	printPrompt();
	printStatus(0x00);
}
//left: E0 4B E0 CB
//right: E0 4D E0 CD
//up: E0 48 E0 C8
//down: E0 50 E0 D0

void printNode(struct StringListNode *node){
	ttprint("node:");
	ttprintInt((int)node);
	ttprint(" prev:");
	ttprintInt((int)node->prev);
	ttprint(" next:");
	ttprintInt((int)node->next);
	ttprint(" str:");
	ttprintln(node->str);
}

void keyPressed(unsigned char code){
	char c = scancodeToAscii(code, modifier[SHIFT], modifier[CTRL], modifier[CAPSLOCK]);
	if(terminalMode == TERMINAL){
		terminal_keyPressed(code, c);
	} else if(terminalMode == EDITOR){
		editor_keyPressed(code, c);
	} else if(terminalMode == INTERPRETER){
		interpreter_keyPressed(code, c);
	}
	if(code == 0x01) setMessage("ESC");
}

void terminal_keyPressed(unsigned char code, char c){
	if(code == 0x0E){ // backspace
		if(getChar(ttx-1,tty) == prompt) return;
		cursorBackwards();
		int i = 0;
		while(getChar(ttx + i, tty) != 0){
			printChar(ttx+i,tty,getChar(ttx+i+1,tty),0x0F);
			i++;
		}
	} else if(code == 0x4B){ // left arrow
		cursorBackwards();
	} else if(code == 0x4D){ // right arrow
		cursorForwards();
	} else if(code == 0x48){ // up arrow
		while(getChar(ttx-1,tty) != prompt){
			ttprintChar(' ');
			cursorBackwards();
			cursorBackwards();
        }
		ttprint(historyTemp->str);
		if(historyTemp->next != 0)
			historyTemp = historyTemp->next;
	} else if(code == 0x50){ // down arrow
		while(getChar(ttx-1,tty) != prompt){
			ttprintChar(' ');
			cursorBackwards();
			cursorBackwards();
        }
		ttprint(historyTemp->str);
		if(historyTemp->prev != 0)
			historyTemp = historyTemp->prev;
	} else if(code == 0x1D){ // control on
		modifier[CTRL] = 1;
	} else if(code == 0x9D){ //control off
		modifier[CTRL] = 0;
	} else if(code == 0x36 || code == 0x2A){ //shift on
		modifier[SHIFT] = 1;
	} else if(code == 0xB6 || code == 0xAA){ //shift off
		modifier[SHIFT] = 0;
	} else if(code == 0x38){ // alt on
		modifier[ALT] = 1;
	} else if(code == 0xB8){ // alt off
		modifier[ALT] = 0;
	} else if(code == 0x52){ // insert
		modifier[INSERT]++;
		modifier[INSERT] %= 2;
	} else if(code == 0x3A){ // capslock
		modifier[CAPSLOCK]++;
		modifier[CAPSLOCK] %= 2;
	}
	printStatus(code);
	if(c == 0) return;
	if(c != '\n'){
		if(modifier[INSERT]){
			int i = 0;
			while(getChar(ttx + i, tty) != 0){
				i++;
			}
			while(i > 0){
				printChar(ttx+i,tty,getChar(ttx+i-1,tty),0x0F);
				i--;
			}
		}
		ttprintChar(c);
	} else {
		int i = offset(ttx,tty); //find beginning of command (ie the prompt)
		while(vidmem[i] != prompt && vidmem[i+1] != promptColor){
			i= i-2;
		}
		i+=2;
		char command[(offset(ttx,tty)-i+2)/2];
		command[(offset(ttx,tty)-i)/2] = 0;
		int j;
		for(j = 0; j < (offset(ttx,tty)-i)/2; j++) command[j] = vidmem[i+2*j];
		ttprintChar('\n');

		struct StringListNode *new = (struct StringListNode *)malloc(sizeof(struct StringListNode));
		
		new->str = (char*) malloc(strLen(command));
		memCopy(command,new->str, strLen(command));
		new->next = historyHead;
		new->prev = 0;

		historyHead->prev = new;
		historyHead = new;
		historyTemp = historyHead;

		sh_handler(command);
		if(terminalMode == TERMINAL){
			//ttprintChar(c);
			printPrompt();
		}
	}
}

void setMessage(char *new){
	free(message,strLen(message));
	int len = strLen(new);
	message = (char*) malloc(len);
	memCopy(new,message,len);
}
void printPrompt(){
	ttprintCharColor(prompt,promptColor);
}
void sleep(int mS){
	enableInterrupts();
	unsigned long int start = ticks;
	while(ticks - start < mS){;}
	//ttprint("done");
}
void rtcCall(){
	counter++;
	if(counter > '9') counter = '1';
	printChar(width-1,absolute_height,counter,0x0F);
}
void pitCall(){
	ticks++;
	if(ticks % PITfreq == 0) printStatus(0);
}
unsigned long int millis(){
	return ticks;
}
void printStatus(unsigned char code){
	int i;
	for(i = 0; i < width; i++){
		printChar(i,height,'-',0x0F);
		printChar(i,absolute_height,' ',0x0F);
	}
	int statusX = 0;
	char str[10];
	
	print(statusX, absolute_height, message);
	statusX += strLen(message);
	
	print(statusX, absolute_height, "Code:");
	statusX += 5;
	charToString(code,str);
	if(code != -1) print(statusX, absolute_height, str);
	statusX += 4;

	print(statusX, absolute_height, " Ins:");
	statusX += 5;
	printChar(statusX, absolute_height, modifier[INSERT]+'0', 0x0F);
	statusX += 1;

	print(statusX, absolute_height, " Shift:");
	statusX += 7;
	printChar(statusX, absolute_height, modifier[SHIFT]+'0', 0x0F);
	statusX += 1;

	print(statusX, absolute_height, " Caps:");
	statusX += 6;
	printChar(statusX, absolute_height, modifier[CAPSLOCK]+'0', 0x0F);
	statusX += 1;

	print(statusX, absolute_height, " Ctrl:");
	statusX += 6;
	printChar(statusX, absolute_height, modifier[CTRL]+'0', 0x0F);
	statusX += 1;

	print(statusX, absolute_height, " Alt:");
	statusX += 5;
	printChar(statusX, absolute_height, modifier[ALT]+'0', 0x0F);
	statusX += 1;

	//print(statusX, absolute_height, " Last:");
	//statusX += 6;
	//intToString(commandLength,str);
	//print(statusX, absolute_height, str);
	//statusX += strLen(str)-1;
	
	print(statusX, absolute_height, " Mode:");
	statusX += 6;
	printChar(statusX, absolute_height, terminalMode+'0', 0x0F);
	statusX += 1;	

	//print(statusX, absolute_height, " ttx:");
	//statusX += 5;
	//intToString(ttx, str);
	//print(statusX, absolute_height, str);
	//statusX += strLen(str)-1;
	
	print(statusX, absolute_height, " mem:");
	statusX += 5;
	doubleToString(heapUsage(), str);
	print(statusX, absolute_height, str);
	statusX += strLen(str)-1;
	
	print(statusX, absolute_height, "%");
	statusX++;
	
	print(statusX, absolute_height, " Time:");
	statusX += 6;
	intToString(ticks/PITfreq,str);
	print(statusX, absolute_height,str);
	statusX += strLen(str)-1;
}
void ttprint(char *string){
	int i;
	for(i = 0; string[i] != 0; i++){
		ttprintChar(string[i]);
	}
}
void ttprintInt(int n){
	int len = numDigits(n) + 1;
	if(n < 0) len++; // minus sign
	char *str = (char*) malloc(len);
	intToString(n,str);
	ttprint(str);
	free(str,len);
}
void ttprintIntln(int n){
	ttprintInt(n);
	ttprintChar('\n');
}
void ttprintCharColor(char c, int col){
	if(c == 0) return;
	int printable = 1;
	if(ttx >= width){
		ttx = 0;
		tty++;
	}
	if(c == '\n'){
		ttx = 0;
		tty++;
		c = 0;
	}
	if(c == '\t'){
		int i;
		for(i = 0; i < 4; i++) ttprintChar(' ');
		printable = 0;	
	}
	if(tty >= height){
		scrollUp();
	}
	if(c != 0 && printable){
		printChar(ttx,tty,c,col);
		ttx++;
	}
	setCursor(ttx,tty);
}
void ttprintChar(char c){
	ttprintCharColor(c,0x0F);
}
void cursorForwards(){
	if(getChar(ttx,tty) == 0) return;
	ttx++;
	if(ttx >= width){
		ttx = 0;
		tty++;
	}
	if(tty >= height){
		scrollUp();
	}
	setCursor(ttx,tty);
}
void cursorBackwards(){
	ttx--;
	if(ttx < 0){
		ttx = width-1;
		tty--;
	}
	setCursor(ttx,tty);
	if(getChar(ttx,tty) == prompt && getColor(ttx,tty) == promptColor){
		ttx++;
		setCursor(ttx,tty);
	}
}
	
void scrollUp(){
	int j;
	for(j = 0; j < height-1; j++){
		memCopy(vidmem+(j+1)*(2*width),vidmem+j*(2*width),2*width);
	}
	for(j = 0; j < 2*width; j++){
		printChar(j,height-1,0,0x0F);
	}
	tty--;
}
void ttprintln(char *string){
	ttprint(string);
	ttprintChar('\n');
}
void print(int x, int y, char *string){
	int i = 0;
	while(string[i] != 0) printChar(x++,y,string[i++],0x0f);
}
void printChar(int x, int y, char c, int col){
	vidmem[offset(x,y)] = c;
	vidmem[offset(x,y)+1] = col;
}
char getChar(int x, int y){
	return vidmem[offset(x,y)];
}
int getColor(int x, int y){
	return vidmem[offset(x,y)+1];
}
void setCursor(int x, int y){
	char high, low;
	low = (char)((x+y*width) % 256);
	high = (char)((x+y*width) / 256);
	writeByteToPort(REG_SCREEN_CTRL, 14);
	writeByteToPort(REG_SCREEN_DATA, high);
	writeByteToPort(REG_SCREEN_CTRL, 15);
	writeByteToPort(REG_SCREEN_DATA, low);
	ttx = x;
	tty = y;
}
int offset(int x, int y){
	return 2*(y*width+x);
}
void clearScreen(int color){
	int i;
	int j;
	for(i = 0; i < width; i++){
		for(j = 0; j < height; j++){
			printChar(i,j,0,color);
		}
	}
}

void disableInterrupts(){
	asm volatile("cli");
}
void enableInterrupts(){
	asm volatile("sti; nop");
}
