#include "aduc812.h"
#include "util.h"
#include "led.h"
#include "int_sio.h"
#include "max.h"
#include "util.h"
#include "speaker.h"
#include "buzz.h"
#include "kb.h"
#include "sys_timer.h"

void main( void )
{
	InitTimer();
	initialize_speaker();
	init_sio(S9600);	
	//InitKB(0, 0);
	//enable_speaker();
	type("Hello from int_sio\n");
	
	EA = 1;
	enable_speaker();
	while (1) {
		unsigned char k;
		k = GetKey();	
		if (k) {
			enable_speaker();
			wsio(k);
			disable_speaker();
		}
		//leds(GetMsCounter());
		//leds(TL0);		
	}
}    
