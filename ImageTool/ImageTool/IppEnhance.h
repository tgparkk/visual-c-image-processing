#pragma once // 이 헤더 파일이 오직 한 번만 포함되도록 설정

#include "IppImage/IppImage.h"

void IppInverse(IppByteImage& img);

void IppBrightness(IppByteImage& img, int n);

void IppContrast(IppByteImage& img, int n);