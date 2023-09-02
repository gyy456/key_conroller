#include <termios.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <std_msgs/Int16.h>
 
#include <boost/thread/thread.hpp>
#include <ros/ros.h>

ros::Publisher key_pub;
// #define KEYCODE_W 0x77
// #define KEYCODE_A 0x61
// #define KEYCODE_S 0x73
// #define KEYCODE_D 0x64
 
// #define KEYCODE_A_CAP 0x41
// #define KEYCODE_D_CAP 0x44
// #define KEYCODE_S_CAP 0x53
// #define KEYCODE_W_CAP 0x57
void keyboardLoop()
{
    std_msgs::Int16 in;
 
    struct termios new_settings;
    struct termios stored_settings;
    //设置终端参数
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    in.data = getchar();
    tcsetattr(0,TCSANOW,&stored_settings);
        
    key_pub.publish(in);
    
}

int main(int argc, char** argv)
{
    ros::init (argc, argv, "keyboard");
    ros::NodeHandle nh( "~" );
    key_pub = nh.advertise<std_msgs::Int16>("cmd_vel",50);

    ros::Rate rate(0.5);
    while(ros::ok())
        keyboardLoop();

        ros::spinOnce();
        rate.sleep();
    

    // tbk.stopRobot();
    
    return(0);
}