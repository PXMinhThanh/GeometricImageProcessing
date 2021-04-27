#include "BilinearInterpolate.h"

uchar BilinearInterpolate::Interpolate(
	float tx, float ty, 
	uchar* pSrc, 
	int srcWidthStep, 
	int nChannels, 
	uchar* pDstRow)
{
	// Round to get nearest neighbour pixels
	float x = fabs(tx);
	float y = fabs(ty);
	float dx = fabs(x - round(x));
	float dy = fabs(y - round(y));
	int interTopLeft = srcWidthStep * (int)floor(x) + nChannels * (int)floor(y);
	int interTopRight = srcWidthStep * (int)floor(x) + nChannels * (int)ceil(y);
	int interBottomLeft = srcWidthStep * (int)ceil(x) + nChannels * (int)floor(y);
	int interBottomRight = srcWidthStep * (int)ceil(x) + nChannels * (int)ceil(y);
	
	if (nChannels == 1)
	{
		// Grayscale image
		uchar interColor =  
			pSrc[interTopLeft] * (1 - dx) * (1 - dy) + 
			pSrc[interTopRight] * (1 - dx) * dy +
			pSrc[interBottomLeft] * dx * (1 - dy) +
			pSrc[interBottomRight] * dx * dy;
		*pDstRow = interColor;
	}
	else if (nChannels == 3)
	{
		// 3 channel color image
		// Blue channel
		uchar interColorBlue =
			pSrc[interTopLeft] * (1 - dx) * (1 - dy) +
			pSrc[interTopRight] * (1 - dx) * dy +
			pSrc[interBottomLeft] * dx * (1 - dy) +
			pSrc[interBottomRight] * dx * dy;
		pDstRow[0] = interColorBlue;

		// Green channel
		uchar interColorGreen =
			pSrc[interTopLeft + 1] * (1 - dx) * (1 - dy) +
			pSrc[interTopRight + 1] * (1 - dx) * dy +
			pSrc[interBottomLeft + 1] * dx * (1 - dy) +
			pSrc[interBottomRight + 1] * dx * dy;
		pDstRow[1] = interColorGreen;
		
		// Red channel
		uchar interColorRed =
			pSrc[interTopLeft + 2] * (1 - dx) * (1 - dy) +
			pSrc[interTopRight + 2] * (1 - dx) * dy +
			pSrc[interBottomLeft + 2] * dx * (1 - dy) +
			pSrc[interBottomRight + 2] * dx * dy;
		pDstRow[2] = interColorRed;
	}

	return 1;
}

BilinearInterpolate::BilinearInterpolate()
{

}

BilinearInterpolate::~BilinearInterpolate()
{

}