#include "max.h"
//#include "buzz.h"
#include "int_sio.h"
#include "aduc812.h"
#include "led.h"
#include "fifo.h"
#include "util.h"
#include "sys_timer.h"

#define KEYS_COUNT 16

/*----------------------------------------------------------------------------
            ��६���� � 䫠��
 -----------------------------------------------------------------------------*/

char KBTable[]="147*2580369#ABCD"; //������ ᨬ�����, ᮮ⢥������� �����蠬
                                   //���������� SDK-1.1
static fifo_t keyQueue;

static enum { KEY_RELEASED, KEY_1ST_PRESS, KEY_REPEATED_PRESS }
typedef KEYSTATE_T;

unsigned short __xdata key_states[KEYS_COUNT];
unsigned short __xdata key_counts[KEYS_COUNT];
unsigned long __xdata key_timestamps[KEYS_COUNT];

unsigned int first_repeat_delay;
unsigned int repeat_delay;

unsigned short const KEY_PRESSED_COUNT = 4;
unsigned short const KEY_RELEASED_COUNT = 2;
/*----------------------------------------------------------------------------
                    �㭪樨
 -----------------------------------------------------------------------------*/
 #if 0
void KBTimerHandler(void) __interrupt ( 5 ) {
	unsigned char row,col,rownum,colnum;
	unsigned int i;
	unsigned char kc = 0; //key pressed count


    //�����஢���� �ந�������� �� "�⮫�栬" ����������, �� ����� ��������
    //"����騩 0".
    for(colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        //�� ����� ��� �� ��।��� �⮫��� �� ������ �� "�冷�" �஢������
        //����稥 ��� (䠪� ���몠��� ���⠪� �����襩)
        for(rownum = 0; rownum < 4; rownum++)
        {
            row = read_max(KB) & (0x10 << rownum);
            if( !row ) //�����㦥�� ����⨥ ������:
            {       
                for(i = 0; i<10000; i++)continue;//�஢�ઠ �� �ॡ��� ���⠪�:
                       //�१ �ਬ�୮ 40�� ����� ᪠��஢���� ⮩ �� ������

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
    //return kc; //�� ���� ������ �� �����
}
#endif
 ////////////////OLD

void KBTimerHandler(void) {//__interrupt ( 5 ) {
	static char colnum = 0;	
	unsigned char row,col,rownum;
	unsigned int i;
	static char kc = 0; //key pressed count
	//static unsigned char c= 0;	
	//buzz();
	
	//leds(colnum);
	EA = 0;
		
	col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

	write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

	//�� ����� ��� �� ��।��� �⮫��� �� ������ �� "�冷�" �஢������
	//����稥 ��� (䠪� ���몠��� ���⠪� �����襩)
	for(rownum = 0; rownum < 4; rownum++)
	{
		unsigned char cur_key_id = (colnum<<2) + rownum;
		//leds(key_counts[0]);		
		row = read_max(KB) & (0x10 << rownum);
		if( !row ) //�����㦥�� ����⨥ ������:
		{
			if (key_counts[cur_key_id] < KEY_PRESSED_COUNT*3/2) {
				key_counts[cur_key_id]++;
			}
		}
		else {
			if (key_counts[cur_key_id] != 0)
				key_counts[cur_key_id]--;
		}
						
		if (key_counts[cur_key_id] >= KEY_PRESSED_COUNT) {			
			unsigned long ts;
			//kc++;
			if (kc < 9) {
				switch(key_states[cur_key_id]) {
					case KEY_RELEASED:						
							ts = GetMsCounter();
							key_states[cur_key_id] = KEY_1ST_PRESS;
							kc++;
							if (kc < 3) {
								pushElement(&keyQueue, KBTable[cur_key_id]);
								buzz();
							}							
							key_timestamps[cur_key_id] = GetMsCounter();												
					break;
					case KEY_1ST_PRESS:
						if (kc <=2) {
							ts = GetMsCounter();							
							if ((ts - key_timestamps[cur_key_id]) > first_repeat_delay) {								
								key_states[cur_key_id] = KEY_REPEATED_PRESS;
								pushElement(&keyQueue, KBTable[cur_key_id]);
								buzz();
								key_timestamps[cur_key_id] = ts;
							}
						}
					break;
					case KEY_REPEATED_PRESS:
						if (kc <= 2) {
							ts = GetMsCounter();
							if ((ts - key_timestamps[cur_key_id]) > repeat_delay) {						
								pushElement(&keyQueue, KBTable[cur_key_id]);
								buzz();
								key_timestamps[cur_key_id] = ts;				
							}
							//kc++;
						}
					break;
				}
			}
		} else if (key_counts[cur_key_id] < KEY_RELEASED_COUNT) {			
			switch(key_states[cur_key_id]) {
				case KEY_RELEASED:
					//key_states[cur_key_id] = KEY_1ST_PRESS;
					//pushElement(&keyQueue, KBTable[cur_key_id]);
				break;
				case KEY_1ST_PRESS:
					kc--;
					key_states[cur_key_id] = KEY_RELEASED;
					//pushElement(&keyQueue, KBTable[cur_key_id]);
				break;
				case KEY_REPEATED_PRESS:
					kc--;
					key_states[cur_key_id] = KEY_RELEASED;
					//pushElement(&keyQueue, KBTable[cur_key_id]);
				break;
			
			}
				if (kc < 0) {
					kc = 0;	
				}
		}
	}
	//leds(key_counts[0] | kc << 4 | key_states[0] << 6);
	colnum++;
	if (colnum >= 4) {	
		//kc = 0;
		colnum = 0;		
	}
	TH2 = 0xA0;     // Timer 2 high byte
    TL2 = 0xC2;     // Timer 2 low byte
	EA = 1;	
}
 
/**----------------------------------------------------------------------------
                        ScanKBOnce()
-------------------------------------------------------------------------------
�����筮� ᪠��஢���� ���������� SDK-1.1. �᫨ ����� ������, � 
ᮮ⢥�����騩 �� ᨬ��� (�. ⠡���� KBTable[]) ����頥��� �� 㪠������� 
�����.

�室:       char *ch - ����, �㤠 �������� ᨬ��� ����⮩ ������. �᫨ 
                �� �뫮 ��ॣ����஢��� ������, ��� ��㬥�� ����������.
��室:      *ch - ����饭�� ᨬ��� ��ࢮ� (� ���浪� ᪠��஢����) �� �������
                ������.
�������:  0 - �� ���� ������ �� �뫠 ����� �� ᪠��஢����;
            1 - ��ॣ����஢��� ����⨥.
----------------------------------------------------------------------------- */
/*char ScanKBOnce(char *ch)
{
unsigned char row,col,rownum,colnum;
unsigned int i;
unsigned char kc = 0; //key pressed count

    //�����஢���� �ந�������� �� "�⮫�栬" ����������, �� ����� ��������
    //"����騩 0".
    for(colnum = 0; colnum < 4; colnum++)
    {
        col = 0x1 << colnum; //0001,0010,0100,1000,0001,...

        write_max(KB, ~col); //11111110,11111101,11111011,11110111,11111110,...

        //�� ����� ��� �� ��।��� �⮫��� �� ������ �� "�冷�" �஢������
        //����稥 ��� (䠪� ���몠��� ���⠪� �����襩)
        for(rownum = 0; rownum < 4; rownum++)
        {
            row = read_max(KB) & (0x10 << rownum);
            if( !row ) //�����㦥�� ����⨥ ������:
            {       
                for(i = 0; i<10000; i++)continue;//�஢�ઠ �� �ॡ��� ���⠪�:
                       //�१ �ਬ�୮ 40�� ����� ᪠��஢���� ⮩ �� ������

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

    return kc; //�� ���� ������ �� �����
}*/

char GetKey() {
	EA = 0;
	if (!isEmpty(&keyQueue)) {
		return pullElement(&keyQueue);
	}
	EA = 1;
	return 0;
}

void InitKB(unsigned int first_repeat_d, unsigned int repeat_d) {			
	short i;
	first_repeat_delay = first_repeat_d;
	repeat_delay = repeat_d;
	initFifo(&keyQueue);
	for (i = 0; i < KEYS_COUNT; i++) {
		key_states[i] = KEY_RELEASED;
		key_counts[i] = 0;
	}
	/*TR2 = 1;        // Timer 2 start
    TH2 = 0xFB;     // Timer 2 high byte
    TL2 = 0xC2;     // Timer 2 low byte
    ET2 = 1;        // Timer 2 enable interrupts
	//PT2 = 1;
	SetVector(0x202B, (void *) KBTimerHandler);*/
		
	//EA = 1;	
}

