#pragma once
#include "driver/gpio.h"

namespace sillbird {

class GPIO_Base {
protected:
    gpio_num_t m_pin;
    bool m_active_low;
};

class GPIO_Input : public GPIO_Base {
public:
    GPIO_Input(const gpio_num_t pin, const bool activeLow = false);
    GPIO_Input();
    esp_err_t init(const gpio_num_t pin, const bool activeLow = false);
    int read();

private:
    esp_err_t _init(const gpio_num_t pin, const bool activeLow);
};

class GPIO_Output : public GPIO_Base {
public:
    GPIO_Output(const gpio_num_t pin, const bool activeLow = false);
    GPIO_Output();
    esp_err_t init(const gpio_num_t pin, const bool activeLow = false);
    esp_err_t on();
    esp_err_t off();
    esp_err_t toggle();
    esp_err_t setLevel(int level);

public:
    int m_level = 0;
    esp_err_t _init(const gpio_num_t pin, const bool activeLow);
};

}
