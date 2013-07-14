void writeByteToPort(unsigned short port, unsigned char data){
	__asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}
unsigned char readByteFromPort(unsigned short port){
	unsigned char data;
	__asm__("in %%dx, %%al" : "=a" (data) : "d" (port));
	return data;
}
void setTimerFreq(int hz){
	int divisor = 1193180 / hz;      
    writeByteToPort(0x43, 0x36);             /* Set our command byte 0x36 */
    writeByteToPort(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    writeByteToPort(0x40, divisor >> 8);     /* Set high byte of divisor */
}
