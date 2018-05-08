#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/String.h"
#include <sstream>

ros::Publisher velocity_publisher;
ros::Subscriber robotodometry_sub;

double x=0,y=0,theta=0;
double xd=0,yd=0,thetad=0;
double xdn=0,ydn=0,thetadn=0;
double beta=0,angle=0*M_PI/2;

double Dx=1.0; //publishing x span

const double pi=M_PI;
//----------------------------------------------------------------------
void publish_angle(){
	
	double a=pi/4;
//*********************************	
	if((angle-theta)>0){
	double alpha=beta-theta;
		if(abs(alpha)<a){
			beta=theta+a;
			if(beta>angle)beta=angle;
		}
		else beta=theta;
	}
//*********************************	
	else if((angle-theta)<0){
		double alpha=beta-theta;
		if(abs(alpha)<a){
			beta=theta-a;
			if(beta<angle)beta=angle;
		}
		else beta=theta;
	}
//*********************************	
	else{

	}
//*********************************	
}

//----------------------------------------------------------------------
void publish_trajectory(){
	
 double d=0.2; //0.4
	
 double p=sqrt((xd-x)*(xd-x)+(y-yd)*(y-yd));
 double s= atan2((y-yd),(x-xd));
 if (p<d) {
	 xd=x+d*cos(theta);
	 if(xd>Dx) xd=Dx;
	 
	 yd=0;//  publishing trajectory >>>y=-x^2 
	 thetad=0;//thetad;
 }
 
 else {
 }
 
}
//----------------------------------------------------------------------
void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
 {
	x=msg->pose.pose.position.x;
	y=msg->pose.pose.position.y;
	theta=msg->pose.pose.orientation.z;
	
	  double q0 = msg->pose.pose.orientation.w;
	  double q1 = msg->pose.pose.orientation.x;
	  double q2 = msg->pose.pose.orientation.y;
	  double q3 = msg->pose.pose.orientation.z;
	  double omega = msg->twist.twist.angular.z;
	  double psi = atan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3));
	  theta=psi;
	 
    ROS_INFO("Odometry=> X: [%f],Y: [%f],Theta: [%f]", x,y,theta*180/M_PI);
    
   //ROS_INFO("Seq: [%d]", msg->header.seq);
   //ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
   //ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
   //ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
 }

//----------------------------------------------------------------------
void move(double xd, double yd, double thetad, double motionCommand)
{
	
   geometry_msgs::Twist vel_msg;	
   vel_msg.linear.x = xd;
	
   vel_msg.linear.y =yd;
   vel_msg.linear.z =motionCommand;
   
   vel_msg.angular.x = 0;
   vel_msg.angular.y = 0;
   vel_msg.angular.z = thetad;
   
     ros::Rate loop_rate(10);
   
	   velocity_publisher.publish(vel_msg);

	   ros::spinOnce();
	   loop_rate.sleep();
ROS_INFO("Desired: [X= (%f),Y= (%f),Theta= (%f)]\n",xd,yd,thetad*180/pi);
	   
}

//**********************************************************************

int main(int argc, char **argv)
{

	   ros::init(argc, argv, "robot_move_node");
	   ros::NodeHandle n;
	   
	   velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	   robotodometry_sub = n.subscribe("odom", 1000, chatterCallback);
	   
	   ros::Rate loop_rate(10.0);
	

		 while(ros::ok()){
			 
			    publish_trajectory();
				move(xd, yd, thetad, 0);
				
				//publish_angle();
				//move(xd, yd, beta, 1);
				
			    ros::spinOnce();
				loop_rate.sleep();

		 }
	return 0;
}

//**********************************************************************


