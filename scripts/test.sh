#!/bin/sh
xterm  -e  " source devel/setup.sh; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find add_markers)/../maps/myworld2.world"