#include "max.h"
//#include "buzz.h"
#include "int_sio.h"
#include "aduc812.h"
#include "led.h"
#include "fifo.h"
#include "util.h"

#define KEYS_COUNT 16

/*----------------------------------------------------------------------------
            Переменные и флаги
 -----------------------------------------------------------------------------*/

char KBTable[]="147*2580369#ABCD"; //Таблица символов, соответствующих клавишам
                                   //клавиатуры SDK-1.1
static fifo_t keyQueue;
char buffer [2];

static enum { KEY_RELEASED, KEY_1ST_PRESS, KEY_REPEATED_PRESS }
typedef KEYSTATE_T;

unsigned char key_states[KEYS_COUNT];
unsigned char key_counts[KEYS_COUNT];

unsigned char const KEY_PRESSED_COUNT = 400;
unsigned char const KEY_RELEASED_COUNT = 30;
/*----------------------------------------------------------------------------
                    Функции
 -----------------------------------------------------------------------------*/
 #if 0
void KBTimerHandler(void) __interrupt ( 5 ) {
	unsigned char row,col,rownum,colnum;
	unsigned int i;
	unsigned char kc = 0; //key pressed count


    //Сканирование производится по "столбцам" клавиатуры, на которые подается
    //"бегущий 0".
    for(colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        //При подаче нуля на очередной столбец на каждом из "рядов" проверяется
        //наличие нуля (факт замыкания контакта клавишей)
        for(rownum = 0; rownum < 4; rownum++)
        {
            row = read_max(KB) & (0x10 << rownum);
            if( !row ) //Обнаружено нажатие клавиши:
            {       
                for(i = 0; i<10000; i++)continue;//проверка на дребезг контакта:
                       //через примерно 40мс повтор сканирования той же клавиши

                row = read_max(KB) & (0x10 << rownum);
                if( !row )
                {
					//buzz();					
                    //*ch = (KBTable[(colnum<<2) + rownum]);
					//wsio(*ch);
					pushElement(&keyQueue, KBTable[(colnum<<2) + rownum]);
					//leds(0b111);
					kc++;					
                }

            }
        }

    }
	TH2 = 0xFB;     // Timer 2 high byte
    TL2 = 0xC2; 
    //return kc; //Ни одна клавиша не нажата
}
#endif
 ////////////////OLD

void KBTimerHandler(void) __interrupt ( 5 ) {
	static char colnum = 0;	
	unsigned char row,col,rownum;
	unsigned int i;
	unsigned char kc = 0; //key pressed count
	//static unsigned char c= 0;	
	//buzz();
	
	//leds(colnum);
	EA = 0;
		
	col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

	write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

	//При подаче нуля на очередной столбец на каждом из "рядов" проверяется
	//наличие нуля (факт замыкания контакта клавишей)
	for(rownum = 0; rownum < 4; rownum++)
	{
		
		unsigned char cur_key_id = (colnum<<2) + rownum;
		row = read_max(KB) & (0x10 << rownum);
		if( !row ) //Обнаружено нажатие клавиши:
		{       												
			row = read_max(KB) & (0x10 << rownum);
			key_counts[cur_key_id]++;			
						
			if (key_counts[cur_key_id] > KEY_PRESSED_COUNT) {
				key_states[cur_key_id] = KEY_1ST_PRESS;
				pushElement(&keyQueue, KBTable[cur_key_id]);
			}

		}
		else {
			key_counts[cur_key_id]--;
			
			if (key_counts[cur_key_id] < KEY_RELEASED_COUNT) {
				
			}
		}
	}
	
	colnum++;
	if (colnum >= 4) {	
		kc = 0;
		colnum = 0;		
	}
	TH2 = 0x80;     // Timer 2 high byte
    TL2 = 0xC2;     // Timer 2 low byte
	EA = 1;	
}
 
/**----------------------------------------------------------------------------
                        ScanKBOnce()
-------------------------------------------------------------------------------
Единичное сканирование клавиатуры SDK-1.1. Если нажата клавиша, то 
соответствующий ей символ (см. таблицу KBTable[]) помещается по указанному 
адресу.

Вход:       char *ch - адрес, куда поместить символ нажатой клавиши. Если 
                не было зарегистрировано нажатия, этот аргумент игнорируется.
Выход:      *ch - помещенный символ первой (в порядке сканирования) из нажатых
                клавиш.
Результат:  0 - ни одна клавиша не была нажата при сканировании;
            1 - зарегистрировано нажатие.
----------------------------------------------------------------------------- */
/*char ScanKBOnce(char *ch)
{
unsigned char row,col,rownum,colnum;
unsigned int i;
unsigned char kc = 0; //key pressed count

    //Сканирование производится по "столбцам" клавиатуры, на которые подается
    //"бегущий 0".
    for(colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        //При подаче нуля на очередной столбец на каждом из "рядов" проверяется
        //наличие нуля (факт замыкания контакта клавишей)
        for(rownum = 0; rownum < 4; rownum++)
        {
            row = read_max(KB) & (0x10 << rownum);
            if( !row ) //Обнаружено нажатие клавиши:
            {       
                for(i = 0; i<10000; i++)continue;//проверка на дребезг контакта:
                       //через примерно 40мс повтор сканирования той же клавиши

                row = read_max(KB) & (0x10 << rownum);
                if( !row )
                {
					//buzz();					
                    *ch = (KBTable[(colnum<<2) + rownum]);
					wsio(*ch);
					leds(0b111);
					kc++;
					ch++;
                }

            }
        }

    }

    return kc; //Ни одна клавиша не нажата
}*/

char GetKey() {
	EA = 0;
	if (!isEmpty(&keyQueue)) {
		return pullElement(&keyQueue);
	}
	EA = 1;
	return 0;
}

void InitKB(unsigned int first_repeat_delay, unsigned int repeat_speed) {			
	short i;
	initFifo(&keyQueue);
	for (i = 0; i < KEYS_COUNT; i++) {
		key_states[i] = KEY_RELEASED;
		key_counts[i] = 0;
	}
	TR2 = 1;        // Timer 2 start
    TH2 = 0xFB;     // Timer 2 high byte
    TL2 = 0xC2;     // Timer 2 low byte
    ET2 = 1;        // Timer 2 enable interrupts
	//PT2 = 1;
	SetVector(0x202B, (void *) KBTimerHandler);
		
	//EA = 1;	
}

