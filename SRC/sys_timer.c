#include "aduc812.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "sys_timer.h"
#include "max.h"
#include "speaker.h"
#include "kb.h"
#define MAXBASE 8

///////////////////////// WriteMax ///////////////////////////
// Запись байта в регистр ПЛИС
// Вход:
// regnum – адрес регистра ПЛИС,
// val – записываемое значение.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////

unsigned long __systime = 0;


unsigned long GetMsCounter( void ) 
{ 
	unsigned long res; 
	ET2 = 0; 
	res = __systime; 
	ET2 = 1; 
	return res; 
} 

unsigned long DTimeMs( unsigned long t2 ) 
{ 
    unsigned long t1 = ( unsigned long )GetMsCounter(); 
    return t1 - t2;   
} 

void DelayMs( unsigned long ms ) 
{ 
 unsigned long t1 = ( unsigned long )GetMsCounter(); 
 while ( 1 ) 
 { 
       if ( DTimeMs( t1 ) > ms ) break; 
 }   
} 


void T2_handler ( void ) __interrupt (5) {
    TF2 = 0;
    __systime++;
    //TH2 = 0xFB;
    TH2 = 0xFB;
    TL2 = 0xC3;
	
	if (__systime % 10 == 0)
	{
		KBTimerHandler();
	}
}

void InitTimer( void ) {
    SetVector( 0x202B, (void *)T2_handler );
	// Разрешение прерываний от таймера 0
	ET0 = 1;
    //EX0 = 1;    //INT0 interrupt enable
    
    //PT2 = 1;        // Timer 2 high priority
    //PT0 = 0;        // Timer 0 low priority
    TR2 = 1;        // Timer 2 start
    TH2 = 0xFB;     // Timer 2 high byte
    TL2 = 0xC2;     // Timer 2 low byte
    ET2 = 1;        // Timer 2 enable interrupts
    //EA = 1;         // Enable global interrupts
}