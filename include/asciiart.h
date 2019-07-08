#ifndef ASCIIART_H_
#define ASCIIART_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class AsciiArt
{
public:
    // Default constructor
    AsciiArt();
    // Destructor
    ~AsciiArt();

    void asciifyPic(std::string pathToImageToAsciify);
    
    void asciifyPic(cv::Mat imageToAsciify);
    
    void asciifyVideo();

    // image matrix to store pixel/image data
    cv::Mat frame, smallerFrame;

    // Video capture device
    cv::VideoCapture cap;

    // type to contain the matrix of ascii characters that represent pixels of an image
    typedef std::vector<std::vector<char> > AsciiMatrix;

private:
// array of ascii characters sorted by density used to replace color pixels
std::string ascii_; // 67 chars

};

#endif // ASCIIART_H_