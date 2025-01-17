/*
  Copyright <2018-2019> <Ainstein, Inc.>

  Redistribution and use in source and binary forms, with or without modification, are permitted 
  provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this list of 
  conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this list of 
  conditions and the following disclaimer in the documentation and/or other materials provided 
  with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors may be used to 
  endorse or promote products derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ainstein_radar_drivers/radar_target_array_to_point_cloud.h"

namespace ainstein_radar_drivers
{

  RadarTargetArrayToPointCloud::RadarTargetArrayToPointCloud( ros::NodeHandle node_handle,
							      ros::NodeHandle node_handle_private ) :
    nh_( node_handle ),
    nh_private_( node_handle_private ),
    listen_tf_( buffer_tf_ )
  {
    pub_cloud_ = nh_private_.advertise<sensor_msgs::PointCloud2>( "cloud", 10 );
    sub_radar_target_array_ = nh_.subscribe( "radar_in", 10,
					     &RadarTargetArrayToPointCloud::radarTargetArrayCallback,
					     this );
  }

  void RadarTargetArrayToPointCloud::radarTargetArrayCallback( const ainstein_radar_msgs::RadarTargetArray &msg )
  {
    // Get the data frame ID and look up the corresponding tf transform:
    Eigen::Affine3d tf_sensor_to_world;
    if( buffer_tf_.canTransform( "map", msg.header.frame_id, ros::Time( 0 ) ) )
      {
	tf_sensor_to_world =
	  tf2::transformToEigen(buffer_tf_.lookupTransform( "map", msg.header.frame_id, ros::Time( 0 ) ) );
      }
    else
      {
	std::cout << "Timeout while waiting for transform to frame " << msg.header.frame_id << std::endl;
	return;
      }
  
    // Clear the point cloud point vector:
    pcl_cloud_.clear();

    // Iterate through targets and add them to the point cloud:
    PointRadarTarget pcl_point;
    for( auto target : msg.targets )
      {
	radarTargetToPclPoint( target, pcl_point );
	pcl_cloud_.points.push_back( pcl_point );
      }

    pcl_cloud_.width = pcl_cloud_.points.size();
    pcl_cloud_.height = 1;
	
    pcl::toROSMsg( pcl_cloud_, cloud_msg_ );
    cloud_msg_.header.frame_id = msg.header.frame_id;
    cloud_msg_.header.stamp = msg.header.stamp;

    pub_cloud_.publish( cloud_msg_ );
  } 
  
  void RadarTargetArrayToPointCloud::radarTargetToPclPoint( const ainstein_radar_msgs::RadarTarget &target,
							    PointRadarTarget& pcl_point )
  {
    pcl_point.x = cos( ( M_PI / 180.0 ) * target.azimuth ) * cos( ( M_PI / 180.0 ) * target.elevation )
      * target.range;
    pcl_point.y = sin( ( M_PI / 180.0 ) * target.azimuth ) * cos( ( M_PI / 180.0 ) * target.elevation )
      * target.range;
    pcl_point.z = sin( ( M_PI / 180.0 ) * target.elevation ) * target.range;

    pcl_point.snr = target.snr;
    pcl_point.range = target.range;
    pcl_point.speed = target.speed;
    pcl_point.azimuth = target.azimuth;
    pcl_point.elevation = target.elevation;
  }

} // namespace ainstein_radar_drivers
