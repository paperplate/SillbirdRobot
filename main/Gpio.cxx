#include "hal/gpio_types.h"
#include <Gpio.hxx>
#include <driver/gpio.h>
#include <esp_attr.h>

namespace sillbird {

Gpio::Gpio(const int pin) : mPin(pin), mIsrCallback(nullptr) {}

Gpio::~Gpio() {
  gpio_reset_pin(static_cast<gpio_num_t>(mPin));
  gpio_isr_handler_remove(static_cast<gpio_num_t>(mPin));
}

bool Gpio::Init(Mode mode, Pull pull, IntrType intrType) {
  gpio_config_t ioConf = {};
  ioConf.pin_bit_mask = (1ULL << mPin);

  switch (mode) {
  case Mode::Input: {
    ioConf.mode = GPIO_MODE_INPUT;
    break;
  }
  case Mode::Output: {
    ioConf.mode = GPIO_MODE_INPUT;
    break;
  }
  default: {
    ioConf.mode = GPIO_MODE_INPUT_OUTPUT;
    break;
  }
  }

  ioConf.pull_up_en =
      (pull == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
  ioConf.pull_down_en =
      (pull == Pull::Down) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;

  switch (intrType) {
  case IntrType::Disable: {
    ioConf.intr_type = GPIO_INTR_DISABLE;
    break;
  }
  case IntrType::Falling: {
    ioConf.intr_type = GPIO_INTR_NEGEDGE;
    break;
  }
  case IntrType::Rising: {
    ioConf.intr_type = GPIO_INTR_POSEDGE;
    break;
  }
  default: {
    ioConf.intr_type = GPIO_INTR_ANYEDGE;
    break;
  }
  }

  gpio_reset_pin(static_cast<gpio_num_t>(mPin));
  return (gpio_config(&ioConf) == ESP_OK);
}

void Gpio::SetLevel(Level level) {
  gpio_set_level(static_cast<gpio_num_t>(mPin), static_cast<int>(level));
}

Gpio::Level Gpio::GetLevel() const {
  return static_cast<Level>(gpio_get_level(static_cast<gpio_num_t>(mPin)));
}

void IRAM_ATTR Gpio::IsrRouter(void *arg) {
  Gpio *instance = static_cast<Gpio *>(arg);
  if (instance && instance->mIsrCallback) {
    instance->mIsrCallback();
  }
}

void Gpio::OnInterrupt(std::function<void()> callback) {
  mIsrCallback = callback;
  gpio_install_isr_service(0);
  gpio_isr_handler_add(static_cast<gpio_num_t>(mPin), IsrRouter, this);
}

} // namespace sillbird
