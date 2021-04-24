#include "utils.h"

void addAlphaMat(cv::Mat& dst, cv::Mat& alpha)
{
	std::vector<cv::Mat> dst_channels;
	std::vector<cv::Mat> alpha_channels;

	split(dst, dst_channels);
	split(alpha, alpha_channels);

	for (auto i = 0; i < 3; i++)
	{
		dst_channels[i] = dst_channels[i].mul(255.0 - alpha_channels[3], 1.0 / 255.0);
		dst_channels[i] += alpha_channels[i].mul(alpha_channels[3], 1.0 / 255.0);
	}
	cv::merge(dst_channels, dst);
}
