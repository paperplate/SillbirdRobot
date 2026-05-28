#include <Hal.hxx>

#include <driver/gpio.h>
#include <esp_attr.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>
#include <hal/gpio_types.h>

namespace sillbird {

namespace hal {
void DelayMs(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }
} // namespace hal

namespace gpio::hal {
static gpio_mode_t MapMode(Mode mode) {
  switch (mode) {
  case Mode::Input:
    return GPIO_MODE_INPUT;
  case Mode::Output:
    return GPIO_MODE_OUTPUT;
  case Mode::InputOutput:
    return GPIO_MODE_INPUT_OUTPUT;
  default:
    return GPIO_MODE_DISABLE;
  }
}

bool Init(const GpioConfig &config) {
  gpio_config_t ioConf = {
      .pin_bit_mask = (1ULL << config.mPin),
      .mode = MapMode(config.mMode),
      .pull_up_en =
          (config.mPull == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
      .pull_down_en = (config.mPull == Pull::Down) ? GPIO_PULLDOWN_ENABLE
                                                   : GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  gpio_reset_pin(static_cast<gpio_num_t>(config.mPin));
  return (gpio_config(&ioConf) == ESP_OK);
}

void SetLevel(uint8_t pin, Level level) {
  gpio_set_level(static_cast<gpio_num_t>(pin), static_cast<int>(level));
}

Level GetLevel(uint8_t pin) {
  return static_cast<Level>(gpio_get_level(static_cast<gpio_num_t>(pin)));
}
} // namespace gpio::hal
} // namespace sillbird
