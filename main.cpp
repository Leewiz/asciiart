#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <ncurses.h>
#include <iostream>
#include <fstream>

#include <thread>
#include <chrono>

std::string ascii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"; // 67 chars
std::string type2string(int type);
void asciify(cv::Mat image);
typedef std::vector<std::vector<char> > AsciiMatrix;

int main(int argc, char** argv)
{
	// this section gets access to the connected camera
	cv::Mat frame, smallerFrame;
	cv::VideoCapture cap;
	std::string lineFromFile;

	if(!cap.open(0))
	{
        return 0;
	}

	initscr();
    for(;;)
    {
		
		clear();
		
		cap >> frame;
		cv::resize(frame, smallerFrame, cv::Size(138, 138));
		asciify(smallerFrame); // writes frame to file

		std::ifstream is ("test.txt", std::ifstream::binary);
		if (is) 
		{
			// get length of file:
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);

			char * buffer = new char [length];

			// read data as a block:
			is.read (buffer,length);

			if (is)
			{
				addstr(buffer); // print buffer to screen	
				refresh();
			}
			else
			{
				std::cout << "error: only " << is.gcount() << " could be read";
				return 1;
			}
			is.close();
			// ...buffer contains the entire file...

			delete[] buffer;
		}

		if( frame.empty() ) break; // end of video stream
		// imshow("this is you, smile! :)", frame); // display camera capture in a window
		if( cv::waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
    }

	// create mat objects to use to experiment with opencv builtin features.
	// "image" is the raw image.
	cv::Mat image, smallerImage, gray, canny;
	image = cv::imread("../test.jpg");

	// make sure an image loaded
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	// print some stuff about the image because why not
	std::cout << image.size() << std::endl; // image dimensions
	std::cout << image.total() << std::endl; // total number of pixels
	std::cout << type2string(image.type()) << std::endl; // type of matrix element

	// resize the image
	cv::resize(image, smallerImage, cv::Size(), 0.23, 0.23);
	

	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try
	{
		cv::imwrite("test.jpg", image, compression_params);
	}
	catch (std::runtime_error & ex)
	{
		fprintf(stderr, "Exception converting image to PNG format: %s \n", ex.what());
		return 1;
	}

	asciify(smallerImage);

	// testing some opencv features
	cv::namedWindow("iss", cv::WINDOW_AUTOSIZE);
	cv::imshow("iss", smallerImage); // raw image

	// convert color to grayscale
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::namedWindow("ass", cv::WINDOW_AUTOSIZE);
	cv::imshow("ass", gray); // grayscale image

	// use Canny algorithm to find edges
	cv::Canny(gray, canny, 10, 100, 3, true);
	cv::namedWindow("issass", cv::WINDOW_AUTOSIZE);
	cv::imshow("issass", canny);


	cv::waitKey(0);
	return 0;
}

std::string type2string(int type)
{
	std::string r;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth)
	{
	case CV_8U: r = "8U"; break;
	case CV_8S: r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

void asciify(cv::Mat image)
{
	std::ofstream fstream;
	fstream.open("test.txt");

	std::vector<char> row(600*.23, '`');
	AsciiMatrix thePic(600*.23, row);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			double average = 0;

			// get the rgb Vec3 color code for current pixel
			cv::Vec3b& rgb = image.at<cv::Vec3b>(i,j);

			// calculate average 'brightness' for all 3 colors of this pixel
			average = (rgb[0] + rgb[1] + rgb[2]) / 3;

			// assign an ascii character based on the calculated average 'brightness' of the pixel
			thePic[i][j] = ascii[average / 4];

			// write the character representation of the pixel to file
			fstream << thePic[i][j] << thePic[i][j] << thePic[i][j];
			if (j == image.cols-1)
			{
				fstream << "\n";
			}
		}
	}
	fstream.close();
}