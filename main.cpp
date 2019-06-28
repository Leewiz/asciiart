#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <fstream>.

std::string type2string(int type);
typedef std::vector<std::vector<char> > AsciiMatrix;

int main(int argc, char** argv)
{
	// creating mat objects to use to experiment with opencv builtin features.
	// "image" is the raw image.
	cv::Mat image, gray, canny;
	image = cv::imread("C:/Users/green/Downloads/premium.png");

	// make sure an image loaded
	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	std::cout << image.size() << std::endl;
	std::cout << image.total() << std::endl;
	std::cout << type2string(image.type()) << std::endl;

	std::string ascii = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"; // 67 chars
	std::ofstream stream;
	stream.open("test.txt");

	std::vector<char> row(600, '`');
	AsciiMatrix thePic(600, row);

	//char array[600][600];

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			double average = 0;

			// get the rgb Vec3 color code for current pixel
			cv::Vec3b& rgb = image.at<cv::Vec3b>(i,j);

			// calculate average 'brightness' for all 3 colors of this pixel
			average = (rgb[0] + rgb[1] + rgb[2]) / 3;

			// assign average value to each color. This effectively grayscales the image.
			//rgb[0] = average;
			//rgb[1] = average;
			//rgb[2] = average;

			// assign an ascii character based on the calculated average 'brightness' of the pixel
			thePic[i][j] = ascii[average / 4];

			// write the character representation of the pixel to file
			stream << thePic[i][j] << thePic[i][j] << thePic[i][j];
			if (j == image.cols-1)
			{
				stream << "\n";
			}
		}
	}
	stream.close();

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
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


	//if (image.isContinuous())
	//{
	//	array.assign(image.data, image.data + image.total());
	//}
	//else
	//{
	//	for (int i = 0; i < image.rows; i++)
	//	{
	//		array.insert(array.end(), image.ptr<uchar>(i), image.ptr<uchar>(i) + image.cols);
	//	}
	//}


	//for (int i = 0; i < array.size(); i++)
	//{
	//	uchar& uxy = array[i];
	//	int color = (int)uxy;
	//	std::cout << color << " ";

	//	if (i % 3 == 0)
	//	{
	//		std::cout << std::endl;
	//	}
	//}


	// testing some opencv features
	cv::namedWindow("iss", cv::WINDOW_AUTOSIZE);
	cv::imshow("iss", image); // raw image

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