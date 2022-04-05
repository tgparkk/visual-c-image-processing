#pragma once

#include <vector>
#include "IppImage.h"
#include "IppFeature.h"

void IppBinarization(IppByteImage& imgSrc, IppByteImage& imgDst, int threshold);