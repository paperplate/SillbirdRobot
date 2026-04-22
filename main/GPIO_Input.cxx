#include <GPIO_Input.hxx>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sys/stat.h>

namespace sillbird {

GPIO_Input::GPIO_Input(const gpio_num_t pin, const bool activeLow)
    : mPin(pin), mActiveLow(activeLow) {
  esp_err_t status{ESP_OK};

  status |= gpio_reset_pin(mPin);
  status |= gpio_set_direction(mPin, GPIO_MODE_INPUT);

  // enable pullup for input pin
  status |= gpio_pullup_en(mPin);

  // disable pulldown for input pin
  status |= gpio_pulldown_dis(mPin);

  // configure raising edge detection interrupt for input pin
  status |= gpio_set_intr_type(mPin, GPIO_INTR_POSEDGE);

  // install gpio isr service to default values
  status |= gpio_install_isr_service(0);

  ESP_LOGI(__func__, "Init GPIO_Input %s",
           (status == ESP_OK) ? "Success" : "Failed");
}

Level GPIO_Input::GetLevel() {
  return static_cast<Level>(gpio_get_level(mPin));
}

bool GPIO_Input::IsActive() { return mActiveLow == (GetLevel() == Level::LOW); }

} // namespace sillbird
