#include <iostream>
#include "cmdline.h"
#include "CoverGen.h"
#include "Y4mGen.h"
using namespace std;

void processArgs(string input, string output, int duration, bool coverMode, int width, int height) {
	Mat image;

	if (coverMode) {
		if (width % 2 != 0 || height % 2 != 0) {
			cerr << "Error: The width and height of the output movie must be even." << endl;
			return;
		}

		auto coverGen = make_shared<CoverGen>(input, width, height);
		cerr << "Generating image...";
		image = coverGen->getStaticFrame();
		cerr << "finish." << endl;

		coverGen.reset(); //ÊÍ·ÅµôÉú³ÉÆ÷
	}
	else {
		image = cv::imread(input);
	}

	auto y4mGen = make_shared<Y4mGen>(image, duration);
	y4mGen->genMovie(output);
}

int main(int argc, char *argv[]) {
	ios::sync_with_stdio(false);
	cmdline::parser argParser;

	argParser.add<string>("input", 'i', "Static image to be converted.", true);
	argParser.add<string>("output", 'o', "The location of output. default: standard output. Hint: The output will be very large. Try not to save it and use pipeline to dispose of it.", false, "-");
	argParser.add<int>("duration", 'd', "The duration of the movie, in milliseconds.", true);
	argParser.add("cover", 'c', "Cover mode");
	argParser.add<int>("width", 'w', "Width of output (Only valid in Cover mode.)", false, 1280);
	argParser.add<int>("height", 'h', "Height of output (Only valid in Cover mode.)", false, 720);
	argParser.set_program_name("static-movie-generator");

	argParser.parse_check(argc, argv);

	bool coverMode = argParser.exist("cover");
	string input = argParser.get<string>("input");
	string output = argParser.get<string>("output");
	int duration = argParser.get<int>("duration");
	int width = argParser.get<int>("width");
	int height = argParser.get<int>("height");
	processArgs(input, output, duration, coverMode, width, height);
}
