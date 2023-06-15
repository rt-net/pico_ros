# Copyright 2023 RT Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import Command
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    declare_arg_run_jsp_gui = DeclareLaunchArgument(
        'run_jsp_gui',
        default_value='false',
        description='Set true to run joint_state_publisher_gui node.')

    xacro_file = os.path.join(
        get_package_share_directory('pico_description'),
        'urdf',
        'pico.urdf.xacro')
    params = {'robot_description': Command(['xacro ', xacro_file]),
              }

    rviz_config_file = get_package_share_directory('pico_description') + '/rviz/urdf.rviz'

    robot_state_pub_node = Node(
      package="robot_state_publisher",
      executable="robot_state_publisher",
      output="both",
      parameters=[params, {'ignore_timestamp': True}],
    )

    joint_state_pub_gui_node = Node(
      package="joint_state_publisher_gui",
      executable="joint_state_publisher_gui",
      output="screen",
      condition=IfCondition(LaunchConfiguration('run_jsp_gui'))
    )

    rviz_node = Node(
      package="rviz2",
      executable="rviz2",
      name="rviz2",
      output="log",
      arguments=["-d", rviz_config_file]
    )

    nodes = [
      declare_arg_run_jsp_gui,
      rviz_node,
      robot_state_pub_node,
      joint_state_pub_gui_node
    ]

    return LaunchDescription(nodes)
