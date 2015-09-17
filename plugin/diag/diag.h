/**********************************************************************************

	Copyright (C) 2015 by Tiger Leap <weinyzhou86@gmail.com>

*    File          ��diag.h

*    Link          ��http://blog.csdn.net/weinyzhou

*    Designer      �� Tiger Zhou (��ı��)

*    Contact Email ��weinyzhou86@gmail.com

*    Build Date    ��2015/08/10 17:47

*    Modify Date   ��

*    Description   ����Ȩ����,δ�����������Կ��������Ƹ�����Ĳ��ֻ�ȫ����Υ�߱ؾ������κ����ʿ�����ȡ����ϵ��.

*    Copyright (c) 2015 Tiger Leap(Tiger Zhou/Weiny Zhou). All rights reserved.

**********************************************************************************/
#ifndef __diag_H__
#define __diag_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>  




typedef struct Diag_t
{
	int sclk;
	int rclk;
	int dio;

}Diag;



Diag* diag_create(int sclk, int rclk, int dio);

int diag_clean(Diag* diag);

int diag_shownumInt(Diag* diag, int num);

int diag_shownumStr(Diag* diag, const char* num);



void diag_destroy(Diag* diag);


#endif // __diag_H__
