#include "asciiart.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <ncurses.h>

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	AsciiArt art;

	try
	{
		art.asciifyVideo();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	try
	{
		art.asciifyPic(cv::imread("../test.jpg"));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	

	
	return 0;
}