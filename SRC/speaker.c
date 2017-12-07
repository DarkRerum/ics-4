#include "speaker.h"
#include "max.h"
#include "aduc812.h"
#include "led.h"
#include "util.h"

//������ ���������� Timer0, ������� ������ � �������� 4���

char speaker_on = 0;
short play_time = 0;
short play_length = 500;

void T0_ISR( void ) __interrupt ( 1 ){
	if( speaker_on ){
		write_max(ENA, 0b0111100);
		//write_max(ENA, 0x3C);
		
	}else{
		write_max(ENA, 0b0100000);
		//write_max(ENA, 0x20);
	}
	
	speaker_on=!speaker_on;
	
	play_time += 1;	
	
	if (play_time >= play_length) {
		disable_speaker();
	}
}

void initialize_speaker(){
	SetVector( 0x200B, (void *)T0_ISR );
	TMOD|=0b00000010; //Timer0, ���������� ���������, 8 ��� �������
	ET0=1;       //�������� ���������� �� ������� 0
	TH0=-250;
}

void buzz(){
	play_time = 0;
	speaker_on=0;
	TL0=-250;
	TR0=1;      //��������� ���� ������� 0
}

void enable_speaker(){
	play_time = 0;
	speaker_on=0;
	TL0=-250;
	TR0=1;      //��������� ���� ������� 0
}

void disable_speaker(){
	TR0=0;      //��������� ���� ������� 0
	
	speaker_on=0;
	write_max(ENA, 0b0100000);
}