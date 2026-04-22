#include <GPIO_Output.hxx>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

namespace sillbird {

GPIO_Output::GPIO_Output(const gpio_num_t pin, const bool activeLow)
    : mPin(pin), mActiveLow(activeLow) {
  esp_err_t status{ESP_OK};

  status |= gpio_reset_pin(mPin);
  status |= gpio_set_direction(mPin, GPIO_MODE_OUTPUT);

  ESP_LOGI(__func__, "Init GPIO_Output %s",
           (status == ESP_OK) ? "Success" : "Failed");
}

bool GPIO_Output::SetLevel(Level level) {
  return gpio_set_level(mPin, static_cast<uint32_t>(level)) == ESP_OK;
}

bool GPIO_Output::Activate(bool activate) {
  return SetLevel((mActiveLow ? Level::LOW : Level::HIGH));
}

} // namespace sillbird
