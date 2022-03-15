#include "pch.h"
#include "IppFilter.h"

const float  PI_F = 3.14159265358979323846f;

void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{ 1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 1 },
	};

	int i, j, m, n, sum;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			sum = 0;
			for (m = 0; m < 3; m++)
				for (n = 0; n < 3; n++)
				{
					sum += (pSrc[j - 1 + m][i - 1 + n] * mask[m][n]);
				}

			pDst[j][i] = static_cast<BYTE>(limit(sum / 9. + 0.5));
		}
}

void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst = imgSrc;

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int mask[3][3] = {
		{ 1, 2, 1 },
		{ 2, 4, 2 },
		{ 1, 2, 1 },
	};

	int i, j, m, n, sum;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			sum = 0;
			for (m = 0; m < 3; m++)
				for (n = 0; n < 3; n++)
				{
					sum += (pSrc[j - 1 + m][i - 1 + n] * mask[m][n]);
				}

			pDst[j][i] = static_cast<BYTE>(limit(sum / 16. + 0.5));
		}
}

void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma)
{
	register int i, j, k, x;

	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h); // output

	BYTE** pSrc = imgSrc.GetPixels2D();
	float** pDst = imgDst.GetPixels2D(); // output

	//-------------------------------------------------------------------------
	// 1차원 가우시안 마스크 & 실수 연산을 위한 버퍼 이미지 생성
	//-------------------------------------------------------------------------

	int dim = static_cast<int>(2 * 4 * sigma + 1.0);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	int dim2 = dim / 2;

	IppFloatImage imgMask(dim, 1);
	float* pMask = imgMask.GetPixels();

	for (i = 0; i < dim; i++)
	{
		x = i - dim2;
		pMask[i] = exp(-(x * x) / (2 * sigma * sigma)) / (sqrt(2 * PI_F) * sigma);
	}

	IppFloatImage imgBuf(w, h);
	float** pBuf = imgBuf.GetPixels2D();

	//-------------------------------------------------------------------------
	// 세로 방향 마스크 연산
	//-------------------------------------------------------------------------

	float sum1, sum2;
	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++)
		{
			sum1 = sum2 = 0.f;

			for (k = 0; k < dim; k++)
			{
				x = k - dim2 + j;

				if (x >= 0 && x < h)
				{
					sum1 += pMask[k];
					sum2 += (pMask[k] * pSrc[x][i]);
				}
			}

			pBuf[j][i] = sum2 / sum1;
		}

	//-------------------------------------------------------------------------
	// 가로 방향 마스크 연산
	//-------------------------------------------------------------------------

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			sum1 = sum2 = 0.f;

			for (k = 0; k < dim; k++)
			{
				x = k - dim2 + i;

				if (x >= 0 && x < w)
				{
					sum1 += pMask[k];
					sum2 += (pMask[k] * pBuf[j][x]);
				}
			}

			pDst[j][i] = sum2 / sum1;
		}
}