# Home Service Robot 

In this simulation, a robot is built to pick up and deliver an object in the map generated by SLAM. We will introduce some important packages used and the parameters chosen to optimize the performance.
## Installation
This package has been tested on Ubuntu 18 && ROS Kinetic. 

Follow the steps below to build the package and run.
1. Install the package
```
cd ~/<catkin workspace directory>/src && git clone https://github.com/johnnyzzzzzZ/ND_HomeServiceRobot.git
```
2. Build the package
```
cd ~/<catkin workspace directory> && catkin_make
```
3. Source the bash file
```
echo "source ~/<catkin workspace directory>/devel/setup.bash" >> ~/.bashrc
```
4. Run the script file in the scripts directory
```
cd $(rospack find add_markers)/../scripts && ./<script file>
```

Use *test_slam.sh* to start SLAM algorithm and generate the map.
Use *home_service.sh* to simulate a robot to an deliver.

## Localiztion
We select the offical ROS package **amcl** (http://wiki.ros.org/amcl#Parameters) for the purpose of localizing our robot. It would be implemented in the **amcl_demo.launch** file. In brief, this algorithm will generate many particles, propagate their motion, and find particles which matches the laser scan data to represent the robot. 
#### Parameters
The parameters are configured in the **myAMCL.xml** file where we use a smaller range of particle number than default, from  200 to 4000, to reduce the compuational cost. For the laser model paramters, we select 12 meters for *laser_max_range* and 60 for *laser_max_beams* to include as many reasonable data as possible. Double the resamping frequency by setting *resample_interval* to 1.
#### Subscribed Topics
* *scan* : laser scan data
* *tf* : transfromation between frames, which are map, odom, and base_link in this case
* *initialpose* : the initial state of robot to initialize the particle filter
* *map* : the map generated by SLAM
#### Published Topics
* *amcl_pose* : the estimated pose of the robot
* *particlecloud* : the set of all poses estimated by the filter
* *tf* : publish the transformation between odom and map

Both of the *map* and *odom* are fixed frames. However, the robot pose in *odom* frame will accumulate the error as time goes on while the pose in *map* frame can be estimated directly by AMCL. Since each node in the tf tree can only have one parent node, we need to publish the transformation between odom and map to cancel out the accumulated error of the pose in odom frame as shown in the figure below.

![amcl](http://wiki.ros.org/amcl?action=AttachFile&do=get&target=amcl_localization.png)

## Mapping
The ROS package **gmapping** (http://wiki.ros.org/gmapping) is used to generate the map. This algorithm is similar to **amcl** since both of them estimate the robot pose with particle filter. However, **gmapping** only has a few partilces for the filter since each particle needs to bring a complicated map with it. 

In the beginning, the algorithm will create particles and propagate their motion. Every time the **gmapping** receives a new laser scan data, it will update the map of each particle and compute their score. This kind of process is called *scan match* in the algorithm. Finally, the node would determine the weight for each particle and do resampling. The map from the particle with the biggest weight would be published to the **map** topic. Each grid cell in the map would be assigned the probability of being occupied which is computed from the times to be visited and hit. According to the probability, the state of cell can be classified into three groups(free, occupied, and unknown). 
#### Parameters
All the parameters used are configured directly in the **slam_gmapping.cpp** file. Every map would have a score which can be computed from the laser scan data and the particle pose. A map with a higher score has more similarities to the ground truth. In optimization, we would compare the score of the map generated with the parameter *minimumScore* to determine whether to use a pose from odometry or a corrected pose including consideration for laser scan data. Higher *minimumScore* could prevent the robot pose jumping in the map.  We also select zero *kernelSize* to simplify the optimization process so the necessary time to process a laser scan data could be reduced to one fifth or more. 

#### Subscribed Topics
* /scan: laser scan data used for scan matching
* /tf: transformation between frames
#### Published Topics
* /map: map data in a matrix form updated periodically


## Navigation
We use the ROS package **move_base** (http://wiki.ros.org/move_base) to accomplish navagation tasks. This node will link together a global plannar and a local plannar to navigate the robot's way to the goal. At first, the global plannar would find a route to the destination given the map provided. When the robot follows the route, the local plannar would make use of the sensor data to modify the route avoiding the obstacle that does not appear on the map.

![move_base configuration](http://wiki.ros.org/navigation/Tutorials/RobotSetup?action=AttachFile&do=get&target=overview_tf.png)

The configuration of the **move_base** node can be shown in the above diagram. The white components are necessary components that are already implemented. The blue components are necessary componens that we need to provide for the **move_base** node.

#### Subscribed topics
* /tf : the transformation between frames
* /odom: the odometry information including linear and angular motion information
* /sensor_topics: some sensor information such as laser scan data or point cloud

#### Published topics
* /cmd_vel: the action command meant for execution by the motor

