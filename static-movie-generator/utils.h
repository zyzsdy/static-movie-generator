#pragma once
#include <opencv2/opencv.hpp>

inline cv::Rect getCenterRect(int width, int height) {
	int ksize;
	int cstart;

	if (width < height) {
		ksize = width;
		cstart = (height - ksize) / 2;
		return cv::Rect(0, cstart, ksize, ksize);
	}
	else {
		ksize = height;
		cstart = (width - ksize) / 2;
		return cv::Rect(cstart, 0, ksize, ksize);
	}
}

inline cv::Rect getCenterImageArea(int width, int height, double scale = 0.9) {
	int ksize;

	if (width < height) {
		ksize = width * scale;
	}
	else {
		ksize = height * scale;
	}

	int xstart = (width - ksize) / 2;
	int ystart = (height - ksize) / 2;

	return cv::Rect(xstart, ystart, ksize, ksize);
}

inline cv::Rect getCenterRectFromSquare(int squareLength, int width, int height) {
	int cstart;
	
	if (width < height) {
		cstart = (squareLength - width) / 2;
		return cv::Rect(cstart, 0, width, height);
	}
	else {
		cstart = (squareLength - height) / 2;
		return cv::Rect(0, cstart, width, height);
	}
}

void addAlphaMat(cv::Mat& dst, cv::Mat& alpha);