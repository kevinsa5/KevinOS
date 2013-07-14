#define width 80
#define height 23
#define absolute_height 24
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define SHIFT 0
#define CTRL 1
#define ALT 2
#define INSERT 3
#define CAPSLOCK 4

void print(int, int, char *);
void printChar(int, int, char, int);
char getChar(int, int);
int getColor(int, int);
void ttprintChar(char);
void ttprint(char *);
void ttprintInt(int);
void ttprintln(char *);
void ttprintIntln(int);
int offset(int, int);
void setCursor(int, int);
void clearScreen(int);
void scrollUp();
void cursorBackwards();
void cursorForwards();
void keyPressed(unsigned char);
void pitCall();
void printStatus(unsigned char);

#include "driver.c"
#include "util.c"
#include "IDT.c"
#include "shell_commands.c"

char* vidmem;
int ttx;
int tty;
char prompt;
int modifier[6];
unsigned long int ticks;
int commandLength;

void main(){
	vidmem = (char*) 0xb8000;
	ttx = 0;
	tty = 0;
	ticks = 0;
	setSeed(5);
	modifier[5] = 0;
	int i = 0;
	commandLength = 0;
	while(modifier[i] != 0) modifier[i] = 0;
	modifier[INSERT] = 1;
	prompt = '$';
	clearScreen(0x0F);
	ttprintln("I am a computer! I am running Kevin's OS!"); 
	ttprint("Preparing Interrupts...");
	idt_init();
	setTimerFreq(100);
	ttprintln(" done");
	ttprintChar(3);
	ttprintln("");
	ttprintln("Prompt is ready, dickhead:");
	ttprintChar(prompt);
	printStatus(0x00);
}
//left: E0 4B E0 CB
//right: E0 4D E0 CD
//up: E0 48 E0 C8
//down: E0 50 E0 D0
void keyPressed(unsigned char code){
	char c = scancodeToAscii(code, modifier[SHIFT], modifier[CTRL], modifier[CAPSLOCK]);
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
		int i = offset(ttx,tty);		//find beginning of command (ie the prompt)
		while(vidmem[i] != prompt){
			i= i-2;
		}
		i+=2;
		char command[(offset(ttx,tty)-i+2)/2];
		command[(offset(ttx,tty)-i)/2] = 0;
		int j;
		for(j = 0; j < (offset(ttx,tty)-i)/2; j++) command[j] = vidmem[i+2*j];
		ttprintChar('\n');
		commandLength = strLen(command);
		
		sh_handler(command);
		
		ttprintChar(c);
		ttprintChar(prompt);
	}
}
void pitCall(){
	ticks++;
	if(ticks % 100 == 0) printStatus(-1);
}
void printStatus(unsigned char code){
	int i;
	for(i = 0; i < width; i++){
		printChar(i,height,'-',0x0F);
		printChar(i,absolute_height,' ',0x0F);
	}
	int statusX = 0;
	print(statusX, absolute_height, "KeyCode:");
	statusX += 8;
	char str[10];
	charToString(code,str);
	if(code != -1) print(statusX, absolute_height, str);
	statusX += 4;
	print(statusX, absolute_height, " Insert:");
	statusX += 8;
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
	print(statusX, absolute_height, " Last Entry:");
	statusX += 12;
	intToString(commandLength,str);
	print(statusX, absolute_height, str);
	statusX += strLen(str-1);
	print(statusX, absolute_height, " Time:");
	statusX += 6;
	intToString(ticks/100,str);
	print(statusX, absolute_height,str);
}
void ttprint(char *string){
	int i;
	for(i = 0; string[i] != 0; i++){
		ttprintChar(string[i]);
	}
}
void ttprintInt(int n){
	char str[n/10+1];
	intToString(n,str);
	ttprint(str);
}
void ttprintIntln(int n){
	ttprintInt(n);
	ttprintChar('\n');
}
void ttprintChar(char c){
	if(c == 0) return;
	if(ttx >= width){
		ttx = 0;
		tty++;
	}
	if(c == '\n'){
		ttx = 0;
		tty++;
		c = 0;
	}
	if(tty >= height){
		scrollUp();
	}
	if(c != 0){
		printChar(ttx,tty,c,0x0F);
		ttx++;
	}
	setCursor(ttx,tty);
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
	if(getChar(ttx,tty) == prompt){
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

