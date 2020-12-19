#!/bin/sh
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find add_markers)/maps/myworld2.world" &
sleep 10
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find add_markers)/maps/myMMap.yaml" &
sleep 30
xterm  -e  " source devel/setup.bash; roslaunch turtlebot_rviz_launchers  view_navigation.launch"
