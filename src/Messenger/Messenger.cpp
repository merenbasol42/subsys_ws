#include <Arduino.h>
#include <References/Pin.h>

#include "Messenger.h"

#include "./References.h"

void error_happend(){
    digitalWrite(INTERNAL_LED_PIN, HIGH);    
}

Messenger* Messenger::instance = nullptr;

Messenger::Messenger() {    
    instance = this;
        
    Serial.begin(SERIAL_BAUDRATE);
    set_microros_serial_transports(Serial);

    // Mesajların initialize edilmesi
    std_msgs__msg__String__init(&this->out_msg);
    geometry_msgs__msg__Quaternion__init(&this->encoder_stats_msg);
    geometry_msgs__msg__Vector3__init(&this->cmd_vel_msg);
    
    cmd_vel_cb = nullptr;
}

Messenger::~Messenger() {
    deinitialize();
}

void Messenger::connect() {
    pinMode(INTERNAL_LED_PIN, OUTPUT);

    bool state = LOW;
    while (rmw_uros_ping_agent(PING_TIMEOUT, 1)) {
        // Agent bağlantısı sağlanana kadar 100ms bekle
        digitalWrite(
            INTERNAL_LED_PIN, 
            state
        );
        state = !state;
    }

    digitalWrite(
        INTERNAL_LED_PIN, 
        LOW
    );
    // Bağlantı sağlandığında ROS bileşenlerini initialize et
    initialize();
}

/*
 * Subscription callback fonksiyonu
 */
void Messenger::uros_cmd_vel_cb(const void *msgin) {
    geometry_msgs__msg__Vector3* msg = (geometry_msgs__msg__Vector3*) msgin;
    instance->cmd_vel_cb(Velocity(msg->x, msg->y));
}

/*
 * ROS bileşenlerinin initialize edilmesi
 */
void Messenger::initialize() {
    this->allocator = rcl_get_default_allocator();
    this->init_options = rcl_get_zero_initialized_init_options();
    if (rcl_init_options_init(&this->init_options, this->allocator) != RCL_RET_OK) {
        error_happend();
        return;
    }
    rcl_init_options_set_domain_id(&this->init_options, ROS_DOMAIN_ID);
    if (rclc_support_init_with_options(&this->support, 0, NULL, &this->init_options, &this->allocator) != RCL_RET_OK) {
        error_happend();
        return;
    }
    if (rclc_node_init_default(&this->node, NODE_NAME, NAME_SPACE, &this->support) != RCL_RET_OK) {
        error_happend();
        return;
    }
  
    if (rclc_publisher_init(
            &this->out_pubber,
            &this->node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
            "/mctrl/out", &rmw_qos_profile_default) != RCL_RET_OK) {
        error_happend();
        return;
    }
    if (rclc_publisher_init(
            &this->encoder_pubber,
            &this->node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Quaternion),
            "/mctrl/enc_stats", &rmw_qos_profile_default
        ) != RCL_RET_OK) {
        error_happend();
        return;
    }
    if (rclc_subscription_init(
            &this->cmd_vel_subber,
            &this->node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Vector3),
            "/mctrl/cmd_vel", &rmw_qos_profile_default
        ) != RCL_RET_OK) {
        error_happend();
        return;
    }
    
    this->executor = rclc_executor_get_zero_initialized_executor();
    if (rclc_executor_init(&this->executor, &this->support.context, HANDLES_NUM, &this->allocator) != RCL_RET_OK) {
        error_happend();
        return;
    }
    
    if (
        rclc_executor_add_subscription(
            &this->executor,
            &this->cmd_vel_subber,
            &this->cmd_vel_msg,
            &Messenger::uros_cmd_vel_cb,
            ON_NEW_DATA) != RCL_RET_OK
        ) {
        error_happend();
        return;
    }
}

/*
 * Kaynakların temizlenmesi
 */
void Messenger::deinitialize() {
    std_msgs__msg__String__fini(&out_msg);
    geometry_msgs__msg__Quaternion__fini(&encoder_stats_msg);
    geometry_msgs__msg__Vector3__fini(&cmd_vel_msg);

    if (rcl_publisher_fini(&this->out_pubber, &this->node) != RCL_RET_OK) {
        error_happend();
    }
    if (rcl_publisher_fini(&this->encoder_pubber, &this->node) != RCL_RET_OK) {
        error_happend();
    }
    if (rcl_subscription_fini(&this->cmd_vel_subber, &this->node) != RCL_RET_OK) {
        error_happend();
    }
    if (rcl_node_fini(&this->node) != RCL_RET_OK) {
        error_happend();
    }
    if (rclc_executor_fini(&this->executor) != RCL_RET_OK) {
        error_happend();
    }
    if (rclc_support_fini(&this->support) != RCL_RET_OK) {
        error_happend();
    }
    if (rcl_init_options_fini(&this->init_options) != RCL_RET_OK) {
        error_happend();
    }
}

void Messenger::set_cmd_vel_cb_func(void (*func)(Velocity)) {
    this->cmd_vel_cb = func;
}

void Messenger::pub_encoder_stats(EncoderStats stats) {
    this->encoder_stats_msg.x = stats.dap.left;
    this->encoder_stats_msg.y = stats.dap.right;
    this->encoder_stats_msg.z = stats.vel.left;
    this->encoder_stats_msg.w = stats.vel.right;

    if (rcl_publish(
            &this->encoder_pubber,
            &this->encoder_stats_msg,
            NULL) != RCL_RET_OK) {
        error_happend();
    }
}

void Messenger::pub_output(String text) {
    const char* char_data = text.c_str();
    size_t text_size = text.length() + 1;
    
    if (text_size > out_msg.data.capacity) {
        if (out_msg.data.data != nullptr) {
            free(this->out_msg.data.data);
        }
        this->out_msg.data.data = (char *)malloc(text_size);
        if (this->out_msg.data.data == nullptr) {
            error_happend();
            return;
        }
        this->out_msg.data.capacity = text_size;
    }
    
    memcpy(this->out_msg.data.data, char_data, text_size);
    out_msg.data.size = text.length();

    if (rcl_publish(
        &this->out_pubber,
        &this->out_msg,
        NULL) != RCL_RET_OK) {
            error_happend();
    }
}

void Messenger::spin_some() {
    rclc_executor_spin_some(
        &this->executor,
        RCL_MS_TO_NS(SPIN_TIMEOUT)
    );
}

