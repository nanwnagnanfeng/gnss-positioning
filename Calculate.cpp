#include "stdafx.h"
#include "math.h"
#include "Calculate.h"
#include "DecodeNovData.h"
#include "CordConvert.h"
#include "Matrix.h"
/*
����ֵ��
1��δ������������
2����������
*/
int SatPosVelCal(GPSEphem Eph[], XYZ SatPos[], Velocity Vsat[], Obsdata *obs, int i)
{
	if (Eph[i].PRN < 0 || Eph[i].PRN > 32)
	{
		return 1;
	}
	if (obs->obsPRN[i].PRN < 0 || obs->obsPRN[i].PRN > 32)
	{
		return 1;
	}
	int j = 0;
	double n0, n;//ƽ���˶����ٶ�(������)
	double Tk;//��������ο���Ԫʱ��
	GPSTime t;//�źŷ���ʱ��
	double Mk;//ƽ�����
	double Ek, E0;//ƫ�����
	double Vk;//������
	double faik, duk, uk;//(������)�����Ǿ�
	double rk0, drk, rk;//(������)��
	double ik0, dik, ik;//(������)������
	double xk, yk;//�����ڹ��ƽ���λ��
	double omegak;//�����������㾭��

	double F;
	double dT;

	double Ek1;//ƫ�����Ekһ�׵���
	double faik1, uk1;//�����Ǿ�һ�׵���
	double rk1;//��һ�׵���
	double ik1;//������һ�׵���
	double omegak1;//�����㾭��һ�׵���
	double R[12];//ת������
	double M[4];//�������
	double V[3];//�ٶȾ���
	double xk1, yk1;//�����λ�����ٶ�
	double dtSV = 0;//�����Ӳ��ֵ�������ε�������
	double dtr_o, dtSV_o;//���ڼ�������

	double rE;//������ת�Ƕ�
	double A[3], B[3], C[9];//���ڵ�����ת��������
	while (j<2)
	{

		//����ƽ���˶����ٶ�
		n0 = sqrt(miu / (Eph[i].A*Eph[i].A*Eph[i].A));


		//��������������ο���Ԫ��ʱ��
		t.SecOfWeek = obs->ObsTime.SecOfWeek - obs->obsPRN[i].psr / c - dtSV;
		t.Week = obs->ObsTime.Week;
		while (t.SecOfWeek < 0)
		{
			t.SecOfWeek += 604800;
			t.Week = t.Week - 1;
			if (t.SecOfWeek < -1e-10)
			{
				break;
			}
		}
		Tk = t.SecOfWeek - Eph[i].TOE.SecOfWeek + 604800 * (t.Week - Eph[i].TOE.Week);  /*  ǰ��Ϊ�źŷ���ʱ��  */
																						/*if (fabs(Tk) > 7200)//�����ʱ�����2h����������
																						{
																						return 2;
																						}*/
																						//��ƽ���˶����ٶȽ��и���
		n = n0 + Eph[i].deltaN;

		//����ƽ����ǡ�ƫ����ǡ�������
		Mk = Eph[i].M0 + n*Tk;

		E0 = Mk;
		while (j<10000)
		{
			Ek = Mk + Eph[i].ecc*sin(E0);
			if (fabs(Ek - E0) <= 1.0e-15)
			{
				break;
			}
			E0 = Ek;
			j++;
		}

		Vk = atan2(sqrt(1 - Eph[i].ecc*Eph[i].ecc)*sin(Ek) / (1 - Eph[i].ecc*cos(Ek)), (cos(Ek) - Eph[i].ecc) / (1 - Eph[i].ecc*cos(Ek)));
		//���������Ǿ�
		faik = Vk + Eph[i].omega;
		//������׵��͸�����
		duk = Eph[i].cus*sin(2 * faik) + Eph[i].cuc*cos(2 * faik);
		drk = Eph[i].crs*sin(2 * faik) + Eph[i].crc*cos(2 * faik);
		dik = Eph[i].cis*sin(2 * faik) + Eph[i].cic*cos(2 * faik);
		//���������������Ǿࡢ�򾶡�������
		uk = faik + duk;
		rk = Eph[i].A*(1 - Eph[i].ecc*cos(Ek)) + drk;
		ik = Eph[i].I0 + dik + Eph[i].I_o*Tk;

		//���������ڹ��ƽ���ϵ�λ��
		xk = rk*cos(uk);
		yk = rk*sin(uk);
		//���������������㾭��
		omegak = Eph[i].omega0 + (Eph[i].omega_o - omegaE)*Tk - omegaE*Eph[i].TOE.SecOfWeek;
		//���������ڵع�����ϵ�µ�λ��
		SatPos[i].x = xk*cos(omegak) - yk*cos(ik)*sin(omegak);
		SatPos[i].y = xk*sin(omegak) + yk*cos(ik)*cos(omegak);
		SatPos[i].z = yk*sin(ik);
		//������λ�ý��е�����ת����
		rE = obs->obsPRN[i].psr / c * omegaE;
		C[0] = cos(rE);
		C[1] = sin(rE);
		C[2] = 0;
		C[3] = -sin(rE);
		C[4] = cos(rE);
		C[5] = 0;
		C[6] = 0;
		C[7] = 0;
		C[8] = 1;
		A[0] = SatPos[i].x;
		A[1] = SatPos[i].y;
		A[2] = SatPos[i].z;
		matrix_mult(3, 3, 3, 1, C, A, B);
		SatPos[i].x = B[0];
		SatPos[i].y = B[1];
		SatPos[i].z = B[2];
		//**********�����Ӳ�************//
		F = -2 * sqrt(miu) / (c*c);
		dT = F * Eph[i].ecc*sqrt(Eph[i].A)*sin(Ek);
		dtSV = Eph[i].af0 + Eph[i].af1*(t.SecOfWeek - Eph[i].TOC.SecOfWeek + 604800 * (t.Week - Eph[i].TOC.Week)) + Eph[i].af2
			*(t.SecOfWeek - Eph[i].TOC.SecOfWeek + 604800 * (t.Week - Eph[i].TOC.Week))*(t.SecOfWeek - Eph[i].TOC.SecOfWeek + 604800 * (t.Week - Eph[i].TOC.Week)) + dT - Eph[i].tgd;
		//Vsat[i].deltaTsv = Eph[i].af0 + Eph[i].af1*(t.SecOfWeek - Eph[i].TOC.SecOfWeek) + Eph[i].af2*(pow((t.SecOfWeek - Eph[i].TOC.SecOfWeek), 2)) + dT - Eph[i].tgd;
		Vsat[i].deltaTsv = dtSV;
		//if (fabs(Vsat[i].deltaTsv) > 1)//�������в��ֳ��ֵõ����Ӳ���󣬽�����Ľ������
		//{
		//	Vsat[i].deltaTsv = 0;
		//}

		//**********���������ٶ�***********//
		//����Ek����
		Ek1 = Eph[i].N / (1 - Eph[i].ecc*cos(Ek));
		//����uk����
		faik1 = sqrt((1 + Eph[i].ecc) / (1 - Eph[i].ecc))*cos(Vk / 2)*cos(Vk / 2)*Ek1 / (cos(Ek / 2)*cos(Ek / 2));
		uk1 = 2 * (Eph[i].cus*cos(2 * faik) - Eph[i].cuc*sin(2 * faik))*faik1 + faik1;
		//����rk����
		rk1 = Eph[i].A*Eph[i].ecc*sin(Ek)*Ek1 + 2 * (Eph[i].crs*cos(2 * faik) - Eph[i].crc*sin(2 * faik))*faik1;
		//����ik����
		ik1 = Eph[i].I_o + 2 * (Eph[i].cis*cos(2 * faik) - Eph[i].cic*sin(2 * faik))*faik1;
		//����ŷ��٤k����
		omegak1 = Eph[i].omega_o - omegaE;
		//����ת������R
		R[0] = cos(omegak);
		R[1] = -sin(omegak)*cos(ik);
		R[2] = -(xk*sin(omegak) + yk*cos(omegak)*cos(ik));
		R[3] = yk*sin(omegak)*sin(ik);
		R[4] = sin(omegak);
		R[5] = cos(omegak)*cos(ik);
		R[6] = xk*cos(omegak) - yk*sin(omegak)*cos(ik);
		R[7] = yk*cos(omegak)*sin(ik);
		R[8] = 0;
		R[9] = sin(ik);
		R[10] = 0;
		R[11] = yk*cos(ik);
		//��������
		xk1 = rk1*cos(uk) - rk*uk1*sin(uk);
		yk1 = rk1*sin(uk) + rk*uk1*cos(uk);
		M[0] = xk1;
		M[1] = yk1;
		M[2] = omegak1;
		M[3] = ik1;
		//������˵õ������ٶ�
		matrix_mult(3, 4, 4, 1, R, M, V);
		Vsat[i].vx = V[0];
		Vsat[i].vy = V[1];
		Vsat[i].vz = V[2];
		//�����ٶȵ�����ת����
		A[0] = Vsat[i].vx;
		A[1] = Vsat[i].vy;
		A[2] = Vsat[i].vz;
		matrix_mult(3, 3, 3, 1, C, A, B);
		Vsat[i].vx = B[0];
		Vsat[i].vy = B[1];
		Vsat[i].vz = B[2];

		//�������ټ���
		dtr_o = F * Eph[i].ecc*sqrt(Eph[i].A)*cos(Ek)*Ek1;
		dtSV_o = Eph[i].af1 + 2 * Eph[i].af2*(t.SecOfWeek - Eph[i].TOC.SecOfWeek + 604800 * (t.Week - Eph[i].TOC.Week)) + dtr_o;
		Vsat[i].clkvel = dtSV_o;

		j++;


	}


	return 0;
}


