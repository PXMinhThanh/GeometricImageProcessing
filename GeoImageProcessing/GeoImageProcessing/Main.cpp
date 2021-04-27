#include "GeometricTransformer.h"

int main(int argc, char** argv)
{
	// Parse the command line
	String keys =
		"{@srcImg  |<none> | input image path}"
		"{@dstImg  |<none> | output image path}"
		"{@a       |1.0	   | }"
		"{@b       |1.0	   | }"
		"{zoom     |       | scale an image with (sx, sy): sx * height; sy * width}"   
		"{resize   |       | resize an image with new width and height}"
		"{rotK     |       | rotate an image around its center and unchange the result size}"
		"{rotP     |       | rotate an image around its center and keep the whole image}"
		"{flipV    |       | flip an image vertically}"
		"{flipH    |       | flip an image horizontally}"
		"{bl       |       | bilinear interpolation}"
		"{nn       |       | nearest neighbour interpolation}"
		"{help     |       | show help message}";

	CommandLineParser parser(argc, argv, keys);
	// Print information about the application
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	// Check if the input file path is empty
	String srcImgPath = parser.get<String>("@srcImg");
	if (srcImgPath.empty())
	{
		cout << "Input file path is empty!" << endl;
		return 0;
	}

	// Read the input image from file
	Mat srcImg = imread(srcImgPath, IMREAD_ANYCOLOR);
	if (!srcImg.data)
	{
		cout << "Image not loaded!" << endl;
		return 0;
	}

	// Initialize variables
	Mat dstImg;
	GeometricTransformer* transformer = new GeometricTransformer;
	PixelInterpolate* interpolator;
	if (parser.has("bl"))
		interpolator = new BilinearInterpolate;
	else
		interpolator = new NearestNeighborInterpolate;

	// Geometric transformations
	if (parser.has("zoom"))
	{
		// Zoom
		float sx = parser.get<float>("@a");
		float sy = parser.get<float>("@b");
		transformer->Scale(srcImg, dstImg, sx, sy, interpolator);
	}
	else if (parser.has("resize"))
	{
		// Resize
		int newWidth = parser.get<int>("@a");
		int newHeight = parser.get<int>("@b");
		transformer->Resize(srcImg, dstImg, newWidth, newHeight, interpolator);
	}
	else if (parser.has("rotK"))
	{
		// Rotate unkeep
		float angle = atof(argv[4]);
		transformer->RotateUnkeepImage(srcImg, dstImg, angle, interpolator);
	}
	else if (parser.has("rotP"))
	{
		// Rotate keep whole
		float angle = atof(argv[4]);
		transformer->RotateKeepImage(srcImg, dstImg, angle, interpolator);
	}
	else if (parser.has("flipV"))
	{
		// Flip vertically
		transformer->Flip(srcImg, dstImg, VERTICAL_DIRECTION, interpolator);
	}
	else if (parser.has("flipH"))
	{
		// Flip horizontally
		transformer->Flip(srcImg, dstImg, HORIZONTAL_DIRECTION, interpolator);
	}
	else
	{
		cout << "The transformation is empty!" << endl;
		return 0;
	}

	// Write destination image to output file
	String dstImgPath = parser.get<String>("@dstImg");
	if (dstImgPath.empty() || imwrite(dstImgPath, dstImg) == false)
	{
		cout << "Can not save output image!" << endl;
		return 0;
	}

	// Print any errors if any
	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	// Notification completing process
	cout << "Process and save data successfully!" << endl;
	return 0;
}
