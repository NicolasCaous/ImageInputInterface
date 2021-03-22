cv::Mat _buttonMaskDetection()
{
    cv::Mat sourceFrame = _currentFrame.clone();
    cv::Mat mask, mask2, mask3, finalMask;

    cv::Mat channel[3];
    cv::split( sourceFrame, channel );

    cv::threshold( channel[0], mask, BLUE_THRESH_VALUE, 255, cv::THRESH_BINARY);
    cv::threshold( channel[1], mask2, GREEN_THRESH_VALUE, 255, cv::THRESH_BINARY_INV);
    cv::threshold( channel[2], mask3, RED_THRESH_VALUE, 255, cv::THRESH_BINARY_INV);

    cv::bitwise_and( mask, mask2, finalMask );
    cv::bitwise_and( finalMask, mask3, finalMask );

    cv::Mat result;
    result = cv::Mat::zeros( finalMask.size(), finalMask.type() );
    std::vector< std::vector< cv::Point > > contours;
    std::vector< cv::Vec4i > hierarchy;
    int savedContour = -1;
    double maxArea = 0.0;

    // Find the largest contour
    //// findContours will "eat" the input image, so clone the binary Mat, we need it later:
    cv::findContours(
      finalMask.clone(),
      contours, hierarchy,
      cv::RETR_EXTERNAL,
      cv::CHAIN_APPROX_SIMPLE,
      cv::Point()
    );
    for ( int i = 0; i< contours.size(); i++ )
    {
        double area = cv::contourArea( contours[i] );
        if ( area > maxArea )
        {
            maxArea = area;
            savedContour = i;
        }
    }
    // Create mask
    cv::drawContours( result, contours, savedContour, cv::Scalar(255), cv::FILLED, 8 );
    cv::bitwise_and( finalMask, result, finalMask );

    return finalMask;
}
