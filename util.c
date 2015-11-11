
#include "util.h"
#include "malloc.h"
/*
struct StringListNode* makeStringBuilder(char* s){
	struct StringListNode* node = (struct StringListNode*) malloc(sizeof(struct StringListNode));
	node->prev = 0;
	node->next = 0;
	char* str = (char*)malloc(strLen(s));
	memCopy(s,str,strLen(s));
	node->str = str;
	return s;
}
*/

int flattenStringList(struct StringListNode* head, char** str){
	int length = 0, index = 0;
	struct StringListNode *temp = head;
	while(temp){
		length += strLen(temp->str)-1;
		temp = temp->next;	
	}
	*str = (char*)malloc(length+1);
	temp = head;
	while(temp){
		memCopy(temp->str, (*str)+index, strLen(temp->str)-1);
		index += strLen(temp->str)-1;
		temp = temp->next;
	}
	(*str)[index] = 0;
	return length;
}

void strReplace(char* str, char from, char to){
	while(*str){
		if(*str == from)
			*str = to;
		str++;
	}
}


void memFill(char* to, char filler, int len){
	int i;
	for(i = 0; i < len; i++) to[i] = filler;
}
void memCopy(char* from, char* to, int len){
	int i;
	for(i = 0; i < len; i++){
		to[i] = from[i];
	}
}

int strLen(char* str){
	int i = 0;
	while(str[i++] != 0);
	return i;
}

void doubleToString(double n, char* s){
	//default to two decimal places
	int intpart = (int) n;
	int floatpart = (int)(100*n) % 100;
	intToString(intpart, s);
	int len = strLen(s)-1;
	s[len] = '.';
	intToString(floatpart, (s+len+1));
}

void intToString(int n, char s[]){
	int num = n;
	if(n == 0){
		s[0] = '0';
		s[1] = 0;
		return;
	}
	if(num < 0){
		s++;
		n*=-1;
	}
	int i;
	for(i = 0; n != 0; i++){
		s[i] = '0'+(n % 10);
		n/=10;
	}
	s[i] = 0;
	reverseInPlace(s);
	if(num < 0){
		s--;
		s[0] = '-';
	}
}

int strToInt(char* s){
	int sign = 1;
	if(s[0] == '-') sign = -1;
	int n = 0;
	int i = (sign == 1 ? 0 : 1);
	int len = strLen(s);
	int power = pow(10,len-i-2);
	while(s[i] != 0){
		n += (s[i] - '0') * power;
		i++;
		power /= 10;
	}
	return n*sign;
}

void itoh(char n, char* s){
	unsigned char c = n;
	char upper = (c / 16);
	char lower = (c % 16);
	s[0] = upper < 10 ? '0' + upper : 'A' + (upper-10);
	s[1] = lower < 10 ? '0' + lower : 'A' + (lower-10);
}

char table[2][49] = {{0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x39, 0x1C, 0x34, 0x33, 0x35, 0x0D, 0x0C, 0x29, 0x1A, 0x1B, 0x2B, 0x28, 0x27},
					 {'0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , ' ' , '\n', '.' , ',' , '/', '=' , '-' , '`' , '[' , ']' , '\\', '\'', ';' }};

char scancodeToAscii(unsigned char scan, char shift, char ctrl, char caps){
	int i;
	for(i = 0; i < sizeof(table[0]); i++){
		if(scan == table[0][i]){
			if(!(shift || caps) && table[1][i]>='A' && table[1][i]<='Z') return table[1][i]-'A'+'a';
			if(shift || caps){
				if(table[1][i] == '1') return '!';
				if(table[1][i] == '2') return '@';
				if(table[1][i] == '3') return '#';
				if(table[1][i] == '4') return '$';
				if(table[1][i] == '5') return '%';
				if(table[1][i] == '6') return '^';
				if(table[1][i] == '7') return '&';
				if(table[1][i] == '8') return '*';
				if(table[1][i] == '9') return '(';
				if(table[1][i] == '0') return ')';
				if(table[1][i] == ',') return '<';
				if(table[1][i] == '.') return '>';
				if(table[1][i] == '/') return '?';
				if(table[1][i] == '=') return '+';
				if(table[1][i] == '-') return '_';
				if(table[1][i] == '`') return '~';
				if(table[1][i] == '[') return '{';
				if(table[1][i] == ']') return '}';
				if(table[1][i] == '\\') return '|';
				if(table[1][i] == '\'') return '"';
				if(table[1][i] == ';') return ':';
			}
			return table[1][i];
		}
	}
	return 0;
}
void charToString(unsigned char c, char* str){
	char high = c >> 4;
	char low = c%16;
	str[0] = '0';
	str[1] = 'x';
	str[2] = '0' + high; 
	if(str[2] > '9'){
		str[2] += 'A' - '9' - 1;
	}
	str[3] = '0' + low;
	if(str[3] > '9'){
		str[3] += 'A' - '9' - 1;
	}
	str[4] = 0;
}
int strEquals(char* a, char* b){
	int i = 0;
	while(a[i] == b[i]){
		if(a[i++] == 0) return 1;
	}
	return 0;
}

int strBeginsWith(char* a, char*b){
	if(strLen(a) < strLen(b)) return 0;
	int i = 0;
	while(a[i] == b[i]) i++;
	if(b[i] == 0) return 1;
	return 0;
}

void reverseInPlace(char s[]){
	if(strLen(s) <= 1){
		return;
	}
	int i,j;
	char c;
	for(i = 0, j = strLen(s)-2; i < j; i++, j--){
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int isLower(char c){
	return c >= 'a' && c <= 'z';
}
int isUpper(char c){
	return c >= 'A' && c <= 'Z';
}

int isAlpha(char c){
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isNumeric(char c){
	return c >= '0' && c <= '9';
}

int isAlphaNumeric(char c){
	return isAlpha(c) || isNumeric(c);
}
static unsigned long int rNum;
void setSeed(unsigned int seed){
	rNum = seed;
}
unsigned int rand(unsigned int max){
	rNum = rNum * 1103515245+12345;
	return (unsigned int)(rNum/65536) % max;
}

int pow(int base, int exp)
{
	// exponentiation by squaring
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}
int numDigits(int num){
	if(num < 0) num *= -1;
	if ( num < 10 )
		return 1;
	if ( num < 100 )
		return 2;
	if ( num < 1000 )
		return 3;
	if ( num < 10000 )
		return 4;
	if ( num < 100000 )
		return 5;
	if ( num < 1000000 )
		return 6;
	if ( num < 10000000 )
		return 7;
	if ( num < 100000000 )
		return 8;
	if ( num < 1000000000 )
		return 9;
	if ( num < 10000000000 )
		return 10;
	return 11;
}
