#pragma once

#include "IppImage.h"

void DFT1d(double* re, double* im, int N, int dir);
void FFT1d(double* re, double* im, int N, int dir);
bool IsPowerOf2(int n);

class IppFourier
{
public:
	int width;
	int height;
	IppDoubleImage real; // �Ǽ���
	IppDoubleImage imag; // �����

public:
	IppFourier();

	void SetImage(IppByteImage& img);
	void GetImage(IppByteImage& img);
	void GetSpectrumImage(IppByteImage& img);
	void GetPhaseImage(IppByteImage& img);

	// ������ Ǫ���� ��ȯ �Լ�
	void DFT(int dir);
	void DFTRC(int dir);
	void FFT(int dir);

	// ���ļ� ���������� ���͸� �Լ�
	void LowPassIdeal(int cutoff);
	void HighPassIdeal(int cutoff);
	void LowPassGaussian(int cutoff);
	void HighPassGaussian(int cutoff);
};
