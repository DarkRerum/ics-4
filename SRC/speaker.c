#include "speaker.h"
#include "max.h"
#include "aduc812.h"
#include "led.h"
#include "util.h"

//Спикер использует Timer0, который играет с частотой 4кгц

char speaker_on = 0;

void T0_ISR( void ) __interrupt ( 1 ){
	if( speaker_on ){
		write_max(ENA, 0b0111100);
		//write_max(ENA, 0x3C);
		
	}else{
		write_max(ENA, 0b0100000);
		//write_max(ENA, 0x20);
	}
	
	speaker_on=!speaker_on;
}

void initialize_speaker(){
	SetVector( 0x200B, (void *)T0_ISR );
	TMOD|=0b00000010; //Timer0, внутренний генератор, 8 бит счетчик
	ET0=1;       //включаем прерывание от таймера 0
	TH0=-250;
}
	
void enable_speaker(){
	speaker_on=0;
	TL0=-250;
	TR0=1;      //разрешаем счет таймера 0
}

void disable_speaker(){
	TR0=0;      //запрещаем счет таймера 0
	
	speaker_on=0;
	write_max(ENA, 0b0100000);
}