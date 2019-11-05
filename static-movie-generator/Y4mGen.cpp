#include "Y4mGen.h"
#include <iostream>
#include <fstream>

Y4mGen::Y4mGen(cv::Mat& image, int duration)
{
	this->frameSize = std::ceil((30.0 / 1001.0) * duration);
	
	auto imageSize = image.size();
	this->width = imageSize.width;
	this->height = imageSize.height;

	this->bufferLength = this->width * this->height * 3 / 2 * sizeof(unsigned char);
	this->yuv420_frame_buffer = new unsigned char[this->bufferLength];

	cv::Mat yuv420Mat(this->height + this->height / 2, this->width, CV_8UC1, this->yuv420_frame_buffer);
	cv::cvtColor(image, yuv420Mat, cv::COLOR_RGB2YUV_YV12);

	memcpy_s(this->yuv420_frame_buffer, this->bufferLength, yuv420Mat.data, this->bufferLength);

	std::cerr << "Total frames: " << this->frameSize << std::endl;
}

Y4mGen::~Y4mGen()
{
	delete[] this->yuv420_frame_buffer;
}

void Y4mGen::genMovie(std::string output)
{
	std::ostream* outputStream;
	if (output == "-") {
		outputStream = &std::cout;
	}
	else {
		outputStream = new std::ofstream(output, std::ios::out | std::ios::binary);
		if (!*outputStream) {
			std::cerr << "The output file failed to open." << std::endl;
			return;
		}
	}

	std::stringstream y4mHeaderStream;
	std::string y4mHeader;
	y4mHeaderStream << "YUV4MPEG2 W" << this->width << " H" << this->height << " F30000:1001 Ip A1:1 C420\n";
	y4mHeader = y4mHeaderStream.str();

	outputStream->write(y4mHeader.c_str(), y4mHeader.length());
	outputStream->flush();
	for (int i = 0; i < this->frameSize; i++) {
		outputStream->write("FRAME\n", 6);
		outputStream->write((const char *)this->yuv420_frame_buffer, this->bufferLength);
		outputStream->flush();
	}
}
