/****************************************************************************

    test_sio.c - тест драйвера последовательного канала
                 для учебного стенда SDK-1.1

    (C) test_sio.c, Ключев А.О.  2007 г.

Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ.  Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU.

Вы должны были получить копию Универсальной Общественной Лицензии
GNU вместе с этой программой; если нет, напишите по адресу: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
Россия, Санкт-Петербург, кафедра вычислительной техники СПбГУИТМО  
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include "aduc812.h"
//#include "sio.h"
#include "int_sio.h"
#include "dip.h"
#include "led.h"
#include "fifo.h"
#include "mode.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <kb.h>

Mode g_mode;

static void clear_error() {
	leds(0);
}

static void error() {
	leds(0x77);
}

static void process_char() {
	if( rsiostat() )
	{		
		char char_buf[29];
		unsigned char c;
		clear_error();
		c = rsio();
		
		if (c >= 'a' && c <= 'z') {
			unsigned char ptr = 0;
			unsigned char l = c - 0x20 - 1;
			char_buf[ptr++] = c;
			
			for (; l != 'A' - 1; l--, ptr++) {
				char_buf[ptr] = l;
			}
			char_buf[ptr++] = '\r';
			char_buf[ptr++] = '\n';
			char_buf[ptr++] = '\0';
			
			type(char_buf);
		}
		else {
			error();
		}			
	}
}
static unsigned char parse_hex(char* str) {
	unsigned char result = 0;
	while (*str) {		
		char c = tolower(*str);
		result *=16;
		
		if (c >= '0' && c <= '9') {
			result += c - '0';			
		}
		else if (c >= 'a' && c <= 'f') {
			result += c - 'a' + 10;
		}
		else {
			error();
			return result / 16;
		}
		str++;
	}
	return result;
}


static void type_converted(char* str) {
	char buffer[4];
	unsigned char i;
	unsigned char index = 0;
	unsigned char number = parse_hex(str);

	buffer[3] = 0;
	buffer[2] = '0';
	buffer[1] = '0';
	buffer[0] = '0';
	while (number != 0) {
		i = number % 10;
		number /= 10;
		buffer[2 - index] = '0' + i;
		++index;
	}
	type(buffer);
}


void convert_number() {
	int i = 0;
    char buffer[30];
	unsigned char c;
	memset(buffer, 0, sizeof(buffer) * sizeof(buffer[0]));

	i = 0;
	while(1) {
		if (rsiostat()) {
			if ((c = rsio()) != '\r') {
				if (c == '\n') {
					continue;
				}
				buffer[i] = c;
				++i;
				wsio(c);
			}
			else {
				break;
			}
		}			
	}
	clear_error();
	type("\r\n");
	type_converted(buffer);
	type("\r\n");
}

void key_callback() {
	unsigned short i;
	unsigned char ch[10];
	
	char keys_pressed = ScanKBOnce(ch);
	if (keys_pressed > 2) {
		return;
	}
	for (i = 0; i < keys_pressed; i++) {
		wsio(ch[i]);
	}
	
	/*if( ScanKBOnce(ch) )
        {
			
            wsio(ch[0]);
			wsio(ch[1]);
			//buzz();
            //LCD_Putch(ch);
        }
		 /*else
            for(i = 0; i < 2000; i++); //Задержка, регулирующая скорость 
		*/
}

void convert_16_10() {
	//leds(0x11);
}

void main( void )
{

	init_sio( S2400 );
	
	//buzz();
	
	while(0)    //Вывод в бесконечном цикле нажатых на клавиатуре SDK-1.1
    {           //клавиш на ЖКИ и терминал ПК
        //WriteMax(LEDLINE, leds); //Единичка в соответствующем бите зажигает
                                //светодиод, нолик - гасит
        /*leds <<= 1;
        if( !(leds & 0x80) )
            leds |= 1;*/
            

       
                                    //изменения состояния светодиодов
    }
	
    while( 1 )
    {
		if (getDips() != 0) {
			EA = 0;
			g_mode = MODE_CHAR;
		}
		else {
			g_mode = MODE_NUM;
			EA = 1;
		}
		
		if (g_mode == MODE_CHAR) {
			key_callback();
		}
		else {
			convert_16_10();
		} 
    }
}    
