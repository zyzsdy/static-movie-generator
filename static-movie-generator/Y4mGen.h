#pragma once
#include <opencv2/opencv.hpp>

class Y4mGen
{
	unsigned char* yuv420_frame_buffer;
	int bufferLength;
	int frameSize;
	int width;
	int height;
public:
	Y4mGen(cv::Mat& image, int duration);
	~Y4mGen();
	void genMovie(std::string output);
};

