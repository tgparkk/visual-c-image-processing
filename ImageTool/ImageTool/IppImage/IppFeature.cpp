#include "pch.h"
#include "IppFeature.h"

void IppEdgeRoberts(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = p1[j][i] - p1[j - 1][i - 1];
			h2 = p1[j][i] - p1[j - 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}

void IppEdgePrewitt(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = -p1[j - 1][i - 1] - p1[j - 1][i] - p1[j - 1][i + 1]
				+ p1[j + 1][i - 1] + p1[j + 1][i] + p1[j + 1][i + 1];
			h2 = -p1[j - 1][i - 1] - p1[j][i - 1] - p1[j + 1][i - 1]
				+ p1[j - 1][i + 1] + p1[j][i + 1] + p1[j + 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}

void IppEdgeSobel(IppByteImage& img, IppByteImage& imgEdge)
{
	int w = img.GetWidth();
	int h = img.GetHeight();

	imgEdge.CreateImage(w, h);

	BYTE** p1 = img.GetPixels2D();
	BYTE** p2 = imgEdge.GetPixels2D();

	int i, j, h1, h2;
	double hval;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++)
		{
			h1 = -p1[j - 1][i - 1] - 2 * p1[j - 1][i] - p1[j - 1][i + 1]
				+ p1[j + 1][i - 1] + 2 * p1[j + 1][i] + p1[j + 1][i + 1];
			h2 = -p1[j - 1][i - 1] - 2 * p1[j][i - 1] - p1[j + 1][i - 1]
				+ p1[j - 1][i + 1] + 2 * p1[j][i + 1] + p1[j + 1][i + 1];

			hval = sqrt(static_cast<double>(h1 * h1 + h2 * h2));

			p2[j][i] = static_cast<BYTE>(limit(hval + 0.5));
		}
}