#include "main.hxx"

void Main::run() {
    vTaskDelay(pdMS_TO_TICKS(200));
    led.setLevel(button.read());
}

void Main::setup() {
    button.enableInterrupt(GPIO_INTR_NEGEDGE);
    button.setEventHandler(&button_event_handler);

    button2.enablePullup();
    button2.enableInterrupt(GPIO_INTR_NEGEDGE);
    button2.setEventHandler(&button2_event_handler);
}

void Main::button_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data) {
    std::cout << "button triggered interrupt id: " << id << "\n";
}

void Main::button2_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data) {
    std::cout << "button triggered interrupt id: " << id << "\n";
}

extern "C" void app_main() {
    esp_event_loop_create_default();

    Main app;

    app.setup();

    while (true) {
	app.run();
    }
}
