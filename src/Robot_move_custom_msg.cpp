#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <robot_move/DesiredPose.h>  
#include <sstream>
ros::Publisher DesiredPose_publisher;

const double pi=M_PI;
void publishDesiredPose(double x, double y, double Dtheta, bool command_Desired)
{
	
   robot_move::DesiredPose DesiredPose;
	
	
   DesiredPose.xDesired = x;
   DesiredPose.yDesired = y;
   DesiredPose.thetaDesired = Dtheta;
   DesiredPose.commandDesired = command_Desired;
   
     ros::Rate loop_rate(10);
   
	   DesiredPose_publisher.publish(DesiredPose);
	   ros::spinOnce();
	   loop_rate.sleep();
	 

}


int main(int argc, char **argv)
{

	   ros::init(argc, argv, "robot_move_node");
	   ros::NodeHandle n;
	  
	  DesiredPose_publisher = n.advertise<robot_move::DesiredPose>("/DesiredPose", 1000);
	   
		ros::Rate loop_rate(1.0);
	
	
		 while(ros::ok()){
				publishDesiredPose(2.0, 2.0, 0,0);
			    ros::spinOnce();
				loop_rate.sleep();
	   //rotate(degrees_to_radians(10.0), degrees_to_radians(2.0), 0);   
	   //rotate(0.2, 3.14,0);

	}
	return 0;
}

//----------------------------------------------------------------------

