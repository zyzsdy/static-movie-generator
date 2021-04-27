#pragma once
#include <opencv2/opencv.hpp>

class Y4mGen
{
	unsigned char* yuv420_frame_buffer;
	int bufferLength;
	int YUV_bufferLength;
	int frameSize;
	int width;
	int height;
	double framerate;
public:
	Y4mGen(cv::Mat& image, int duration, double framerate);
	~Y4mGen();
	void genMovie(std::string output) const;
};
