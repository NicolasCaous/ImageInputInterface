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

        // Capture frame-by-frame
        cap >> frame;

        // Get box from image
        Mat box(
            frame,
            Range( 10, 100 ),
            Range( 10, 500 ));

        Mat greyBox;

        cvtColor( box, greyBox, COLOR_BGR2GRAY );

        // Sum all values of box
        double boxValue = sum( greyBox )[0];

        // Convert double to String
        std::ostringstream strs;
        strs << boxValue;

        // Draws a rectangle inside de frame
        rectangle(
            frame,
            Point( 10, 10 ),
            Point( 100, 100 ),
            Scalar( 0, 255, 0 ),
            1, 8, 0 );

        // Shows the box value
        putText(
            frame,
            strs.str(),
            Point2f( 100, 100 ),
            FONT_HERSHEY_PLAIN,
            2,
            Scalar( 0, 255, 0 ) );

        // Shows the box value
        putText(
            frame,
            ( boxValue < 1000000 ) ? "true" : "false",
            Point2f( 400, 100 ),
            FONT_HERSHEY_PLAIN,
            2,
            Scalar( 0, 255, 0 ) );

        // If the frame is empty, break immediately
        if ( frame.empty() )
            break;

        // Display the resulting frame
        imshow( "Frame", frame );
        imshow( "Box", greyBox );

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
