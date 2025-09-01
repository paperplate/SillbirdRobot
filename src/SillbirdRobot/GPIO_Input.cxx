#include "GPIO_Input.hxx"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "freertos/idf_additions.h"
#include <cstdint>

namespace sillbird {

bool GPIO_Input::m_interrupt_service_installed{false};
portMUX_TYPE GPIO_Input::m_eventChangeMutex = portMUX_INITIALIZER_UNLOCKED;

ESP_EVENT_DEFINE_BASE(INPUT_EVENTS);

GPIO_Input::GPIO_Input(const gpio_num_t pin, const bool activeLow) {
    _init(pin, activeLow);
}

GPIO_Input::GPIO_Input(){}

esp_err_t GPIO_Input::init(const gpio_num_t pin, const bool activeLow) {
    return _init(pin, activeLow);
}

int GPIO_Input::read() {
    return m_active_low ? !gpio_get_level(m_interrupt_args.pin) : gpio_get_level(m_interrupt_args.pin);
}

esp_err_t GPIO_Input::_init(const gpio_num_t pin, const bool activeLow) {
    esp_err_t status {ESP_OK};

    m_active_low = activeLow;
    m_interrupt_args.pin = pin;

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
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_PULLUP_ONLY);
}

esp_err_t GPIO_Input::disablePullup() {
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_FLOATING);
}

esp_err_t GPIO_Input::enablePulldown() {
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_PULLDOWN_ONLY);
}

esp_err_t GPIO_Input::disablePulldown() {
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_FLOATING);
}

esp_err_t GPIO_Input::enablePullupPulldown() {
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_PULLUP_PULLDOWN);
}

esp_err_t GPIO_Input::disablePullupPulldown() {
    return gpio_set_pull_mode(m_interrupt_args.pin, GPIO_FLOATING);
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
	status = gpio_set_intr_type(m_interrupt_args.pin, intteruptType);
    }

    if (ESP_OK == status) {
	status = gpio_isr_handler_add(m_interrupt_args.pin, gpio_isr_callback, (void*) m_interrupt_args.pin);
    }

    return status;
}

esp_err_t GPIO_Input::setEventHandler(esp_event_handler_t eventHandler) {
    esp_err_t status{ESP_OK};

    taskENTER_CRITICAL(&m_eventChangeMutex);

    status = _clearEventHandlers();

    status = esp_event_handler_instance_register(INPUT_EVENTS, m_interrupt_args.pin, eventHandler, 0, nullptr);

    if (ESP_OK == status) {
	m_interrupt_args.event_handler_set = true;
    }

    taskEXIT_CRITICAL(&m_eventChangeMutex);

    return status;
}

esp_err_t GPIO_Input::setEventHandler(esp_event_loop_handle_t eventLoop, esp_event_handler_t eventHandler) {
    esp_err_t status{ESP_OK};

    taskENTER_CRITICAL(&m_eventChangeMutex);

    status = _clearEventHandlers();

    status |= esp_event_handler_instance_register_with(eventLoop, INPUT_EVENTS, m_interrupt_args.pin, eventHandler, 0, nullptr);

    if (ESP_OK == status) {
	m_event_handle = eventHandler;
	m_interrupt_args.custom_event_loop_handle = eventLoop;
	m_interrupt_args.custom_event_handler_set = true;
    }

    taskEXIT_CRITICAL(&m_eventChangeMutex);

    return status;
}

void IRAM_ATTR GPIO_Input::gpio_isr_callback(void* args) {
    int32_t pin = (reinterpret_cast<struct interrupt_args*>(args))->pin;
    bool custom_event_handler_set = (reinterpret_cast<struct interrupt_args*>(args))->custom_event_handler_set;
    bool event_handler_set = (reinterpret_cast<struct interrupt_args*>(args))->event_handler_set;
    bool queue_enabled = (reinterpret_cast<struct interrupt_args*>(args))->queue_enabled;
    esp_event_loop_handle_t custom_event_loop_handle = (reinterpret_cast<struct interrupt_args*>(args))->custom_event_loop_handle;
    QueueHandle_t queue_handle = (reinterpret_cast<struct interrupt_args*>(args))->queue_handle;

    if (queue_enabled) {
	xQueueSendFromISR(queue_handle, &pin, NULL);
    }
    else if (custom_event_handler_set) {
	esp_event_isr_post_to(custom_event_loop_handle, INPUT_EVENTS, pin, nullptr, 0, nullptr);
    }
    else if (event_handler_set) {
	esp_event_isr_post(INPUT_EVENTS, pin, nullptr, 0, nullptr);
    }
}

void GPIO_Input::setQueueHandle(QueueHandle_t eventQueue) {
    taskENTER_CRITICAL(&m_eventChangeMutex);
    _clearEventHandlers();
    m_interrupt_args.queue_handle = eventQueue;
    m_interrupt_args.queue_enabled = true;
    taskEXIT_CRITICAL(&m_eventChangeMutex);
}

esp_err_t GPIO_Input::_clearEventHandlers() {
    esp_err_t status{ESP_OK};

    if (m_interrupt_args.custom_event_handler_set) {
	esp_event_handler_unregister_with(m_interrupt_args.custom_event_loop_handle, INPUT_EVENTS, m_interrupt_args.pin,  m_event_handle);
	m_interrupt_args.custom_event_handler_set = false;
    }
    else if (m_event_handler_set) {
	esp_event_handler_instance_unregister(INPUT_EVENTS, m_interrupt_args.pin, nullptr);
	m_event_handler_set = false;
    }

    m_interrupt_args.queue_handle = nullptr;
    m_interrupt_args.queue_enabled = false;

    return status;
}

}
