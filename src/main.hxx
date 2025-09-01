#pragma once

#include <array>
#include <iostream>

#include "esp_event_base.h"

#include "GPIO_Input.hxx"
#include "GPIO_Output.hxx"
#include "Servo.hxx"

class Main final {
public:
    void run();
    void setup();

    esp_event_loop_handle_t gpio_loop_handle{};
    std::array<sillbird::GPIO_Output, 10> leds = {{
        { GPIO_NUM_15, true },
        { GPIO_NUM_2, true },
        { GPIO_NUM_0, true },
        { GPIO_NUM_4, true },
        { GPIO_NUM_5, true },
        { GPIO_NUM_18, true },
        { GPIO_NUM_19, true },
        { GPIO_NUM_21, true },
        { GPIO_NUM_22, true },
        { GPIO_NUM_23, true },
    }};
    sillbird::Servo servo{
        {
            .max_angle = 180,
            .min_width_us = 500,
            .max_width_us = 2500,
            .freq = 50,
            .timer_number = LEDC_TIMER_0,
            .channels {
                .servo_pin = {
                    GPIO_NUM_14,
                },
                .ch = {
                    LEDC_CHANNEL_0,
                },
            },
            .channel_number = 1,
        },
        LEDC_LOW_SPEED_MODE
    };
    //sillbird::GPIO_Output motor { GPIO_NUM_14, true };
    sillbird::GPIO_Input button { GPIO_NUM_13 };
    sillbird::GPIO_Input button2 { GPIO_NUM_34 };

    static void button_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    static void button2_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    static void task_custom_event(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    static QueueHandle_t button_event_queue;
    static void gpio_task_example(void *arg);
};
