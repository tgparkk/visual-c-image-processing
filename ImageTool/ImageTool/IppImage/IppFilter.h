#pragma once

#include "IppImage.h"

void IppFilterMean(IppByteImage& imgSrc, IppByteImage& imgDst);

void IppFilterWeightedMean(IppByteImage& imgSrc, IppByteImage& imgDst);

void IppFilterGaussian(IppByteImage& imgSrc, IppFloatImage& imgDst, float sigma);