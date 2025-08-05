#include "sillbirdGPIO.hxx"

namespace sillbird {

GPIO_Input::GPIO_Input(const gpio_num_t pin, const bool activeLow) {
    _init(pin, activeLow);
}

GPIO_Input::GPIO_Input(){}

esp_err_t GPIO_Input::init(const gpio_num_t pin, const bool activeLow) {
    return _init(pin, activeLow);
}

int GPIO_Input::read() {
    return m_active_low ? !gpio_get_level(m_pin) : gpio_get_level(m_pin);
}

esp_err_t GPIO_Input::_init(const gpio_num_t pin, const bool activeLow) {
    esp_err_t status {ESP_OK};

    m_active_low = activeLow;
    m_pin = pin;

    gpio_config_t cfg;
    cfg.pin_bit_mask = 1ULL << pin;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_POSEDGE;

    status |= gpio_config(&cfg);

    return status;
}

GPIO_Output::GPIO_Output(const gpio_num_t pin, const bool activeLow) {
    _init(pin, activeLow);
}

GPIO_Output::GPIO_Output(){}

esp_err_t GPIO_Output::init(const gpio_num_t pin, const bool activeLow) {
    return _init(pin, activeLow);
}

esp_err_t GPIO_Output::on() {
    m_level = true;
    return gpio_set_level(m_pin, m_active_low ? 0 : 1);
}

esp_err_t GPIO_Output::off() {
    m_level = false;
    return gpio_set_level(m_pin, m_active_low ? 1 : 0);
}

esp_err_t GPIO_Output::toggle() {
    m_level = !m_level;
    return gpio_set_level(m_pin, m_level);
}

esp_err_t GPIO_Output::setLevel(int level) {
    m_level = m_active_low ? !level : level;
    return gpio_set_level(m_pin, m_level);
}

esp_err_t GPIO_Output::_init(const gpio_num_t pin, const bool activeLow) {
    esp_err_t status{ESP_OK};

    m_active_low = activeLow;
    m_pin = pin;

    gpio_config_t cfg;
    cfg.pin_bit_mask = 1ULL << pin;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;

    status |= gpio_config(&cfg);

    return status;
}

}
