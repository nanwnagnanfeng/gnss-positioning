#pragma once
#include "TimeConvert.h"
#include "CordConvert.h"
#include "DecodeNovData.h"
#ifndef __ERRORCORRECT_H__
#define __ERRORCORRECT_H__
#define H0 0.0 //m
#define T0 15.0  //�¶ȣ����ϣ�
#define K0 288.15 //�¶ȣ������¶ȣ�
#define p0 1013.25 //��ѹ��mbar��
#define RH0 0.5//���ʪ��


struct Obspospara
{
	//double H;//��վ�߶�
	double T;//��վ����
	double p;//��վ��ѹ
	double RH;//��վ���ʪ��
};
double Klobutchar(IONO *ionodata, BLH *Obspos, GPSTime *t, double eleang, double aziang);
double Hopfield(double H, double eleang);


#endif