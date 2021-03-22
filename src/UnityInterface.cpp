#include "Utils/Dependencies.cpp"

#include "Utils/ExportPreProcess.cpp"
#include "Utils/MonoArrayWorkaround.cpp"

#include "GlobalVariables.cpp"

#include "Algorithms/buttonMaskDetection.cpp"

#include "Threads/CaptureThreadFunction.cpp"
#include "Threads/BoxThreadFunction.cpp"

#include "Utils/HelperFunctions.cpp"

extern "C" void EXPORT AddBox( int id )
{
    _AddBox( id );
}

extern "C" void EXPORT RemoveBox( int id )
{
    _RemoveBox( id );
}

// Initialize module.RED_THRESH_VALUE
extern "C" void EXPORT Init( int mainThreadDeltaMs, int boxThreadDeltaMs, int blueThreshValue, int greenThreshValue, int redThreshValue )
{
    _Init( mainThreadDeltaMs, boxThreadDeltaMs, blueThreshValue, greenThreshValue, redThreshValue );
}

extern "C" void EXPORT Stop()
{
    _Stop();
}

extern "C" int EXPORT GetStatus()
{
    return STATUS;
}

extern "C" int EXPORT GetMatType()
{
    return _currentFrame.type();
}

extern "C" double EXPORT GetBoxValue( int id )
{
    return _delta( _boxesCurrentValuesB[id], _boxesBaseValuesB[id] )
         + _delta( _boxesCurrentValuesG[id], _boxesBaseValuesG[id] )
         + _delta( _boxesCurrentValuesR[id], _boxesBaseValuesR[id] );
}

extern "C" void EXPORT SetBaseValues()
{
    std::map< int, std::thread >::iterator it;

    for ( it = _boxesThreads.begin(); it != _boxesThreads.end(); ++it )
    {
        _boxesBaseValuesB[it->first] = _boxesCurrentValuesB[it->first];
        _boxesBaseValuesG[it->first] = _boxesCurrentValuesG[it->first];
        _boxesBaseValuesR[it->first] = _boxesCurrentValuesR[it->first];
    }
}

extern "C" void EXPORT GetRawImage( byte *data, int width, int height )
{
    cv::Mat resizedMat( height, width, _currentFrame.type() );
    cv::resize( _currentFrame, resizedMat, resizedMat.size(), cv::INTER_CUBIC );

    cv::Mat argbImg;
    cv::cvtColor( resizedMat, argbImg, cv::COLOR_BGR2RGBA );
    std::memcpy( data, argbImg.data, argbImg.total() * argbImg.elemSize() );
}

extern "C" void EXPORT GetRawButtonMaskImage( int id, byte *data, int width, int height )
{
    if ( _boxesThreads.count( id ) != 0 )
    {
        cv::Mat resizedMat( height, width, _boxesMasks[id].type() );
        cv::resize( _boxesMasks[id], resizedMat, resizedMat.size(), cv::INTER_CUBIC );

        cv::Mat argbImg;
        cv::cvtColor( resizedMat, argbImg, cv::COLOR_GRAY2RGBA );
        std::memcpy( data, argbImg.data, argbImg.total() * argbImg.elemSize() );
    }
}

extern "C" void EXPORT GetRawButtonImage( int id, byte *data, int width, int height )
{
    if ( _boxesThreads.count( id ) != 0 )
    {
        cv::Mat cropFrame;
        cv::bitwise_and( _currentFrame, _currentFrame, cropFrame, _boxesMasks[id] );

        cv::Mat resizedMat( height, width, cropFrame.type() );
        cv::resize( cropFrame, resizedMat, resizedMat.size(), cv::INTER_CUBIC );

        cv::Mat argbImg;
        cv::cvtColor( resizedMat, argbImg, cv::COLOR_BGR2RGBA );
        std::memcpy( data, argbImg.data, argbImg.total() * argbImg.elemSize() );
    }
}

extern "C" void EXPORT GetMaskDetectionOutput( byte *data, int width, int height )
{
    cv::Mat mask = _buttonMaskDetection().clone();
    cv::Mat resizedMat( height, width, mask.type() );
    cv::resize( mask, resizedMat, resizedMat.size(), cv::INTER_CUBIC );

    cv::Mat argbImg;
    cv::cvtColor( resizedMat, argbImg, cv::COLOR_GRAY2RGBA );
    std::memcpy( data, argbImg.data, argbImg.total() * argbImg.elemSize() );
}

extern "C" int EXPORT GetRawImageArrayLenght()
{
    return _currentFrame.rows * _currentFrame.cols * 3;
}

extern "C" int EXPORT GetRawImageHeight()
{
    return _currentFrame.rows;
}

extern "C" int EXPORT GetRawImageWidth()
{
    return _currentFrame.cols;
}
