#include <QCoreApplication>
#include "serial.h"
#include <math.h>
#include<iostream>
#include<thread>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Serial s;
//    int last_dist = 0, last_angle = 0, last_bearing = 0;
//    while(true)
//    {
//        int angle = get_ball_angle();
//        int dist = get_ball_dist();
//        int bearing = s.get_bearing();
//        if((std::abs((last_bearing - bearing) - (last_angle - angle)) > 2))
//        {
//            calculate_route(angle, bearing, dist);
//        }

//        last_bearing = bearing;
//        last_angle = angle;
//    }
    while(printf("%s",s.readln().c_str())){std::this_thread::sleep_for(std::chrono::milliseconds(600));};
    std::cout<<"calibration finished"<<std::endl;
    s.println("move 20 100 \n");
    s.println("turn 0 90");
    s.println("start");

    return a.exec();
}
