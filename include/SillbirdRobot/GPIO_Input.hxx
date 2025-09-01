#pragma once

#include "GPIO_Base.hxx"
#include "esp_event_base.h"
#include "portmacro.h"
#include "soc/gpio_num.h"

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
    esp_err_t setEventHandler(esp_event_loop_handle_t eventLoop, esp_event_handler_t eventHandler);

    void setQueueHandle(QueueHandle_t eventQueue);

    static void IRAM_ATTR gpio_isr_callback(void* args);

private:
    esp_err_t _init(const gpio_num_t pin, const bool activeLow);
    esp_err_t _clearEventHandlers();

    static bool m_interrupt_service_installed;

    struct interrupt_args {
        bool event_handler_set = false;
        bool custom_event_handler_set = false;
        bool queue_enabled = false;
        gpio_num_t pin;
        esp_event_loop_handle_t custom_event_loop_handle{nullptr};
        QueueHandle_t queue_handle{nullptr};
    } m_interrupt_args;

    bool m_event_handler_set = false;
    esp_event_handler_t m_event_handle = nullptr;
    static portMUX_TYPE m_eventChangeMutex;
};

}
