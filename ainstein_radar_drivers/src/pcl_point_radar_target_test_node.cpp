#include "ainstein_radar_drivers/pcl_point_radar_target.h"

int main( int argc, char** argv )
{
  // Initialize ROS node:
  ros::init( argc, argv, "pcl_point_radar_target_node" );
  ros::NodeHandle node_handle;
  ros::NodeHandle node_handle_private( "~" );

  ainstein_radar_drivers::PointRadarTarget point;
  
  ros::spin();

  return 0;
}

