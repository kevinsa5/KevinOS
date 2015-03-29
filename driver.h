#ifndef DRIVER_H
#define DRIVER_H

void writeByteToPort(unsigned short, unsigned char );
unsigned char readByteFromPort(unsigned short);

void setTimerFreq(int);
void play_sound(int);
int PITfreq;
#endif
