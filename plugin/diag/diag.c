/**********************************************************************************

	Copyright (C) 2015 by Tiger Leap <weinyzhou86@gmail.com>

*    File          ：diag.c

*    Link          ：http://blog.csdn.net/weinyzhou

*    Designer      ： Tiger Zhou (周谋雄)

*    Contact Email ：weinyzhou86@gmail.com

*    Build Date    ：2015/08/10 17:47

*    Modify Date   ：

*    Description   ：版权所有,未经允许不得擅自拷贝，复制该软件的部分或全部，违者必究！有任何疑问可与我取得联系。.

*    Copyright (c) 2015 Tiger Leap(Tiger Zhou/Weiny Zhou). All rights reserved.

**********************************************************************************/


#include "diag.h"

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <wiringPi.h>

#define PINMODE(GPIO,ISOUT) pinMode(GPIO,ISOUT)
#define PINMODE_I(GPIO) PINMODE(GPIO,INPUT)
#define PINMODE_O(GPIO) PINMODE(GPIO,OUTPUT)

#define DIGITALWRITE(GPIO,ISHIGH) digitalWrite(GPIO,ISHIGH)
#define DIGITALWRITE_H(GPIO) DIGITALWRITE(GPIO,HIGH)
#define DIGITALWRITE_L(GPIO) DIGITALWRITE(GPIO,LOW)



Diag* diag_create(int sclk, int rclk, int dio)
{
	Diag* diag = NULL;
	diag = (Diag*)malloc(sizeof(Diag));
	if (!diag){
		printf("new mem is failed.");
		goto error_lab;
	}
	memset(diag, 0, sizeof(Diag));
	diag->sclk = sclk;
	diag->rclk = rclk;
	diag->dio = dio;
	wiringPiSetup();
	PINMODE_O(diag->sclk);
	PINMODE_O(diag->rclk);
	PINMODE_O(diag->dio);

	DIGITALWRITE_L(diag->sclk);
	DIGITALWRITE_L(diag->rclk);

error_lab:
	return diag;
}

static unsigned char leds[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, //0-9
	0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF, 0xbf
};
static void led_out(Diag* diag,unsigned char x){
	unsigned char i;
	for (i = 8; i >= 1; i--)
	{
		if (x & 0x80) DIGITALWRITE_H(diag->dio); else DIGITALWRITE_L(diag->dio);
		x <<= 1;
		DIGITALWRITE_L(diag->sclk);
		DIGITALWRITE_H(diag->sclk);
	}
}

static void display(Diag* diag)
{
	DIGITALWRITE_L(diag->rclk);
	DIGITALWRITE_H(diag->rclk);
}

int diag_clean(Diag* diag)
{
	int ret = -1;
	if (diag == NULL){
		printf("input diag is null.");
		goto error_lab;
	}
	led_out(diag, 0x00);
	display(diag);
	ret = 0;
error_lab:
	return ret;
}

int diag_shownumInt(Diag* diag, int num)
{
	int ret = -1;
	char buf[10] = { 0 };
	
	if (diag == NULL){
		printf("input diag is null.");
		goto error_lab;
	}
	ret = snprintf(buf, sizeof(buf), "%d", num);
	if (ret < 0){
		ret = diag_shownumStr(diag, "ERR");
	}
	else
		ret = diag_shownumStr(diag,buf);
error_lab:
	return ret;
}

int diag_shownumStr(Diag* diag, const char* num)
{
	int ret = -1;
	int i = 0,p=0;
	if (diag == NULL){
		printf("input diag is null.");
		goto error_lab;
	}
	for ( i = 0;i<strlen(num);++i,++p)
	{
		unsigned char val = 0;
		switch (num[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			const unsigned char code[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };
			int idx = num[i] - '0';
			val=code[idx];
		}break;
		case 'a':case 'A':
			val=0x88;
			break;
		case 'b':case 'B':
			val = 0x83;
			break;
		case 'c':case 'C':
			val = 0xc6;
			break;
		case 'd':case 'D':
			val = 0xa1;
			break;
		case 'e':case 'E':
			val = 0x86;
			break;
		case 'f':case 'F':
			val = 0x8e;
			break;
		case 'g':case 'G':
			val = 0xc2;
			break;
		case 'h':case 'H':
			val = 0x89;
			break;
		case 'i':case 'I':
			val = 0xcf;
			break;
		case 'j':case 'J':
			val = 0xf0;
			break;
		case 'l':case 'L':
			val = 0xc7;
			break;
		case 'n':case 'N':
			val = 0xab;
			break;
		case 'o':case 'O':
			val = 0xa3;
			break;
		case 'p':case 'P':
			val = 0x8c;
			break;
		case 'q':case 'Q':
			val = 0x98;
			break;
		case 'u':case 'U':
			val = 0xc1;
			break;
		case 'y':case 'Y':
			val = 0x91;
			break;
		case 'r':case 'R':
			val = 0x81;
			break;
		case '-':
			val = 0xbf;
			break;
		case '_':
			val = 0xf7;
			break;
		
		default:
			
			break;
		}
		if (num[i ]=='.')
			val = 0x7f;
		else if (num[i + 1] == '.')
		{
			val &= 0x7f;
			++i;
		}

		led_out(diag, val);
		led_out(diag, pow(2,3-p));
		display(diag);
		delay(2);
	}
	ret = 0;
error_lab:
	return ret;
}



void diag_destroy(Diag* diag)
{
	if (diag == NULL)
	{
		printf("input diag is null.");
		return;
	}
	free(diag);
}