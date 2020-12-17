#include <ros/ros.h>
#include <vector>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Pose.h>
#include <actionlib/client/simple_action_client.h>
#include "add_markers/Inform.h"

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");
  // Create a handle
  ros::NodeHandle n;
  ros::ServiceClient callMarker = n.serviceClient<add_markers::Inform>("/inform");
  add_markers::Inform goal_data;

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  std::vector< std::vector<float> > point = {
    { -6.3, 0.05},
    { 6.85, 1.94}
  };

  for(int i=0;i<2;i++){
    // set up the frame parameters
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    int goal_reach = 0;
    // Define a position and orientation for the robot to reach
    // i= 0 pick up, i = 1 drop off
    goal.target_pose.pose.position.x = point[i][0];
    goal.target_pose.pose.position.y = point[i][1];
    goal.target_pose.pose.position.z = 0;
    goal.target_pose.pose.orientation.x = 0;
    goal.target_pose.pose.orientation.y = 0;
    goal.target_pose.pose.orientation.z = 0;
    goal.target_pose.pose.orientation.w = 1;
    // service parameter
    goal_data.request.px = point[i][0];
    goal_data.request.py = point[i][1];
    goal_data.request.pz = 0;
    goal_data.request.ox = 0;
    goal_data.request.oy = 0;
    goal_data.request.oz = 0;
    goal_data.request.ow = 1;
    goal_data.request.type = i;
    goal_data.request.reach = goal_reach;
    // send goal pose before moving
	callMarker.call(goal_data);
    // Send the goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait an infinite time for the results
    ac.waitForResult();

    // Check if the robot reached its goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    	goal_reach = 1;
      	goal_data.request.reach = goal_reach;
    	callMarker.call(goal_data);
    	ROS_INFO("Hooray, the robot reaches the goal, %f",goal.target_pose.pose.position.x);
    }
    else ROS_INFO("Fail to move to the goal!! Check if there are obstacls on the way.");

    //
    ros::Duration(10).sleep();
  }
  return 0;
}