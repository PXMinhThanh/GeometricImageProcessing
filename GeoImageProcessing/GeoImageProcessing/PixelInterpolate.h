#pragma once
#include "Header.h"

/*
 Lớp base dùng để nội suy màu của 1 pixel
*/
class PixelInterpolate
{
public:
	/*
	Hàm tính giá trị màu của ảnh kết quả từ nội suy màu trong ảnh gốc
	Tham số
		- (tx,ty): tọa độ thực của ảnh gốc sau khi thực hiện phép biến đổi affine
		- pSrc: con trỏ ảnh gốc
		- srcWidthStep: widthstep của ảnh gốc
		- nChannels: số kênh màu của ảnh gốc
		- pDstRow: con trỏ pixel đang nội suy của ảnh kết quả
	Trả về
		- Giá trị màu được nội suy
	*/
	virtual uchar Interpolate(
		float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, uchar* pDstRow) = 0;
	PixelInterpolate();
	~PixelInterpolate();
};