#pragma once

#include "GPIO_Base.hxx"

namespace sillbird {

class GPIO_Input : public GPIO_Base {
public:
    GPIO_Input(const gpio_num_t pin, const bool activeLow = false);
    GPIO_Input();
    esp_err_t init(const gpio_num_t pin, const bool activeLow = false);
    int read();

    esp_err_t enablePullup();
    esp_err_t disablePullup();
    esp_err_t enablePulldown();
    esp_err_t disablePulldown();
    esp_err_t enablePullupPulldown();
    esp_err_t disablePullupPulldown();
    
    esp_err_t enableInterrupt(gpio_int_type_t intteruptType);

    esp_err_t setEventHandler(esp_event_handler_t eventHandler);

    static void IRAM_ATTR gpio_isr_callback(void* args);

private:
    esp_err_t _init(const gpio_num_t pin, const bool activeLow);

    bool m_event_handler_set;
    static bool m_interrupt_service_installed;
};

}
