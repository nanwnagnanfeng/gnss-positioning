#pragma once
#ifndef __RTCMDECODE_H__
#define __RTCMDECODE_H__
#include "TimeConvert.h"
#define MAXLEN 8192
#define MAXCHANNEL 32

struct RTCMHEAD
{
	int preamble;//������
	float REFSTN_ID;//��վID
	unsigned short MSGType;
	double modZ;//����Z����
	int SeqNo;//���к�
	unsigned short LEN;//֡��
	unsigned short STNhealth;//��׼վ����״̬
	unsigned char SN;
};
struct RTCMSG1
{
	unsigned short S;//Scale
	unsigned short PRN;
	unsigned short UDRE;
	double PRC;//α�������
	double RRC;//α��������仯��
	double IOD;//��ǰ��������
};
struct RTCMSG3
{
	double ECEF_x;
	double ECEF_y;
	double ECEF_z;

};
struct RTCMRES
{
	GPSTime t;
	RTCMHEAD head;
	RTCMSG1 msg1[MAXCHANNEL];
	RTCMSG3 msg3;
};
struct RTCMBUFF
{
	unsigned char buff2[MAXLEN];
	int buff_position;//����λ��
	int buff_len;//���ݳ�
};


int DecodeRTCMData(unsigned char *buff2, RTCMRES *rtcmresult, int len);
int PreambleSearch(unsigned char raw[], unsigned char word[], RTCMBUFF *rtcmdata, unsigned char *D29, unsigned char *D30, unsigned char W[]);
void FormatRotToWords(unsigned char *rot, unsigned char *word);
unsigned char RTCMroll(unsigned char raw);
bool RTCMParity(unsigned char w[], unsigned char D29, unsigned char D30);
bool GetRTCMHeadAndResultFromBuff(unsigned char W[], unsigned char *D29, unsigned char *D30, RTCMRES *rtcmres);
#endif