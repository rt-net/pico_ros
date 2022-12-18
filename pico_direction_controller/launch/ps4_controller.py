from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
  return LaunchDescription([
#    Node(
#     package="micro_ros_agent",
#     executable="micro_ros_agent",
#     output="screen",
#     arguments=["udp4 --port 8888"]
#    ), 
    Node(
      package="joy_linux",
      executable="joy_linux_node"
    ),
    Node(
      package="pico_direction_controller",
      executable="direction_controller"
    )
   ])

