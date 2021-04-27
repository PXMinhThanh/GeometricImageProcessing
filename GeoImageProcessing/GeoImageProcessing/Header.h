#pragma once
#include <iostream>
#include <cmath>
#include <typeinfo>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#define PI 3.141592654

using namespace cv;
using namespace std;

constexpr bool HORIZONTAL_DIRECTION = 0;
constexpr bool VERTICAL_DIRECTION = 1;

constexpr auto EPSILON = 1e-5;