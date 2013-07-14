
void memCopy(char*, char*, int);
int strLen(char*);
void intToString(int, char*);
int strToInt(char*);
char scancodeToAscii(unsigned char, int, int, int);
void charToString(unsigned char, char*);
int strEquals(char*, char*);
int strBeginsWith(char*, char*);
void reverseInPlace(char*);
void setSeed(unsigned int);
unsigned int rand(unsigned int);
int pow(int, int);

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

void intToString(int n, char s[]){
	if(n == 0){
		s[0] = '0';
		s[1] = 0;
		return;
	}
	int i;
	for(i = 0; n != 0; i++){
		s[i] = '0'+(n % 10);
		n/=10;
	}
	s[i] = '\0';
	reverseInPlace(s);
}

int strToInt(char* s){
	int n = 0;
	int i = 0;
	int len = strLen(s);
	while(s[i] != 0){
		n += (s[i] - '0') * pow(10,len-i-2);
		i++;
	}
	return n;
}

char table[2][41] = {{0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x39, 0x1C, 0x34, 0x33, 0x35},
					 {'0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , ' ' , '\n', '.' , ',' , '/'}};

char scancodeToAscii(unsigned char scan, int shift, int ctrl, int caps){
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

static unsigned long int rNum;
void setSeed(unsigned int seed){
	rNum = seed;
}
unsigned int rand(unsigned int max){
	rNum = rNum * 1103515245+12345;
	return (unsigned int)(rNum/65536) % max;
}

int pow(int base, int power){
	int n = 1;
	while(power-- > 0) n*=base;
	return n;
} 