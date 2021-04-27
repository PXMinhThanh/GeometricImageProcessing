#include "GeometricTransformer.h"

int GeometricTransformer::Transform(
	const Mat& beforeImage,
	Mat& afterImage,
	AffineTransform* transformer,
	PixelInterpolate* interpolator)
{
	// The height and width of images
	int width = afterImage.cols, height = afterImage.rows;

	// widthStep is byte distance between 2 consecutive pixels locate on the same columns
	int widthStep = afterImage.step[0];

	// nChannels is the number of channels
	int nChannels = afterImage.step[1];

	// pData is the pointer to the first element of image
	uchar* pData_aftImg = (uchar*)afterImage.data;
	uchar* pData_bfrImg = (uchar*)beforeImage.data;

	// Init access point
	Point2f p;
	for (int h = 0; h < height; h++, pData_aftImg += widthStep)
	{
		// Get the pointer to the first element of each row
		uchar* pRow_aftImg = pData_aftImg;

		for (int w = 0; w < width; w++, pRow_aftImg += nChannels)
		{
			// Transform each pixel
			p = Point2f(h, w);
			transformer->TransformPoint(p.x, p.y);

			// Check if the transformation point is outside the image
			if (!this->IsInside((int)round(p.x), (int)round(p.y), beforeImage))
				continue;

			// Pixel interpolation
			interpolator->Interpolate(p.x, p.y, pData_bfrImg, beforeImage.step[0], beforeImage.step[1], pRow_aftImg);
		}
	}
	return 1;
}

int GeometricTransformer::RotateKeepImage(
	const Mat& srcImage, 
	Mat& dstImage, 
	float angle, 
	PixelInterpolate* interpolator)
{
	// Check if source image is empty
	if (srcImage.empty()) return 0;
	else {
		// The height and width of images
		int oldHeight = srcImage.rows, oldWidth = srcImage.cols;
		int newHeight = -1, newWidth = -1;

		// Calculate new height and width of destination image
		this->CalcSizeRotatedRect(oldHeight, oldWidth, newHeight, newWidth, angle);
			
		// Initialize destination image
		dstImage = Mat(newHeight, newWidth, srcImage.type(), Scalar(0));

		// Affine inverse transformation: tranform Ohw space to Oxy space
		AffineTransform affInverseRotateK;
		Point pOldCenter = Point((oldHeight - 1) / 2, (oldWidth - 1) / 2);
		Point pNewCenter = Point((newHeight - 1) / 2, (newWidth - 1) / 2);
		affInverseRotateK.Translate(0 - pNewCenter.x, 0 - pNewCenter.y); // Translate new center point to O(0,0)
		affInverseRotateK.Rotate(-angle); // Rotate opposite angle
		affInverseRotateK.Translate(pOldCenter.x, pOldCenter.y); // Translate old center point back

		// Apply Affine inverse transformation
		this->Transform(srcImage, dstImage, &affInverseRotateK, interpolator);

		return 1;
	}

}

int GeometricTransformer::RotateUnkeepImage(
	const Mat& srcImage, 
	Mat& dstImage, 
	float angle, 
	PixelInterpolate* interpolator)
{
	// Check if source image is empty
	if (srcImage.empty()) return 0;
	else {
		// The height and width of images
		int height = srcImage.rows, width = srcImage.cols;

		// Initialize destination image
		dstImage = Mat(height, width, srcImage.type(), Scalar(0));

		// Affine inverse transformation: tranform Ohw space to Oxy space
		AffineTransform affInverseRotateUK;
		Point pCenter = Point((height - 1) / 2, (width - 1) / 2);
		affInverseRotateUK.Translate(0 - pCenter.x, 0 - pCenter.y); // Translate center point to O(0,0)
		affInverseRotateUK.Rotate(-angle); // Rotate opposite angle
		affInverseRotateUK.Translate(pCenter.x, pCenter.y); // Translate center point back

		// Apply Affine inverse transformation
		this->Transform(srcImage, dstImage, &affInverseRotateUK, interpolator);

		return 1;
	}

}

int GeometricTransformer::Scale(
	const Mat& srcImage,
	Mat& dstImage,
	float sx, float sy,
	PixelInterpolate* interpolator)
{
	// Check if source image is empty
	if (srcImage.empty()) return 0;
	else {
		// The height and width of images
		int height = srcImage.rows, width = srcImage.cols;

		// Initialize destination image
		dstImage = Mat((int)(height * sy), (int)(width * sx), srcImage.type(), Scalar(0));

		// Affine inverse transformation: tranform Ohw space to Oxy space
		AffineTransform affInverseScale;
		affInverseScale.Scale(1 / sy, 1 / sx); // Reverse scaling

		// Apply Affine inverse transformation
		this->Transform(srcImage, dstImage, &affInverseScale, interpolator);

		return 1;
	}
}

int GeometricTransformer::Resize(
	const Mat& srcImage,
	Mat& dstImage,
	int newWidth, int newHeight,
	PixelInterpolate* interpolator)
{
	// Check if source image is empty
	if (srcImage.empty()) return 0;
	else {
		// The height and width of images
		int oldHeight = srcImage.rows, oldWidth = srcImage.cols;

		// Initialize destination image
		dstImage = Mat(newHeight, newWidth, srcImage.type(), Scalar(0));

		// Calculate scaling
		float sx = (float)newHeight / (float)oldHeight;
		float sy = (float)newWidth / (float)oldWidth;

		// Affine inverse transformation: tranform Ohw space to Oxy space
		AffineTransform affInverseResize;
		affInverseResize.Scale(1 / sx, 1 / sy); // Reverse scaling

		// Apply Affine inverse transformation
		this->Transform(srcImage, dstImage, &affInverseResize, interpolator);

		return 1;
	}
}

int GeometricTransformer::Flip(
	const Mat& srcImage,
	Mat& dstImage,
	bool direction,
	PixelInterpolate* interpolator)
{
	// Check if source image is empty
	if (srcImage.empty()) return 0;
	else {
		// The height and width of images
		int height = srcImage.rows, width = srcImage.cols;

		// Initialize destination image
		dstImage = Mat(height, width, srcImage.type(), Scalar(0));

		if (direction == HORIZONTAL_DIRECTION)
		{
			// For flipping vertically
			// Affine inverse transformation: tranform Ohw space to Oxy space
			AffineTransform affInverseFlipH;
			affInverseFlipH.Reflect(direction); // Reflect Ox, axis=0
			affInverseFlipH.Translate(0, width - 1); // Translate image vector(0, width-1)

			// Apply Affine inverse transformation
			this->Transform(srcImage, dstImage, &affInverseFlipH, interpolator);
		}
		else {
			// For flipping vertically
			// Affine inverse transformation: tranform Ohw space to Oxy space
			AffineTransform affInverseFlipV;
			affInverseFlipV.Reflect(direction); // Reflect Oy, axis=1
			affInverseFlipV.Translate(height-1, 0); // Translate image vector(height-1, 0)

			// Apply Affine inverse transformation
			this->Transform(srcImage, dstImage, &affInverseFlipV, interpolator);
		}
		return 1;
	}
}

bool GeometricTransformer::IsInside(const int& x, const int& y, const Mat& matrix)
{
	int height = matrix.rows, width = matrix.cols;
	if (x >= 0 && y >= 0 && x < height && y < width)
		return true;
	return false;
}

void GeometricTransformer::CalcSizeRotatedRect(
	const int& oldHeight,
	const int& oldWidth,
	int& newHeight,
	int& newWidth,
	float angle)
{
	if (fabs(sin(angle) - 0.0) < EPSILON)
	{
		newHeight = oldHeight;
		newWidth = oldWidth;
	}
	else if (fabs(sin(angle) - 1.0) < EPSILON || fabs(sin(angle) - (-1.0)) < EPSILON)
	{
		newHeight = oldWidth;
		newWidth = oldHeight;
	}
	else
	{
		if (sin(angle) > 0.0 && cos(angle) < 0.0)
			angle = PI - angle;
		newHeight = (int)(abs(oldHeight * cos(angle)) + abs(oldWidth * sin(angle)));
		newWidth = (int)(abs(oldHeight * sin(angle)) + abs(oldWidth * cos(angle)));
	}
}

GeometricTransformer::GeometricTransformer(){}
GeometricTransformer::~GeometricTransformer(){}

