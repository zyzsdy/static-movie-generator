#pragma once
#include <opencv2/opencv.hpp>

class Y4mGen
{
	unsigned char* yuv420FrameBuffer;
	unsigned long long bufferLength;
	unsigned long long yuvBufferLength;
	unsigned long long frameSize;
	int width;
	int height;
	int framerateN;
	int framerateD;
public:
	Y4mGen(cv::Mat& image, int duration, double framerate);
	~Y4mGen();
	void genMovie(std::string output) const;
};
