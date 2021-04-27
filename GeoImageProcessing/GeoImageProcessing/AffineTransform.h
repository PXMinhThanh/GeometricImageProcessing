#pragma once
#include "Header.h"

/*
Lớp biểu diễn pháp biến đổi affine
*/
class AffineTransform
{
	Mat _matrixTransform;//ma trận 3x3 biểu diễn phép biến đổi affine
public:
	void Translate(float dx, float dy);// xây dựng matrix transform cho phép tịnh tiến theo vector (dx,dy)
	void Rotate(float angle);//xây dựng matrix transform cho phép xoay 1 góc angle
	void Scale(float sx, float sy);//xây dựng matrix transform cho phép tỉ lệ theo hệ số 	
	void Reflect(bool axis);//xây dụng matrix transform cho phép phản chiếu theo trục Ox (axis=0) hoặc trục Oy (axis=1)
	void TransformPoint(float& x, float& y);//transform 1 điểm (x,y) theo matrix transform đã có

	AffineTransform();
	~AffineTransform();
};