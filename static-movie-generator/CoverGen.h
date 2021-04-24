#pragma once
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

class CoverGen
{
	std::string input;
	int width;
	int height;
public:
	CoverGen(std::string, int, int);
	Mat getStaticFrame() const;
};
