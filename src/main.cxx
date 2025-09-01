#include "main.hxx"
#include "esp_event.h"
#include "freertos/idf_additions.h"
#include "portmacro.h"
#include <cstdint>

QueueHandle_t Main::button_event_queue{nullptr};
static bool alternate = true;
void Main::run() {
    for (auto &led : leds) {
	vTaskDelay(pdMS_TO_TICKS(200));
	led.setLevel(button.read());
    }
    float angle = alternate ? 180.0f : 0.0f;
    alternate = !alternate;
    servo.setAngle(angle);
}

void Main::setup() {
    button_event_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    esp_event_loop_args_t gpio_loop_args;
    gpio_loop_args.queue_size = 5;
    gpio_loop_args.task_name = "loop_task";
    gpio_loop_args.task_priority = uxTaskPriorityGet(NULL);
    gpio_loop_args.task_stack_size = 2048;
    gpio_loop_args.task_core_id = tskNO_AFFINITY;

    esp_event_loop_create(&gpio_loop_args, &gpio_loop_handle);
    esp_event_loop_create_default();

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

void Main::gpio_task_example(void *arg) {
    uint32_t io_num;
    for (;;) {
	if (xQueueReceive(Main::button_event_queue, &io_num, portMAX_DELAY)) {
	    std::cout << "Interrupt triggered from pin " << (int)io_num << " and send to queue\n";
	}
    }
}

extern "C" void app_main() {
    esp_event_loop_create_default();

    Main app;

    app.setup();

    while (true) {
	app.run();
    }
}
