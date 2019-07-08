#include "asciiart.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ncurses.h>

#include <pthread.h>
#include <fstream>

static volatile bool keep_running = true;

// press 'q' while video is playing to stop
static void* userInputThread(void*)
{
    while(keep_running)
    {
        if(std::cin.get() == 'q')
        {
            keep_running = false;
        }
    }
}

AsciiArt::AsciiArt()
:ascii_("`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$")
{

}


AsciiArt::~AsciiArt()
{

}


void AsciiArt::asciifyPic(cv::Mat imageToAsciify)
{
    std::ofstream fstream;
	fstream.open("test.txt");

	std::vector<char> row(600*.23, '`');
	AsciiMatrix thePic(600*.23, row);

	for (int i = 0; i < imageToAsciify.rows; i++)
	{
		for (int j = 0; j < imageToAsciify.cols; j++)
		{
			double average = 0;

			// get the rgb Vec3 color code for current pixel
			cv::Vec3b& rgb = imageToAsciify.at<cv::Vec3b>(i,j);

			// calculate average 'brightness' for all 3 colors of this pixel
			average = (rgb[0] + rgb[1] + rgb[2]) / 3;

			// assign an ascii character based on the calculated average 'brightness' of the pixel
			thePic[i][j] = ascii_[average / 4];

			// write the character representation of the pixel to file
			fstream << thePic[i][j] << thePic[i][j] << thePic[i][j];
			if (j == imageToAsciify.cols-1)
			{
				fstream << "\n";
			}
		}
	}

	fstream.close();
}


void AsciiArt::asciifyVideo()
{
    if(!cap.open(0) || !cap.isOpened())
    {
        throw "failed to open cv::VideoCapture -- exiting";
    }

    initscr();

    // thread listening for user interrupt
    pthread_t threadId;
    (void) pthread_create(&threadId, 0, userInputThread, 0);

    std::string lineFromFile;
    while(keep_running)
    {
		clear();
		
		cap >> frame;
		cv::resize(frame, smallerFrame, cv::Size(138, 138));
        // write frame to file
		asciifyPic(smallerFrame); 

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
				return;
			}
			is.close();
			// ...buffer contains the entire file...

			delete[] buffer;
		}

		if( frame.empty() ) break; // end of video stream
    }
    (void) pthread_join(threadId, NULL);
    system("reset");
}

