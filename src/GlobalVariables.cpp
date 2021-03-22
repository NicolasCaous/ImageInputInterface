// Variable to stop all threads. If set to true, all threads will halt.
bool STOP;
// Variable to indicate de status of the module.
//
//  0 -> RUNNING
// -1 -> COULD NOT OPEN CAMERA
//  1 -> CAPTURING IMAGE
//  2 -> FINISHED
int STATUS;
int MAINTHREADDELTAMS;
int BOXTHREADDELTAMS;

// Main thread to process only frames;
std::thread _captureThread;
// And store here.
cv::Mat _currentFrame;

// Map to controll all boxes threads.
std::map< int, std::thread > _boxesThreads;
std::map< int, double > _boxesCurrentValuesB;
std::map< int, double > _boxesCurrentValuesG;
std::map< int, double > _boxesCurrentValuesR;
std::map< int, double > _boxesBaseValuesB;
std::map< int, double > _boxesBaseValuesG;
std::map< int, double > _boxesBaseValuesR;
std::map< int, cv::Mat > _boxesMasks;
std::map< int, bool > _boxesStop;

int BLUE_THRESH_VALUE;
int GREEN_THRESH_VALUE;
int RED_THRESH_VALUE;
