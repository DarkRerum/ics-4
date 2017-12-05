#ifndef _KB_H
#define _KB_H

void InitKB(unsigned int first_repeat_delay, unsigned int repeat_speed);
extern char ScanKBOnce(char *ch);
void KBTimerHandler();
char GetKey();

#endif //_KB_H
