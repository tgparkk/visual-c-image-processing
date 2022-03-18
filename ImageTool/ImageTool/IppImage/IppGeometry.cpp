#include "pch.h"
#include "IppGeometry.h"

#include <math.h>

const double PI = 3.14159265358979323846;

void IppTranslate(IppByteImage& imgSrc, IppByteImage& imgDst, int sx, int sy)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(w, h);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			x = i - sx;
			y = j - sy;
			if (x >= 0 && x < w && y >= 0 && y < h)
				pDst[j][i] = pSrc[y][x];
		}
}

void IppResizeNearest(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x, y;
	double rx, ry;
	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1);
			ry = static_cast<double>(h - 1) * j / (nh - 1);
			x = static_cast<int>(rx + 0.5);
			y = static_cast<int>(ry + 0.5);

			if (x >= w) x = w - 1;
			if (y >= h) y = h - 1;

			pDst[j][i] = pSrc[y][x];
		}
}

void IppResizeBilinear(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, y1, x2, y2;
	double rx, ry, p, q, value;

	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1);
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x1 = static_cast<int>(rx);
			y1 = static_cast<int>(ry);

			x2 = x1 + 1; 
			if (x2 == w) x2 = w - 1;
			y2 = y1 + 1; 
			if (y2 == h) y2 = h - 1;

			p = rx - x1;
			q = ry - y1;

			value = (1. - p) * (1. - q) * pSrc[y1][x1]
				+ p * (1. - q) * pSrc[y1][x2]
				+ (1. - p) * q * pSrc[y2][x1]
				+ p * q * pSrc[y2][x2];

			pDst[j][i] = static_cast<BYTE>(limit(value + .5));
		}
}

void IppResizeCubic(IppByteImage& imgSrc, IppByteImage& imgDst, int nw, int nh)
{
	int w = imgSrc.GetWidth();
	int h = imgSrc.GetHeight();

	imgDst.CreateImage(nw, nh);

	BYTE** pSrc = imgSrc.GetPixels2D();
	BYTE** pDst = imgDst.GetPixels2D();

	int i, j, x1, x2, x3, x4, y1, y2, y3, y4;
	double v1, v2, v3, v4, v;
	double rx, ry, p, q;

	for (j = 0; j < nh; j++)
		for (i = 0; i < nw; i++)
		{
			rx = static_cast<double>(w - 1) * i / (nw - 1);
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x2 = static_cast<int>(rx);
			x1 = x2 - 1; if (x1 < 0) x1 = 0;
			x3 = x2 + 1; if (x3 >= w) x3 = w - 1;
			x4 = x2 + 2; if (x4 >= w) x4 = w - 1;
			p = rx - x2;

			y2 = static_cast<int>(ry);
			y1 = y2 - 1; if (y1 < 0) y1 = 0;
			y3 = y2 + 1; if (y3 >= h) y3 = h - 1;
			y4 = y2 + 2; if (y4 >= h) y4 = h - 1;
			q = ry - y2;

			v1 = cubic_interpolation(pSrc[y1][x1], pSrc[y1][x2], pSrc[y1][x3], pSrc[y1][x4], p);
			v2 = cubic_interpolation(pSrc[y2][x1], pSrc[y2][x2], pSrc[y2][x3], pSrc[y2][x4], p);
			v3 = cubic_interpolation(pSrc[y3][x1], pSrc[y3][x2], pSrc[y3][x3], pSrc[y3][x4], p);
			v4 = cubic_interpolation(pSrc[y4][x1], pSrc[y4][x2], pSrc[y4][x3], pSrc[y4][x4], p);

			v = cubic_interpolation(v1, v2, v3, v4, q);

			pDst[j][i] = static_cast<BYTE>(limit(v + 0.5));
		}
}

double cubic_interpolation(double v1, double v2, double v3, double v4, double d)
{
	double v, p1, p2, p3, p4;

	p1 = 2 * v2;
	p2 = -v1 + v3;
	p3 = 2 * v1 - 5 * v2 + 4 * v3 - v4;
	p4 = -v1 + 3 * v2 - 3 * v3 + v4;

	v = (p1 + d * (p2 + d * (p3 + d * p4))) / 2.;

	return v;
}