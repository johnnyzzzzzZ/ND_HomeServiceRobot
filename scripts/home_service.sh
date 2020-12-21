#!/bin/sh
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find add_markers)/../maps/myworld2.world" &
sleep 10
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find add_markers)/../maps/myMMap.yaml" &
sleep 30
xterm  -e  " roslaunch turtlebot_rviz_launchers  view_navigation.launch" &
sleep 20
xterm  -e  " rosrun add_markers add" &
sleep 5
xterm  -e  " rosrun pick_objects mark"
