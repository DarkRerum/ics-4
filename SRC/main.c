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

static unsigned char parse_hex(char* str) {
	unsigned char result = 0;
	while (*str) {
		char c = str[0];
		result *=16;
		
		if (c >= '0' && c <= '9') {
			result += c - '0';			
		}
		else if (c >= 'A' && c <= 'F') {
			result += c - 'A' + 10;
		}
		else {
			//error();
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


void read() {
	unsigned char k[3];
	char i;
	for (i = 0; i < 3; i++) {
		do {
			k[i] = GetKey();
		}
		while (k[i] == 0);
		wsio(k[i]);
		if (k[i] == '#') {
			if(i == 0) {
				type("\r\nERR empty number\r\n");
				return;
			}			
			k[i] = 0;
			//wsio('\n');
			//wsio('\r');
			type_converted(k);
			wsio('\n');
			wsio('\r');			
		}
		else if (i == 2) {
			type("\r\nERR long number\r\n");
			return;
		}
	}
}

void main( void )
{
	char mode = 0;
	InitTimer();
	initialize_speaker();
	init_sio(S9600);	
	InitKB(0, 0);
	//enable_speaker();
	type("Hello from int_sio\n");
	
	EA = 1;
	enable_speaker();	
	while (1) {
		if (mode) {
			read();
		}
		else {
			unsigned char k;
			k = GetKey();	
			if (k) {
				enable_speaker();
				wsio(k);		
			}
		}		
	}
}    
