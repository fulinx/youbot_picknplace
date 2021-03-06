/**
 * @file      hl_planner.hpp
 * @brief     High-Level Planner executes a list of action instructions
 * @author    Alejandro Bordallo <alex.bordallo@ed.ac.uk>
 * @date      2015-11-03
 * @copyright (MIT) 2015 RAD-UoE Informatics
 */

#ifndef HL_PLANNER_HPP
#define HL_PLANNER_HPP

#include <ros/ros.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

#include <motion_msgs/MoveToPostureAction.h>
#include <motion_msgs/MoveGripperAction.h>
#include <motion_planning_msgs/PlanPickAction.h>
#include <sensing_msgs/DetectObjectAction.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <object_recognition_msgs/RecognizedObjectArray.h>

#include <geometry_msgs/Twist.h>

class HLPlanner {
 public:
  HLPlanner(ros::NodeHandle* nh);
  ~HLPlanner();

  void init();
  void rosSetup();
  void loadParams();

  void execute();
  bool isFinished() {return finished_;}
  static void interrupt(int s);
  static bool isInterrupted() {return interrupted_;}

 private:

  void initLogs();
  void createStateFile();
  void updateLog(std::string action);
  void closeLog();

  void pickA();
  void pickB();
  void gotoA();
  void gotoB();
  void placeA();
  void placeB();
  void dropA();
  void dropB();

  void startDetection();
  void stopDetection();
  void moveLeft();
  void moveRight();
  void stopRobot();
  void initRobot();
  void endRobot();

  void emptyCB(const object_recognition_msgs::RecognizedObjectArray::ConstPtr&
               msg) {;}

  // Flags
  bool finished_;
  bool failed_;
  static bool interrupted_;
  bool enable_alignment_;

  // Constants
  std::string PICK_A;
  std::string PICK_B;
  std::string GOTO_A;
  std::string GOTO_B;
  std::string PLACE_A;
  std::string PLACE_B;
  std::string plan_path_;
  std::string plan_file_;
  std::string log_path_;
  std::string exp_name_;

  int freq_;
  float velocity_;
  float move_time_;
  geometry_msgs::Twist stop_msg_;
  geometry_msgs::Twist move_left_;
  geometry_msgs::Twist move_right_;

  ros::Duration wait_init_;
  ros::Duration wait_;
  ros::Duration wait_detect_;

  std::ofstream state_file_;
  std::ofstream log_file_;

  // Variables
  std::string robot_state_; // A or B
  std::string cube_a_state_; // A, B or R
  std::string cube_b_state_; // A, B or R
  std::vector<std::string> action_list_;
  std::string s_pid;
  char pid [8];

  geometry_msgs::PoseStamped cube_a_pose_;
  geometry_msgs::PoseStamped cube_b_pose_;

  // ROS
  ros::NodeHandle* nh_;
  ros::Subscriber det_sub_;
  ros::Publisher cmd_vel_pub_;
  actionlib::SimpleActionClient<motion_msgs::MoveToPostureAction> posture_ac_;
  actionlib::SimpleActionClient<motion_msgs::MoveGripperAction> gripper_ac_;
  actionlib::SimpleActionClient<motion_planning_msgs::PlanPickAction> pick_ac_;
  actionlib::SimpleActionClient<sensing_msgs::DetectObjectAction> detect_ac_;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> move_ac_;
  motion_msgs::MoveGripperGoal close_;
  motion_msgs::MoveGripperGoal open_;
  motion_msgs::MoveToPostureGoal candle_;
  motion_msgs::MoveToPostureGoal place_;
  motion_msgs::MoveToPostureGoal drop_a_;
  motion_msgs::MoveToPostureGoal drop_b_;
  motion_msgs::MoveToPostureGoal home_;
  motion_planning_msgs::PlanPickGoal pick_;
  sensing_msgs::DetectObjectGoal detect_;
  move_base_msgs::MoveBaseGoal move_a_;
  move_base_msgs::MoveBaseGoal move_b_;
  move_base_msgs::MoveBaseGoal move_a_cube_a_;
  move_base_msgs::MoveBaseGoal move_a_cube_b_;
  move_base_msgs::MoveBaseGoal move_b_cube_a_;
  move_base_msgs::MoveBaseGoal move_b_cube_b_;
};

#endif /* HL_PLANNER_HPP */
