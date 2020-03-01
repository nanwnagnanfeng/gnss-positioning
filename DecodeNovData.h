#pragma once
#ifndef __DECODENOVDATA_H__
#define __DECODENOVDATA_H__
#include "TimeConvert.h"
#include "Serial.h"
#define MAXCHANNEL 32
struct ObsPRN
{
	unsigned short PRN;//����PRN��
	double psr;//α��
	float psr_std;//α���׼��
	double adr;//�ز���λ
	float adr_std;//�ز���λ��׼��
	float DopplerShift;//������Ƶ��
	float CNR;//�����

};
struct Obsdata  //�۲�����
{
	GPSTime ObsTime;

	long obsnum;
	ObsPRN obsPRN[MAXCHANNEL];
	//double ChannelState;//ͨ��״̬

};

struct GPSEphem //��������
{
	short PRN;//����PRN��
	unsigned int IODE1, IODE2, IODC;
	GPSTime TOE, TOC, T, TOW;
	double A, deltaN, M0, ecc, omega;
	double cuc, cus, crc, crs, cic, cis;
	double I0, I_o, omega0, omega_o, tgd;//I_oΪ�������ٶ�
	double af0, af1, af2, N, URA;
};

struct PositionRESULT //��λ���
{
	GPSTime PosTime;//��λʱ��

	double lat, lon, hgt;

	double sigma, pdop;
	float lat_std, lon_std, hgt_std, sol_age, diff_age;
	short solnSVs, TrackSVs;
};

struct IONO//��������
{
	double a[4];//alpha_parameter
	double b[4];//belta_parameter
	bool Valid;
	IONO()
	{
		Valid = false;
	}
};
int DecodeNovData(unsigned char buff[], GPSEphem Eph[], Obsdata *Obs, PositionRESULT *PosRes, IONO *Ion, int n, int *s);
double R8(unsigned char *p);
short I2(unsigned char *p);
unsigned short U2(unsigned char *p);
unsigned int U4(unsigned char *p);
float F4(unsigned char *p);
unsigned int crc32(const unsigned char *buff, int len);

#endif // !__READFILE_H__

