#include "GPIO_Input.hxx"
#include <cstdint>

namespace sillbird {

bool GPIO_Input::m_interrupt_service_installed{false};

ESP_EVENT_DEFINE_BASE(INPUT_EVENTS);

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


esp_err_t GPIO_Input::enablePullup() {
    return gpio_set_pull_mode(m_pin, GPIO_PULLUP_ONLY);
}

esp_err_t GPIO_Input::disablePullup() {
    return gpio_set_pull_mode(m_pin, GPIO_FLOATING);
}

esp_err_t GPIO_Input::enablePulldown() {
    return gpio_set_pull_mode(m_pin, GPIO_PULLDOWN_ONLY);
}

esp_err_t GPIO_Input::disablePulldown() {
    return gpio_set_pull_mode(m_pin, GPIO_FLOATING);
}

esp_err_t GPIO_Input::enablePullupPulldown() {
    return gpio_set_pull_mode(m_pin, GPIO_PULLUP_PULLDOWN);
}

esp_err_t GPIO_Input::disablePullupPulldown() {
    return gpio_set_pull_mode(m_pin, GPIO_FLOATING);
}

esp_err_t GPIO_Input::enableInterrupt(gpio_int_type_t intteruptType) {
    esp_err_t status{ESP_OK};

    if (m_active_low) {
	switch (intteruptType) {
	    case GPIO_INTR_POSEDGE:
		intteruptType = GPIO_INTR_NEGEDGE;
	    break;
	    case GPIO_INTR_NEGEDGE:
		intteruptType = GPIO_INTR_POSEDGE;
	    break;
	    case GPIO_INTR_LOW_LEVEL:
		intteruptType = GPIO_INTR_HIGH_LEVEL;
	    break;
	    case GPIO_INTR_HIGH_LEVEL:
		intteruptType = GPIO_INTR_LOW_LEVEL;
	    break;
	    default:
	    break;
	}
    }

    if (!m_interrupt_service_installed) {
	status = gpio_install_isr_service(0);
	if (ESP_OK == status) {
	    m_interrupt_service_installed = true;
	}
    }

    if (ESP_OK == status) {
	status = gpio_set_intr_type(m_pin, intteruptType);
    }

    if (ESP_OK == status) {
	status = gpio_isr_handler_add(m_pin, gpio_isr_callback, (void*) m_pin);
    }

    return status;
}

esp_err_t GPIO_Input::setEventHandler(esp_event_handler_t eventHandler) {
    esp_err_t status{ESP_OK};

    status = esp_event_handler_instance_register(INPUT_EVENTS, m_pin, eventHandler, 0, nullptr);

    if (ESP_OK == status) {
	m_event_handler_set = true;
    }

    return status;
}

void IRAM_ATTR GPIO_Input::gpio_isr_callback(void* args) {
    int32_t pin = reinterpret_cast<int32_t>(args);

    esp_event_isr_post(INPUT_EVENTS, pin, nullptr, 0, nullptr);
}

}
