#include <QCoreApplication>
#include "serial.h"
#include <math.h>
#include<iostream>
#include<thread>
#include <opencv2/opencv.hpp>
#include "util_funcs.h"
ArduinoSerialCommunicator s("/dev/ttyACM0", 115200);

bool lock = false;

int getAngle()
{
    return 0;
}


void sendCommand(std::string command)
{
    std::string msg= "";
    while(msg.find(command.substr(0, 4) + " 123")==std::string::npos) {
        s.sendString(command);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        msg = s.pollSerialPortForData();
        printf("%s", msg.c_str());
    }
}
std::vector<std::string>  cmds;
std::vector<std::string> calculateRoute(double angle,double dist)
{
    std::vector<std::string> cmds;
    cmds.push_back(std::string("turn") + " " + std::to_string((angle < 0) ? -1 : (angle >= 0)) + " " + std::to_string(std::abs(angle)));
    cmds.push_back(std::string("move") + " " + std::to_string(dist/2) + " 100");
    cmds.push_back("start ");
    return cmds;
}

std::string msg;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    cv::VideoCapture cap(0); //capture the video from webcam

    if (!cap.isOpened())  // if not success, exit program
    {
        std::cout << "Cannot access the camera with given index" << std::endl;
        return -1;
    }

    cv::namedWindow("Control Sliders", CV_WINDOW_AUTOSIZE);
    //create a window called "Control"
    std::cout<< "window created \n";

    ////RED
    //int iLowH = 170;
    //int iHighH = 179;

    //int iLowS = 150;
    //int iHighS = 255;

    //int iLowV = 60;
    //int iHighV = 255;

    //BLUE
    int iLowH = 93;
    int iHighH = 120;

    int iLowS = 196;
    int iHighS = 255;

    int iLowV = 121;
    int iHighV = 255;


    //Create trackbars in "Control" window
    cv::createTrackbar("LowH", "Control Sliders", &iLowH, 179); //Hue (0 - 179)
    cv::createTrackbar("HighH", "Control Sliders", &iHighH, 179);

    cv::createTrackbar("LowS", "Control Sliders", &iLowS, 255); //Saturation (0 - 255)
    cv::createTrackbar("HighS", "Control Sliders", &iHighS, 255);

    cv::createTrackbar("LowV", "Control Sliders", &iLowV, 255);//Value (0 - 255)
    cv::createTrackbar("HighV", "Control Sliders", &iHighV, 255);


    std::cout<< "trackbars created \n";

    double angle=0, distance=0;

    while(true)
    {
        std::cout<< "loop iteration \n";
        auto time_start = std::chrono::high_resolution_clock::now();
        cv::Mat imgOriginal;

        // read a new frame from video
        //bool bSuccess = cap.read(imgOriginal);
        cap >> imgOriginal;

//        if (!bSuccess) //if not success, break loop
//        {
//            std::cout << "Could not read frame from the video stream" << std::endl;
//            break;
//        }

        cv::pyrDown(imgOriginal, imgOriginal);


        //Convert the captured frame from BGR to HSV
        cv::Mat imgHSV;
        cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV);

        cv::Mat imgThresholded;
        inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);

        //Close the gaps in image

            DilateErode(imgThresholded);


        //Get edges using canny algorithm
        cv::Mat canny_out;
        cv::Canny(imgThresholded, canny_out, 100, 200);


        //Find the contours of the image
        std::vector<std::vector<cv::Point>> contouredElementsOnScreen;
        std::vector<cv::Vec3i> circlesFoundUsingContours;
        std::tie(contouredElementsOnScreen, circlesFoundUsingContours) = getContoursAndCircles(canny_out);
        for (auto circle : circlesFoundUsingContours)
        {
            cv::circle(imgOriginal, cv::Point{ (int)circle[0], (int)circle[1] }, (int)circle[2], cv::Scalar{ 255,0,0 }, 5);
        }

        cv::Point blobCenter = findCenterOfBlobUsingMoments(imgThresholded);


        if (blobCenter.x > 0 && blobCenter.y > 0)
        {

            //Draw a red line from the middle of the screen to ball position
            cv::Point screenCenter(imgOriginal.cols / 2, imgOriginal.rows / 2);
            cv::line(imgOriginal, screenCenter, blobCenter, cv::Scalar(0, 0, 255), 5);

        }

        cv::imshow("Thresholded Image", imgThresholded);
        //show B&W image
        //show Edges
        cv::imshow("Canny", canny_out);
        //show the original image with annotations
        cv::imshow("Original", imgOriginal);
//        getchar();
//        angle = getAngle();
//        cmds = calculateRoute(angle, distance);
//        for(auto str : cmds)
//        {
//            sendCommand(str);
//            printf("[RASPI] command sent: %s", str.c_str());
//        }

        cvWaitKey(1);

    }

    return a.exec();
}
