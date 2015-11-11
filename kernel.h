#ifndef KERNEL_H
#define KERNEL_H

//terminal modes:
#define TERMINAL 0
#define EDITOR 1
#define INTERPRETER 2

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
extern char modifier[6];

extern int ttx;
extern int tty;

extern int terminalMode;
extern char* KFS;
extern struct File *fileBuffer;

extern struct StringListNode *historyHead;
extern struct StringListNode *historyTemp;

void sleep(int);
void printChar(int, int, char, int);
char getChar(int, int);
int getColor(int, int);
void ttprintCharColor(char,int);
void print(int,int,char*);
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
void terminal_keyPressed(unsigned char, char);
void editor_keyPressed(unsigned char, char);
void pitCall();
void rtcCall();
void printStatus(unsigned char);
void enableInterrupts();
void disableInterrupts();
void printPrompt();
unsigned long int millis();
void setMessage(char*);

#endif
