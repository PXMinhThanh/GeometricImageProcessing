#include "NearestNeighborInterpolate.h"

uchar NearestNeighborInterpolate::Interpolate(
	float tx, float ty, 
	uchar* pSrc, 
	int srcWidthStep, 
	int nChannels, 
	uchar* pDstRow)
{
	// Round to get nearest neighbour pixels
	int interpPosition = srcWidthStep * (int)round(tx) + nChannels * (int)round(ty);

	if (nChannels == 1)
	{
		// Grayscale image
		*pDstRow = pSrc[interpPosition];
	}
	else if (nChannels == 3)
	{
		// 3 channel color image
		pDstRow[0] = pSrc[interpPosition];
		pDstRow[1] = pSrc[interpPosition + 1];;
		pDstRow[2] = pSrc[interpPosition + 2];
	}

	return 1;
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{

}
NearestNeighborInterpolate::~NearestNeighborInterpolate()
{

}