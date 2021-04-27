#include "AffineTransform.h"

// xây dựng matrix transform cho phép tịnh tiến theo vector (dx,dy)
void AffineTransform::Translate(float dx, float dy)
{
	Mat _matrixTranslate = Mat::eye(3, 3, CV_32FC1);
	_matrixTranslate.at<float>(0, 2) = dx;
	_matrixTranslate.at<float>(1, 2) = dy;
	_matrixTransform = _matrixTranslate * _matrixTransform;
}

//xây dựng matrix transform cho phép xoay 1 góc angle
void AffineTransform::Rotate(float angle)
{
	Mat _matrixRotate = Mat::eye(3, 3, CV_32FC1);
	_matrixRotate.at<float>(0, 0) = cos(angle);
	_matrixRotate.at<float>(0, 1) = -sin(angle);
	_matrixRotate.at<float>(1, 0) = sin(angle);
	_matrixRotate.at<float>(1, 1) = cos(angle);
	_matrixTransform = _matrixRotate * _matrixTransform;
}

//xây dựng matrix transform cho phép tỉ lệ theo hệ số 	
void AffineTransform::Scale(float sx, float sy)
{
	Mat _matrixScale = Mat::eye(3, 3, CV_32FC1);
	_matrixScale.at<float>(0, 0) = sx;
	_matrixScale.at<float>(1, 1) = sy;
	_matrixTransform = _matrixScale * _matrixTransform;
}

//xây dụng matrix transform cho phép phản chiếu theo: 
//	trục Ox (axis=0) 
//	trục Oy (axis=1)
void AffineTransform::Reflect(bool axis)
{
	Mat _matrixReflect = Mat::eye(3, 3, CV_32FC1);
	if (axis == 0)
		_matrixReflect.at<float>(1, 1) = -1;
	else
		_matrixReflect.at<float>(0, 0) = -1;
	_matrixTransform = _matrixReflect * _matrixTransform;
}

//transform 1 điểm (x,y) theo matrix transform đã có
void AffineTransform::TransformPoint(float& x, float& y)
{
	Mat _srcPoint = Mat(3, 1, CV_32FC1);
	_srcPoint.at<float>(0, 0) = x;
	_srcPoint.at<float>(1, 0) = y;
	_srcPoint.at<float>(2, 0) = 1;
	Mat _dstPoint = _matrixTransform * _srcPoint;
	x = _dstPoint.at<float>(0, 0);
	y = _dstPoint.at<float>(1, 0);
}

AffineTransform::AffineTransform()
{
	_matrixTransform = Mat::eye(3, 3, CV_32FC1);
}

AffineTransform::~AffineTransform()
{

}