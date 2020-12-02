#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Pose.h>
#include "add_markers/Inform.h"


class goalSubscriber{
public:
	// constructor
	goalSubscriber():
	marker_pub(n.advertise<visualization_msgs::Marker>("visualization_marker", 1)),
	call_goal(n.advertiseService("/inform", &goalSubscriber::goal_inform, this))
	{
	}
	// service callback
	bool goal_inform(add_markers::Inform::Request& req, add_markers::Inform::Response& res){
		// create marker
		uint32_t shape = visualization_msgs::Marker::CUBE;
		visualization_msgs::Marker marker;
		// Set the frame ID and timestamp.  See the TF tutorials for information on these.
		marker.header.frame_id = "map";
		marker.header.stamp = ros::Time::now();
		// Set the namespace and id for this marker.  This serves to create a unique ID
		// Any marker sent with the same namespace and id will overwrite the old one
		marker.ns = "basic_shapes";
		marker.id = 0;
		// Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
		marker.type = shape;
		// Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
		marker.action = visualization_msgs::Marker::ADD;
		// read pose
		marker.pose.position.x = req.px;
		marker.pose.position.y = req.py;
		marker.pose.position.z = req.pz;
		marker.pose.orientation.x = req.ox;
		marker.pose.orientation.y = req.oy;
		marker.pose.orientation.z = req.oz;
		marker.pose.orientation.w = req.ow;
		// Set the scale of the marker -- 1x1x1 here means 1m on a side
		marker.scale.x = 1.0;
		marker.scale.y = 1.0;
		marker.scale.z = 1.0;
		// Set the color -- be sure to set alpha to something non-zero!
		marker.color.r = 0.0f;
		marker.color.g = 1.0f;
		marker.color.b = 0.0f;
		marker.color.a = 1.0;

		marker.lifetime = ros::Duration();
		// Publish the marker
		while (marker_pub.getNumSubscribers() < 1)
		{
			if (!ros::ok())
			{
				return 0;
			}
			ROS_WARN_ONCE("Please create a subscriber to the marker");
		}
		// move to pick up point
		if (req.type == 0 && req.reach == 0){
			marker_pub.publish(marker);	
          	ROS_INFO("case1");
		}
		// reach pick up point
		else if (req.type == 0 && req.reach == 1){
			marker.action = visualization_msgs::Marker::DELETE;
			marker_pub.publish(marker);
          	ROS_INFO("case2");
		}
		// move to drop off point
		else if (req.type == 1 && req.reach == 0){
			marker.action = visualization_msgs::Marker::DELETE;
			marker_pub.publish(marker);
          	ROS_INFO("case3");
		}
		// reach drop off point
		else if (req.type == 1 && req.reach == 1){
			marker_pub.publish(marker);
          	ROS_INFO("case4");
		}
		return true;
	}
protected:
	ros::NodeHandle n;
	ros::Publisher marker_pub;
	ros::ServiceServer call_goal;
};


int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  goalSubscriber goalSubObject;
  ros::spin();
  return 0;
  
}