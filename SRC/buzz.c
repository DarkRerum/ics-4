/******************************************************************************
                (�) ��� "���", �����-������, �����, 2002.
                http://lmt.cs.ifmo.ru, tel.: +7-812-233-3096,
                mailto: lmt@d1.ifmo.ru

        ����� 䠩� ����  ᢮�����  �����࠭塞�  �ਬ�஬  ��
    �몥  ��  (���������  Keil  C  ���  KEIL  ELEKTRONIK  GmbH)  �
    �।�����祭    ���    �������樨     ࠡ���    �    ������᪨�
    �쥧������⥫�� �⥭��.
        �����  �ணࠬ���   �����   ��    �����࣠���   ��⥫쭮��
    ���஢����  �  �����   ᮤ�ঠ��   �訡��.   �����   ��   ����� 
    �⢥��⢥����� �� �����  ���ଠ樨  �  ���  ��������  �।��
    ��᫥��⢨� �ᯮ�짮����� ������� �ணࠬ�����  �����  �  ��⠢�
    �ணࠬ���� �஥�⮢.
        ����� 䠩� ����� �����࣠���� ��� ���������, ��  ��  �����
    �����࠭����� � �������᪨� 楫��  �  �  ��⠢�  �������᪨�
    �ணࠬ���� �த�⮢.
    

����:       buzz.c
�����:     1.0.0
����:      LAN
���ᠭ��:   ����� ��������樮����� �ணࠬ����� ���ᯥ祭�� ��� �⥭��
            SDK-1.1. ����� � ������᪨� �쥧������⥫��. ����� ����� 
            �ᯮ���� ����� ࠡ��� � �㭪樮�����  ����⥫��
            業�ࠫ쭮�� ������ �⥭�� �� ���� ���� Altera 
            MAX3064 (3128) (䠩�� max.c, max.h).

���������:
-------------------------------------------------------------------------------
N ���     �����   ����               ���ᠭ��   
-------------------------------------------------------------------------------
1 10.04.02  1.0.0   LAN     ������
******************************************************************************/
#include "max.h"
#include <led.h>

/**----------------------------------------------------------------------------
                        Buzz()
-------------------------------------------------------------------------------
����� ���⪮�� (���浪� 10��) ᨣ���� � ���⮩ �ਬ�୮ 7.5��� �� ��㪮���
�쥧������⥫�.

�室:       ���
��室:      ���
�������:  ���
----------------------------------------------------------------------------- */
void buzz(void)
{
unsigned char i;//
unsigned short dur;

	//leds(0xF4);

    for(dur = 0; dur < 500; dur++)
    {
        write_max(ENA,0x3C);
        for(i=0; i < 2; i++)continue;
        write_max(ENA,0x20);
        for(i=0; i < 2; i++)continue;
    }

}

