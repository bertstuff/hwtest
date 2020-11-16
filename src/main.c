/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
//#include "Driver/include/lpc17xx_wdt.h"
#endif


/* Include files */
#include <Board/Def_init.h>

#include <Device/I2C/BUTTONS/Buttons_3x4.h>

//#include <Screen/SCR_Init.h>
//#include <Screen/SCR_Startup.h>
#include <Screen/Transaction/SCR_Transaction_init.h>

#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/ntp/ntpd.h>
#include <Core/protocol/Net/AMS/AMS.h>
#include <Core/Debug/Error_check.h>
#include <Core/Debug/Debug_Terminal.h>
//#include <Core/Device/Common/LCD_16x2.h>

#include "Global.h"
#include "Power.h"
uint16_t Server_prefix_nr = 0;

extern uint8_t lcd_i2c_data;
extern int16_t lcd_addr; //if < 0 than gpio device
void testbacklight()
{

	int i;
	for( i = 0; i < 10; i++ ) {
		I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
		lcd_i2c_data ^= 0x80;
		int j;
		for( j = 0; j < 10; j++ ) LCD_2x16_Delay(1000);
	}
}


void scanlcd()
{
	int i, n = 0, addr=-1;
	for( i = 0; i < 8; i++ ) {
		Status rc = LCD_2x16_I2C_Init(i);
		if( rc != ERROR ) {
			n++;
			addr = i;
		}
		printf(rc == ERROR ? "No display at address %d\n" : "Display found at address %d\n",i);
	}
	if( n == 1 ) {
		printf("Re-init address %d\n",addr);
		LCD_2x16_I2C_Init(addr);
		Point_t p = { 0,0 };
		LCD_2x16_Draw_String(p,"Een tekst op regel 1\r\nNog een tekst op regel 2");
		testbacklight();
	} else {
		printf("Number of lcds found != 1\n");
	}

}

void writelcd(char *str)
{
	Point_t p = { 0,0 };
	LCD_2x16_Draw_String(p,str);
}


void testkeyboard()
{
	Button3x4_init(0);
	uint8_t kar,prev_kar,bl=0;
	while( Serial_Terminal_Bytes_Avail() <= 0 ) {
		kar = Button3x4_get_value(bl);
		if( kar != prev_kar ) {
			printf("%x -> %x\n",prev_kar,kar);
			prev_kar = kar;
			bl = !bl;
		}
	}
	kar = Serial_Terminal_getc();
}

void testkeyboard1()
{
	const uint8_t expected[] = {1,2,3,4,5,6,7,8,9,0x58,0,0x56};
	Button3x4_init(0);

	printf("Press keys 1-9,left bottom, mid bottom, right bottom\n");
	uint8_t kar,prev_kar,bl=0, next = 0;

	while( Serial_Terminal_Bytes_Avail() <= 0 && next < sizeof(expected) ) {
		kar = Button3x4_get_value(bl);
		if( kar != prev_kar ) {
			printf("%x -> %x\n",prev_kar,kar);
			if( kar == 0xFF ) {
				if( prev_kar == expected[next] ) {
					printf(".");
					next++;
				} else {
					printf("error, start over with first key\n");
					next = 0;
				}
			}
			prev_kar = kar;
			bl = !bl;
		}
	}

	kar = Serial_Terminal_getc();
}

void DebugCommands()
{
	int kar;
	static char readLine[80];
	static int pos;
	static char saved;
	while( Serial_Terminal_Bytes_Avail() > 0) {
		kar = Serial_Terminal_getc();
		switch(kar) {
		case '\r':
		case '\n':
			printf("\n");
			if( pos == 0 ) {
				readLine[0] = saved;

			} else {
				saved = readLine[0];
				readLine[pos] = 0;
				pos = 0;
			}
			if( !strcmp(readLine,"help") ) {
				printf("Help\n");
			} else if( !strcmp(readLine,"scanlcd") ) {
				scanlcd();
			} else if( !strncmp(readLine,"writelcd",8) ) {
				writelcd(readLine+8);
			} else if( !strcmp(readLine,"testbl") ) {
				testbacklight();
			} else if( !strcmp(readLine,"testkb") ) {
				testkeyboard1();
			} else if( !strcmp(readLine,"blon") ) {
				I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
			} else {
				printf("\nunknown command %s\n",readLine);
			}
			printf(">");
			break;
		case 8 :
			printf("\b \b");
			if( pos > 0 ) pos--;
			break;
		default:
			readLine[pos++] = kar;
			printf("%c",kar);
		}
	}
}


/* Private Variables */
int main(void) {
	Default_init();

	Status r=LCD_2x16_I2C_Init(0);

	Point_t p = { 0,0 };
	Status s = LCD_2x16_Draw_String(p,"Tekst op regel 1\r\nTekst op regel 2");
	//Start main applications
	printf("Enter main loop  %d %d\r\n",r,s);
	while (1){
		DebugCommands();
	}
	  return 0;
}


