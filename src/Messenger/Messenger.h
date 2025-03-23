#include <Arduino.h>

#include <CommInterfaces/Velocity.h>
#include <CommInterfaces/EncoderStats.h>

/////////////////////
/// For Micro ROS ///
/////////////////////

#include <micro_ros_platformio.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>

/*
 * ROS Message Interfaces
 */
#include <std_msgs/msg/string.h>
#include <geometry_msgs/msg/vector3.h>
#include <geometry_msgs/msg/quaternion.h>

/*
 * Definitions
 */

class Messenger {
private:
    static Messenger* instance; 
    rcl_node_t node;
    rclc_support_t support;
    rcl_init_options_t init_options;
    rclc_executor_t executor;
    rcl_allocator_t allocator;
    
    // Publisher & Subscription objects
    rcl_publisher_t out_pubber;
    rcl_publisher_t encoder_pubber;
    rcl_subscription_t cmd_vel_subber;
    
    // ROS messages
    geometry_msgs__msg__Quaternion encoder_stats_msg;
    geometry_msgs__msg__Vector3 cmd_vel_msg;
    std_msgs__msg__String out_msg;
    
    // Callback function pointer for cmd_vel (Command Velocity)
    void (*cmd_vel_cb) (Velocity);
    
    // Static callback wrapper
    static void uros_cmd_vel_cb(const void * msg);
    
    // Initialization and deinitialization methods
    void initialize();
    void deinitialize();

public: 
    Messenger();
    ~Messenger();
    
    // Bağlantı sağlanana kadar bekleyip initialize metodunu çağırır.
    void connect();
    
    void set_cmd_vel_cb_func(void (*func)(Velocity));
    void pub_encoder_stats(EncoderStats stats);
    void pub_output(String text);
    
    void spin_some();
};

