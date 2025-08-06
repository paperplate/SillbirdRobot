#pragma once

#include "driver/gpio.h"
#include "esp_event.h"

namespace sillbird {

ESP_EVENT_DECLARE_BASE(INPUT_EVENTS);

class GPIO_Base {
protected:
    gpio_num_t m_pin;
    bool m_active_low;
};

}
