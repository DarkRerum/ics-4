#ifndef __UTIL_H
#define __UTIL_H

#define S9600   0xFD
#define S4800   0xFA
#define S2400   0xF4
#define S1200   0xE8

void SetVector(unsigned char __xdata * Address, void * Vector);

#endif