#!/bin/sh
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find add_markers)/maps/myworld2.world" &
sleep 20
xterm  -e  " source devel/setup.bash; rosrun gmapping slam_gmapping" &
sleep 10
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_rviz_launchers  view_navigation.launch" &
sleep 10
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_teleop keyboard_teleop.launch"
