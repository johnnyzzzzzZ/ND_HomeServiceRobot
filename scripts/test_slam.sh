#!/bin/sh
xterm  -e  " source /opt/ros/kinetic/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 20
xterm  -e  " source /opt/ros/kinetic/setup.bash; rosrun gmapping slam_gmapping" &
sleep 10
xterm  -e  " source /opt/ros/kinetic/setup.bash; roslaunch turtlebot_rviz_launchers  view_navigation.launch" &
sleep 10
xterm  -e  " source /opt/ros/kinetic/setup.bash; roslaunch turtlebot_teleop keyboard_teleop.launch"
