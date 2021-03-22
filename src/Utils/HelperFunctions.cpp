void _Start()
{
    STOP = false;
    STATUS = 0;

    // Initialize Camera Capture
    _captureThread = std::thread( _captureThreadFunction );
}

void _Init( int mainThreadDeltaMs, int boxThreadDeltaMs, int blueThreshValue, int greenThreshValue, int redThreshValue )
{
    _boxesThreads = std::map< int, std::thread >();
    _boxesCurrentValuesB = std::map< int, double >();
    _boxesCurrentValuesG = std::map< int, double >();
    _boxesCurrentValuesR = std::map< int, double >();
    _boxesBaseValuesB = std::map< int, double >();
    _boxesBaseValuesG = std::map< int, double >();
    _boxesBaseValuesR = std::map< int, double >();
    _boxesMasks = std::map< int, cv::Mat >();
    _boxesStop = std::map< int, bool >();

    MAINTHREADDELTAMS = mainThreadDeltaMs;
    BOXTHREADDELTAMS = boxThreadDeltaMs;

    BLUE_THRESH_VALUE = blueThreshValue;
    GREEN_THRESH_VALUE = greenThreshValue;
    RED_THRESH_VALUE = redThreshValue;

    _Start();
}

void _Stop()
{
    STOP = true;

    std::map< int, std::thread >::iterator it;

    _captureThread.join();

    for ( it = _boxesThreads.begin(); it != _boxesThreads.end(); ++it )
    {
        it->second.join();
    }

    _boxesThreads.clear();
    _boxesCurrentValuesB.clear();
    _boxesCurrentValuesG.clear();
    _boxesCurrentValuesR.clear();
    _boxesBaseValuesB.clear();
    _boxesBaseValuesG.clear();
    _boxesBaseValuesR.clear();
    _boxesMasks.clear();
    _boxesStop.clear();

    STATUS = 2;
}

void _Restart()
{
    if ( STATUS == 0 )
    {
        _Stop();
    }
    _Start();
}

void _AddBox( int id )
{
    if ( _boxesThreads.count( id ) == 0 )
    {
        _boxesThreads[id] = std::thread(
            _boxThreadFunction,
            id );
        _boxesCurrentValuesB[id] = 0.0;
        _boxesCurrentValuesG[id] = 0.0;
        _boxesCurrentValuesR[id] = 0.0;
        _boxesBaseValuesB[id] = 0.0;
        _boxesBaseValuesG[id] = 0.0;
        _boxesBaseValuesR[id] = 0.0;
        _boxesStop[id] = false;
    }
}

void _RemoveBox( int id )
{
    if ( _boxesThreads.count( id ) != 0 )
    {
        _boxesStop[id] = true;
        _boxesThreads[id].join();

        _boxesThreads.erase(id);
        _boxesCurrentValuesB.erase(id);
        _boxesCurrentValuesG.erase(id);
        _boxesCurrentValuesR.erase(id);
        _boxesBaseValuesB.erase(id);
        _boxesBaseValuesG.erase(id);
        _boxesBaseValuesR.erase(id);
        _boxesMasks.erase(id);
        _boxesStop.erase(id);
    }
}

double _delta( double a, double b )
{
    return ( a > b ) ?  a - b : b - a;
}
