void _captureThreadFunction()
{
    cv::VideoCapture _capture;

    _capture.open( 0 );

    if (! _capture.isOpened() )
    {
        STATUS = -1;
        return;
    }

    while (! STOP )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( MAINTHREADDELTAMS ) );
        _capture >> _currentFrame;
        STATUS = 1;
    }

    STATUS = 0;
    _capture.release();
}
