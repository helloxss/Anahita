#include "ros/ros.h"
#include <sensor_msgs/Imu.h>
#include <std_msgs/Float64.h>

#define TO_DEG(x) (x * 57.2957795131)

std_msgs::Float64 imu_yaw;
std_msgs::Float64 imu_roll;
std_msgs::Float64 imu_pitch;
   
ros::Publisher imu_yaw_pub;
ros::Publisher imu_roll_pub;
ros::Publisher imu_pitch_pub;

void imu_data_callback(sensor_msgs::Imu msg)
{
  float q0 = msg.orientation.w;
  float q1 = msg.orientation.x;
  float q2 = msg.orientation.y;
  float q3 = msg.orientation.z;
  // convert from quaternion to euler angle (yaw)
  imu_yaw.data = TO_DEG(atan2(2*q1*q2-2*q0*q3, 2*q0*q0+2*q1*q1-1));
  // convert form quaternion to euler angle (roll)
  imu_roll.data = TO_DEG(atan2(2 * q2 * q3 - 2 * q0 * q1, 2 * q0 * q0 + 2 * q3 * q3 - 1));
  // convert form quaternion to euler angle (pitch)
  imu_pitch.data = TO_DEG(-asin(2 * q1 * q3 + 2 * q0 * q2));
   
	ROS_INFO("PX4 IMU Data (Roll, Pitch, Yaw) = (%.4f, %.4f, %.4f)", imu_roll.data, imu_pitch.data, imu_yaw.data);
   
  imu_yaw_pub.publish(imu_yaw);  
  imu_roll_pub.publish(imu_roll);
  imu_pitch_pub.publish(imu_pitch);
}

int main(int argc, char **argv)
{
  // initiliazing ROS node
  ros::init(argc, argv, "px4");
  ros::NodeHandle nh;

  // initializing publishers
  std::string imu_yaw_pub_topic = "/mavros/imu/yaw";
  std::string imu_roll_pub_topic = "/mavros/imu/roll";
  std::string imu_pitch_pub_topic = "/mavros/imu/pitch";
     
  imu_yaw_pub = nh.advertise<std_msgs::Float64>(imu_yaw_pub_topic, 1000);
  imu_roll_pub = nh.advertise<std_msgs::Float64>(imu_roll_pub_topic, 1000);
  imu_pitch_pub = nh.advertise<std_msgs::Float64>(imu_pitch_pub_topic, 1000);

  //initializing subscribers
  ros::Subscriber imu_data_sub = nh.subscribe<sensor_msgs::Imu>("/mavros/imu/data", 1000, &imu_data_callback);

  ros::spin();

  return 0;
}
