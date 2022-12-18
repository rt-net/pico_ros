#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joy.hpp"


float linear_x,angular_z;


void callback(const sensor_msgs::msg::Joy::SharedPtr data)
{
  linear_x = data->axes[1]*1000.0;
  angular_z = data->axes[3]*500.0;
}


int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto direction_controller = rclcpp::Node::make_shared("direction_controller");
  auto subscriber = direction_controller->create_subscription<sensor_msgs::msg::Joy>("/joy",1,callback);

  auto node =rclcpp::Node::make_shared("twist_publisher");
  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",1);

  rclcpp::WallRate loop(100);

  while(rclcpp::ok()){
    auto msg = geometry_msgs::msg::Twist(); 
    msg.linear.x = linear_x;
    msg.angular.z = angular_z;
    publisher->publish(msg);
    rclcpp::spin_some(direction_controller);
    loop.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
