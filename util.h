#ifndef UTIL_H
#define UTIL_H

void memFill(char*, char, int);
void memCopy(char*, char*, int);
int strLen(char*);
void intToString(int, char*);
int strToInt(char*);
char scancodeToAscii(unsigned char, char, char, char);
void charToString(unsigned char, char*);
int strEquals(char*, char*);
int strBeginsWith(char*, char*);
void reverseInPlace(char*);
void setSeed(unsigned int);
unsigned int rand(unsigned int);
int pow(int, int);
int numDigits(int);
int isAlpha(char);
int isNumeric(char);
int isAlphaNumeric(char);

struct StringListNode
{
    char* str;
    struct StringListNode *next;
    struct StringListNode *prev;
};

struct File
{
    char* filename;
    int filesize;
    struct StringListNode *firstLine;
};

#endif
