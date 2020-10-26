#include "Y4mGen.h"
#include <iostream>
#include <fstream>

Y4mGen::Y4mGen(cv::Mat& image, int duration)
{
	this->frameSize = std::ceil((30.0 / 1001.0) * duration);

	auto imageSize = image.size();
	this->width = imageSize.width;
	this->height = imageSize.height;

	this->bufferLength = this->height * this->width * 3;
	this->yuv420_frame_buffer = new unsigned char[this->bufferLength];

	unsigned char* yuv420p = new unsigned char[this->bufferLength];
	unsigned char* rgb = new unsigned char[this->bufferLength];
	memcpy_s(rgb, this->bufferLength, image.data, this->bufferLength);
	if (yuv420p == NULL || rgb == NULL)
		return;
	int frameSize = width * height;
	int chromaSize = frameSize / 4;

	int yIndex = 0;
	int uIndex = frameSize;
	int vIndex = frameSize + chromaSize;

	int R, G, B, Y, U, V;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			B = rgb[(i * width + j) * 3 + 0];
			G = rgb[(i * width + j) * 3 + 1];
			R = rgb[(i * width + j) * 3 + 2];

			//RGB to YUV
			Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
			U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
			V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

			yuv420p[yIndex++] = (unsigned char)((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
			if (i % 2 == 0 && j % 2 == 0)
			{
				yuv420p[uIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
				yuv420p[vIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
			}
		}
	}
	yuv420_frame_buffer = yuv420p;

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
