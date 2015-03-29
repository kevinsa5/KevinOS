#include "driver.h"

void writeByteToPort(unsigned short port, unsigned char data){
	__asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}
unsigned char readByteFromPort(unsigned short port){
	unsigned char data;
	__asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
	return data;
}
int PITfreq;
void setTimerFreq(int hz){
	PITfreq = hz;
	int divisor = 1193180 / hz;      
	writeByteToPort(0x43, 0x36);             /* Set our command byte 0x36 */
	writeByteToPort(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	writeByteToPort(0x40, divisor >> 8);     /* Set high byte of divisor */
}
void play_sound(int nFrequency) {
	if(nFrequency == 0){
		char tmp = (readByteFromPort(0x61) & 0xFC);
		writeByteToPort(0x61, tmp);
		return;
 	}
 	int Div;
 	char tmp;
 
    //Set the PIT to the desired frequency
	Div = 1193180 / nFrequency;
	writeByteToPort(0x43, 0xb6);
	writeByteToPort(0x42, (char) (Div) );
	writeByteToPort(0x42, (char) (Div >> 8));

	//And play the sound using the PC speaker
	tmp = readByteFromPort(0x61);
	if (tmp != (tmp | 3)) {
		writeByteToPort(0x61, tmp | 3);
	}
}
