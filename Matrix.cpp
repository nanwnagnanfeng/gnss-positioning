/**************��������***************/
#include "stdafx.h"
#include "math.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <float.h>
//����ӷ�
void matrix_add(int row, int line, double matA[], double matB[], double matC[])
{
	int i, j;
	for (i = 0;i <row;i++)
	{
		for (j = 0;j <line;j++)
		{
			matC[row*i + j] = matA[row*i + j] + matB[row*i + j];
		}
	}
}
//�������
void matrix_minus(int row, int line, double matA[], double matB[], double matC[])
{
	int i, j;
	for (i = 0;i < row;i++)
	{
		for (j = 0;j < line;j++)
		{
			matC[row*i + j] = matA[row*i + j] - matB[row*i + j];
		}
	}
}
//����˷�
void matrix_mult(int row1, int line1, int row2, int line2, double matA[], double matB[], double matC[])
{
	int i, j, k;
	if (line1 != row2)
	{
		printf("��������ܽ��г˷����㡣\n");
		return;
	}
	double temp;
	for (i = 0;i <row1;i++)
	{
		for (j = 0;j < line2;j++)
		{
			temp = 0.0;
			for (k = 0;k < row2;k++)
			{
				temp += matA[line1*i + k] * matB[line2*k + j];
			}
			matC[line2*i + j] = temp;
		}
	}
}
//����ת��
void matrix_trans(int row, int line, double matA[], double matB[])
//matAΪԭʼ����matBΪת�ú����
{
	int i, j;
	for (i = 0;i < row;i++)
	{
		for (j = 0;j < line;j++)
		{
			matB[row*j + i] = matA[line*i + j];
		}
	}
}
//��������
int MatrixInv(int n, double a[], double b[])
{
	int i, j, k, l, u, v, is[10], js[10];   /* matrix dimension <= 10 */
	double d, p;

	if (n <= 0)
	{
		printf("Error dimension in MatrixInv!\n");
		exit(EXIT_FAILURE);
	}

	/* ���������ֵ���������b�������b�������棬a���󲻱� */
	for (i = 0;i<n;i++)
	{
		for (j = 0;j<n;j++)
		{
			b[i*n + j] = a[i*n + j];
		}
	}

	for (k = 0;k<n;k++)
	{
		d = 0.0;
		for (i = k;i<n;i++)   /* �������½Ƿ�������Ԫ�ص�λ�� */
		{
			for (j = k;j<n;j++)
			{
				l = n*i + j;
				p = fabs(b[l]);
				if (p>d)
				{
					d = p;
					is[k] = i;
					js[k] = j;
				}
			}
		}

		if (d<DBL_EPSILON)   /* ��Ԫ�ؽӽ���0�����󲻿��� */
		{
			printf("Divided by 0 in MatrixInv!\n");
			return 0;
		}

		if (is[k] != k)  /* ����Ԫ�����ڵ��������½Ƿ�������н��е��� */
		{
			for (j = 0;j<n;j++)
			{
				u = k*n + j;
				v = is[k] * n + j;
				p = b[u];
				b[u] = b[v];
				b[v] = p;
			}
		}

		if (js[k] != k)  /* ����Ԫ�����ڵ��������½Ƿ�������н��е��� */
		{
			for (i = 0; i<n; i++)
			{
				u = i*n + k;
				v = i*n + js[k];
				p = b[u];
				b[u] = b[v];
				b[v] = p;
			}
		}

		l = k*n + k;
		b[l] = 1.0 / b[l];  /* �����б任 */
		for (j = 0; j<n; j++)
		{
			if (j != k)
			{
				u = k*n + j;
				b[u] = b[u] * b[l];
			}
		}
		for (i = 0;i<n; i++)
		{
			if (i != k)
			{
				for (j = 0; j<n; j++)
				{
					if (j != k)
					{
						u = i*n + j;
						b[u] = b[u] - b[i*n + k] * b[k*n + j];
					}
				}
			}
		}
		for (i = 0;i<n;i++)
		{
			if (i != k)
			{
				u = i*n + k;
				b[u] = -b[u] * b[l];
			}
		}
	}

	for (k = n - 1;k >= 0;k--)  /* ����������е������»ָ� */
	{
		if (js[k] != k)
		{
			for (j = 0;j<n;j++)
			{
				u = k*n + j;
				v = js[k] * n + j;
				p = b[u];
				b[u] = b[v];
				b[v] = p;
			}
		}
		if (is[k] != k)
		{
			for (i = 0;i<n;i++)
			{
				u = i*n + k;
				v = is[k] + i*n;
				p = b[u];
				b[u] = b[v];
				b[v] = p;
			}
		}
	}

	return (1);
}
void vector_pointmult(int m, double vecA[], double vecB[], double vecC[])
{
	int i;
	for (i = 0;i < m;i++)
	{
		vecC[i] = vecA[i] * vecB[i];
	}

}
void vector_crossmult(double vecA[], double vecB[], double vecC[])
{
	vecC[0] = vecA[1] * vecB[2] - vecB[1] * vecA[2];
	vecC[1] = vecA[2] * vecB[0] - vecA[0] * vecB[2];
	vecC[2] = vecA[0] * vecB[1] - vecB[0] * vecA[1];

}