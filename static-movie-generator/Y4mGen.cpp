#include "Y4mGen.h"
#include <iostream>
#include <fstream>
#include <cmath>

Y4mGen::Y4mGen(cv::Mat& image, const int duration, const double framerate)
{
	auto framerateInt = static_cast<int>(lround(framerate * 1001));
	if (framerateInt % 1000 == 0)
	{
		this->framerateD = 1001;
		this->framerateN = framerateInt;
	}
	else
	{
		this->framerateD = 1000;
		this->framerateN = static_cast<int>(framerate * 1000);
	}

	this->frameSize = std::ceil(framerate / 1000 * duration);

	const auto imageSize = image.size();
	this->width = imageSize.width;
	this->height = imageSize.height;
	this->bufferLength = static_cast<unsigned long long>(this->height) * this->width * 3 * sizeof(unsigned char);
	this->yuvBufferLength = this->bufferLength / 2;
	this->yuv420FrameBuffer = new unsigned char[this->yuvBufferLength];

	auto* yuv420p = new unsigned char[this->yuvBufferLength];
	auto* rgb = new unsigned char[this->bufferLength];
	memcpy_s(rgb, this->bufferLength, image.data, this->bufferLength);
	if (yuv420p == nullptr || rgb == nullptr)
		return;
	const auto frameSize = static_cast<unsigned long long>(this->width) * this->height;
	const auto chromaSize = frameSize / 4;

	auto yIndex = 0;
	auto uIndex = frameSize;
	auto vIndex = frameSize + chromaSize;

	for (auto i = 0; i < this->height; i++)
	{
		for (auto j = 0; j < this->width; j++)
		{
			const int B = rgb[(i * this->width + j) * 3 + 0];
			const int G = rgb[(i * this->width + j) * 3 + 1];
			const int R = rgb[(i * this->width + j) * 3 + 2];

			//RGB to YUV
			const auto Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
			const auto U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
			const auto V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

			yuv420p[yIndex++] = static_cast<unsigned char>((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
			if (i % 2 == 0 && j % 2 == 0)
			{
				yuv420p[uIndex++] = static_cast<unsigned char>((U < 0) ? 0 : ((U > 255) ? 255 : U));
				yuv420p[vIndex++] = static_cast<unsigned char>((V < 0) ? 0 : ((V > 255) ? 255 : V));
			}
		}
	}
	memcpy_s(yuv420FrameBuffer, this->yuvBufferLength, yuv420p, this->yuvBufferLength);

	delete[] yuv420p;
	delete[] rgb;
	std::cerr << "Total frames: " << this->frameSize << std::endl;
}

Y4mGen::~Y4mGen()
{
	delete[] this->yuv420FrameBuffer;
}

void Y4mGen::genMovie(const std::string output) const
{
	std::ostream* outputStream;
	if (output == "-")
	{
		outputStream = &std::cout;
	}
	else
	{
		outputStream = new std::ofstream(output, std::ios::out | std::ios::binary);
		if (!*outputStream)
		{
			std::cerr << "The output file failed to open." << std::endl;
			return;
		}
	}

	std::stringstream y4mHeaderStream;
	std::string y4mHeader;

	y4mHeaderStream << "YUV4MPEG2 W" << this->width << " H" << this->height 
					<< " F" << this->framerateN << ":" << this->framerateD << " Ip A1:1 C420\n";
	y4mHeader = y4mHeaderStream.str();

	outputStream->write(y4mHeader.c_str(), y4mHeader.length());
	outputStream->flush();
	for (auto i = 0; i < this->frameSize; i++)
	{
		outputStream->write("FRAME\n", 6);
		outputStream->write(reinterpret_cast<const char*>(this->yuv420FrameBuffer),
		                    static_cast<unsigned long long>(this->width) * this->height * 3 / 2 * sizeof(unsigned char));
		outputStream->flush();
	}
}
