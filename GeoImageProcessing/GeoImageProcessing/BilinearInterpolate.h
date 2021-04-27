#pragma once
#include "PixelInterpolate.h"

/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
class BilinearInterpolate : public PixelInterpolate
{
public:
	uchar Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, uchar* pDstRow);
	BilinearInterpolate();
	~BilinearInterpolate();
};