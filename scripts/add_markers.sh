#!/bin/sh
xterm  -e  " source devel/setup.sh; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find add_markers)/../maps/myworld2.world" &
sleep 10
xterm  -e  " source devel/setup.sh; roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find add_markers)/../maps/myMMap.yaml" &
sleep 10
xterm  -e  " source devel/setup.sh; roslaunch turtlebot_rviz_launchers  view_navigation.launch" &
sleep 20
xterm  -e  " source devel/setup.sh; rosrun add_markers show" 
