#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/joy.hpp"


float linear_x,angular_z;


void callback(const sensor_msgs::msg::Joy::SharedPtr data)
{
  if(data->axes[7] != 0 ){
    linear_x = 150*data->axes[7];
  }else if((data->axes[1] > 0.00001) || (data->axes[1] < -0.00001)){
    linear_x = data->axes[1]*1000.0;
  }else{
    linear_x = 0;
  } 
  if(data->axes[6] != 0){
    angular_z = 250*data->axes[6];
  }else if((data->axes[3] > 0.00001) || (data->axes[3] < -0.00001)){ 
    angular_z = data->axes[3]*500.0;
  }else{
    angular_z = 0;
  }
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
