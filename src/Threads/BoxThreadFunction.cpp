void _boxThreadFunction( int id )
{
    bool first = true;

    while (! ( STOP || _boxesStop[id] ) )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( BOXTHREADDELTAMS ) );

        if ( first )
        {
            _boxesMasks[id] = _buttonMaskDetection().clone();
        }

        // Get box from image
        cv::Mat box;
        cv::bitwise_and( _currentFrame, _currentFrame, box, _boxesMasks[id] );

        // Sum all values of box
        cv::Scalar sums = cv::sum( box );

        _boxesCurrentValuesB[id] = sums[0];
        _boxesCurrentValuesG[id] = sums[1];
        _boxesCurrentValuesR[id] = sums[2];

        if ( first )
        {
            _boxesBaseValuesB[id] = _boxesCurrentValuesB[id];
            _boxesBaseValuesG[id] = _boxesCurrentValuesB[id];
            _boxesBaseValuesR[id] = _boxesCurrentValuesB[id];
            first = false;
        }
    }
}
