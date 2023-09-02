#include "nav_msgs/Odometry.h"
#include "std_msgs/Empty.h"
#include <ros/ros.h>
// #include <active_perception/perception_utils.h>
// #include <swarmtal_msgs/drone_onboard_command.h>
#include</home/gyy/robot/devel/include/roborts_msgs/TwistAccel.h>
#include <std_msgs/Int16.h>
#include <tf/transform_listener.h>
#include "gazebo_msgs/GetModelState.h"

// 添加的读取键盘信息的函数
#include <termio.h>
#include <stdio.h>
#include "Eigen/Core"

#include "Eigen/Eigenvalues"
// int scanKeyboard()
// {
 
//  int in;
 
//  struct termios new_settings;
//  struct termios stored_settings;
//     //设置终端参数
//  tcgetattr(0,&stored_settings);
//  new_settings = stored_settings;
//  new_settings.c_lflag &= (~ICANON);
//  new_settings.c_cc[VTIME] = 0;
//  tcgetattr(0,&stored_settings);
//  new_settings.c_cc[VMIN] = 1;
//  tcsetattr(0,TCSANOW,&new_settings);
//  in = getchar();
//  tcsetattr(0,TCSANOW,&stored_settings);
 
//  return in;
 
// }

ros::Publisher cmd_vis_pub, pos_cmd_pub, traj_pub, swarm_,vel_pub_,odom_pub,odom_pub2;
Eigen::Vector3d vel;
nav_msgs::Odometry odom;
// quadrotor_msgs::PositionCommand cmd;
roborts_msgs::TwistAccel cmd;
// Info of generated traj
double x,y,z,yaw=0, yawdot=0;
int key=0;
ros::Time start_time_;

// shared_ptr<PerceptionUtils> percep_utils_;

// Info of replan
bool receive_traj_ = false;
double replan_time_;

// // Executed traj, commanded and real ones
// vector<Eigen::Vector3d> traj_cmd_, traj_real_;

// Data for benchmark comparison
// ros::Time start_time, end_time, last_time;


// Loop correction

// Swarm









void cmdCallback(const ros::TimerEvent& e) {
  // No publishing before receive traj data
  // if (!receive_traj_) return;

  ros::Time time_now = ros::Time::now();
  
  Eigen::Vector3d pos, acc, jer;
  double yaw, yawdot;
   
  //traj为计划的轨迹，如果当前时间还在计划时间之内，则按照计划的轨迹继续施行。
  //如果当前时间大于计划时间，则暂停无人机的运动。
  
    pos[0]=x;
    pos[1]=y;
    pos[2]=z;
    if (key==112) {       //中止的优先级最高
      vel.setZero();
      yawdot=0;
    }
    if(key==119 ) {
     //向x正方向                若本身有速度则继续向前
       vel.setZero();
       vel[0]=0.08;

       
    }
    if (key==115){
      //向x反方向
       vel.setZero();
       vel[0]=-0.08;     
    }
    if (key==97){
      //向左y
      vel.setZero();
      vel[1]=0.08;
    }
    if (key==100|| vel[1]<0){
      //向左y
      vel.setZero();
      vel[1]=-0.08;
    }
    if (key==113){    
      // 若无速度 且要原地旋转
      yawdot=0.01;
    }
    if (key==101||yawdot<0){
      
      yawdot=-0.01;

    }
 
  cmd.twist.linear.x =vel[0];
  cmd.twist.linear.y =vel[1];
  cmd.twist.angular.z =yawdot;     

  cmd.accel.linear.x=0;
  cmd.accel.linear.y=0;
  cmd.accel.angular.z=0;

  vel_pub_.publish(cmd);

}

void keyCallback(const std_msgs::Int16& msg){
  key=msg.data;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "traj_server");
  ros::NodeHandle node;
  ros::NodeHandle nh("~");

  
  ros::Subscriber key_sub = node.subscribe("/keyboard/cmd_vel", 10,keyCallback);  //添加键盘回调
  vel_pub_ = node.advertise<roborts_msgs::TwistAccel>("jackal0/cmd_vel_acc", 10);
  // swarm_pos_cmd_pub =
  //     node.advertise<swarmtal_msgs::drone_onboard_command>("/drone_commander/onboard_command",
  //     50);
  ros::Timer cmd_timer = node.createTimer(ros::Duration(0.01), cmdCallback);

  ros::Duration(1.0).sleep();

  // Control parameter
  // cmd.kx = { 5.7, 5.7, 6.2 };
  // cmd.kv = { 3.4, 3.4, 4.0 };

//   std::cout << start_time.toSec() << std::endl;
//   std::cout << end_time.toSec() << std::endl;

  cmd.twist.linear.x =0;
  cmd.twist.linear.y =0;
  cmd.twist.angular.z =0;     

  cmd.accel.linear.x=0;
  cmd.accel.linear.y=0;
  cmd.accel.angular.z=0;


  // test();
  // Initialization for exploration
  // ros::Duration(2.0).sleep();
  // for (int i = 0; i < 100; ++i) {
  //   cmd.position.z += 0.01;
  //   pos_cmd_pub.publish(cmd);
  //   ros::Duration(0.01).sleep();
  // }
  // for (int i = 0; i < 100; ++i) {
  //   cmd.position.z -= 0.01;
  //   pos_cmd_pub.publish(cmd);
  //   ros::Duration(0.01).sleep();
  // }
  // for (int i = 0; i < 100; ++i) {
  //   cmd.position.z += 0.01;
  //   pos_cmd_pub.publish(cmd);
  //   ros::Duration(0.01).sleep();
  // 

  ros::spin();

  return 0;
}
