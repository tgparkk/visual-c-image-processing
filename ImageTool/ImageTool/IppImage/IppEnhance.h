#pragma once // �� ��� ������ ���� �� ���� ���Եǵ��� ����

#include "IppImage.h"

void IppInverse(IppByteImage& img);

void IppBrightness(IppByteImage& img, int n);

void IppContrast(IppByteImage& img, int n);

void IppGammaCorrection(IppByteImage& img, float gamma);

void IppHistogram(IppByteImage& img, float histo[256]);

void IppHistogramStretching(IppByteImage& img);

void IppHistogramEqualization(IppByteImage& img);

bool IppAdd(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

bool IppSub(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

bool IppAve(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);

bool IppDiff(IppByteImage& img1, IppByteImage& img2, IppByteImage& img3);