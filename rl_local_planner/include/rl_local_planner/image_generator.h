/*
 * @name	 	  image_generator.h
 * @brief	 	  An image is generated from laserscan data and waypoints on the path
 * @author  	Ronja Gueldenring
 * @date 	  	2019/04/05
 **/

#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <rl_msgs/StateImageGenerationSrv.h>
#include <rl_msgs/Waypoint.h>
#include <iostream>
#include <nav_msgs/OccupancyGrid.h>
#include <geometry_msgs/Point.h>
#include <tf/transform_listener.h>
#include <pcl/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h> 
#include <pcl/conversions.h>
#include <sensor_msgs/PointCloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <laser_geometry/laser_geometry.h>




namespace rl_image_generator {
  /**
   * This class generates an image from laserscan data and waypoints on the path.
   */
  class ImageGenerator {
    public:
      /**
       * @brief  Constructor f
       */
      ImageGenerator(const ros::NodeHandle& node_handle);
      virtual ~ImageGenerator() = default;
    protected:
      ros::NodeHandle nh_;
    private:
      int img_width_pos_;           //number of height cells in positive direction
      int img_width_neg_;           //number of height cells in negative direction
      int img_height_;              //number of height cells in negative direction
      float resolution_;            //resolution in m/cell
      tf::TransformListener tf_;    

      /**
       * set_path_service + path_callback_
       * @brief Service, that generates image from most recent scan and waypoint.
       */
      ros::ServiceServer get_image_service_;
      bool get_img_callback_(rl_msgs::StateImageGenerationSrv::Request& request, rl_msgs::StateImageGenerationSrv::Response& response);

      /**
       * @brief Image is generated by adding obstacles retrieved 
       * from the laser scans and adding a line from waypoint
       * to waypoint.
       * @param scan laser scan that has to be added to the image.
       * @wp waypoint vector that has to be added to the image.
       **/
      nav_msgs::OccupancyGrid generate_image(sensor_msgs::LaserScan& scan, rl_msgs::Waypoint& wp);
       /**
       * @brief The index of the image for point (x, y) in [m] is computed.
       * @param x x-position
       * @param y y-position
       * @return image index
       */
      int point_to_index_(double x, double y);

      /**
       * @brief Adding scan and occlusions to the image
       * @param image Image, where to add the scan
       * @param scan Scan, that should be added to the image.
       */
      void add_scan_to_img_(std::vector<int8_t>& image, sensor_msgs::LaserScan& scan);

      /**
       * @brief Adding goal point as square
       * @param image Image, where to add the square
       * @param x x-position of goal.
       * @param y y-position of goal.
       */
      void add_goal_point(std::vector<int8_t>& image, float x_goal, float y_goal);

      /**
       * @brief Adding Line to the image (Bresenham)
       * @param image Image, where to add the line
       * @param x1 x-position of first line point.
       * @param y1 y-position of first line point.
       * @param x2 x-position of second line point.
       * @param y2 y-position of second line point.
       */
      void add_line_to_img_(std::vector<int8_t>& image, float x1, float y1, float x2, float y2, int value);

      /**
       * @brief Mean square distance.
       * @param x x-position
       * @param y x-position
       * @retur sqrt(x^2 + y^2)
       */
      double metric_dist(double x, double y);

      std::string robot_frame_ = "base_footprint";

  };
};
#endif /* IMAGE_GENERATOR_H */