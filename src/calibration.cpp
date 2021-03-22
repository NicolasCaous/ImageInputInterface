#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    VideoCapture cap( 0 );

    // Check if camera opened successfully
    if(! cap.isOpened() )
    {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    while( 1 )
    {

        Mat frame;
        Mat sourceFrame;
        Mat mask, mask2, mask3, finalMask;
        Mat finalImage;

        // Capture frame-by-frame
        cap >> sourceFrame;

        Mat channel[3];
        split(sourceFrame, channel);

        threshold(channel[0], mask, 200, 255, THRESH_BINARY);
        threshold(channel[1], mask2, 200, 255, THRESH_BINARY_INV);
        threshold(channel[2], mask3, 200, 255, THRESH_BINARY_INV);
        //adaptiveThreshold(channel[0], mask, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 0);
        //adaptiveThreshold(channel[1], mask2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 3, 0);
        //adaptiveThreshold(channel[2], mask3, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 3, 0);

        bitwise_and(mask, mask2, finalMask);
        bitwise_and(finalMask, mask3, finalMask);

        Mat result;
        result = Mat::zeros(finalMask.size(), finalMask.type());;
        vector<vector<Point> >contours;
        vector<Vec4i>hierarchy;
        int savedContour = -1;
        double maxArea = 0.0;

        // Find the largest contour
        //// findContours will "eat" the input image, so clone the binary Mat, we need it later:
        findContours(finalMask.clone(), contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
        for (int i = 0; i< contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area > maxArea)
            {
                maxArea = area;
                savedContour = i;
            }
        }
        // Create mask
        drawContours(result, contours, savedContour, Scalar(255), FILLED, 8);
        bitwise_and(finalMask, result, finalMask);

        bitwise_and(sourceFrame, sourceFrame, frame, finalMask);

        Mat blackBlock = Mat(frame.size(), frame.type(), Scalar(0, 0, 0));
        Mat greenBlock = Mat(Size(300, 300), frame.type(), Scalar(255, 0, 0));
        greenBlock.copyTo(blackBlock(Rect(100, 100, greenBlock.cols, greenBlock.rows)));

        hconcat(frame, blackBlock, finalImage);

        // If the frame is empty, break immediately
        if ( frame.empty() )
            break;

        // Display the resulting frame
        imshow( "SourceFrame", sourceFrame );
        imshow( "Frame", finalImage );
        imshow( "Mask", finalMask );

        // Press  ESC on keyboard to exit
        char c = (char) waitKey( 25 );

        if( c == 27 )
            break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
