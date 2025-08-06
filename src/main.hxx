#pragma once

#include <cstdint>
#include <iostream>
#include "GPIO_Input.hxx"
#include "GPIO_Output.hxx"

class Main final {
public:
    void run();
    void setup();

    sillbird::GPIO_Output led { GPIO_NUM_2, true };
    sillbird::GPIO_Input button { GPIO_NUM_13 };
    sillbird::GPIO_Input button2 { GPIO_NUM_14 };

    static void button_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    static void button2_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
};
