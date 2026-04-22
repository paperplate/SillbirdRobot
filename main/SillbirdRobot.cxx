#include <Gpio.hxx>
// #include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <hal/gpio_types.h>
// #include <inttypes.h>
#include <memory>
// #include <sdkconfig.h>
// #include <soc/gpio_num.h>

// #include <GPIO_Input.hxx>
// #include <GPIO_Output.hxx>
// #include <Level.hxx>
// #include <StepperMotor.hxx>

// std::unique_ptr<sillbird::GPIO_Input> gButton;
// std::unique_ptr<sillbird::GPIO_Output> gLed;
std::unique_ptr<sillbird::Gpio> gButton;
std::unique_ptr<sillbird::Gpio> gLed;

void setup() {
  using namespace sillbird;
  gButton = std::make_unique<Gpio>(13);
  bool res =
      gButton->Init(Gpio::Mode::Input, Gpio::Pull::Up, Gpio::IntrType::Rising);
  ESP_LOGI(__func__, "Init GPIO button %s", res ? "Success" : "Failed");
  gLed = std::make_unique<Gpio>(2);
  res = gLed->Init(Gpio::Mode::Output);
  ESP_LOGI(__func__, "Init GPIO led %s", res ? "Success" : "Failed");
  // gButton = std::make_unique<sillbird::GPIO_Input>(GPIO_NUM_13, true);
  // gLed = std::make_unique<sillbird::GPIO_Output>(GPIO_NUM_2, false);
}

void run() {
  vTaskDelay(pdMS_TO_TICKS(200));
  if (gButton->GetLevel() == sillbird::Gpio::Level::Low) {
    gLed->SetLevel(sillbird::Gpio::Level::High);
  } else {
    gLed->SetLevel(sillbird::Gpio::Level::Low);
  }
  /*if (gButton->GetLevel() == sillbird::Level::LOW) {
    gLed->SetLevel(sillbird::Level::HIGH);
  } else {
    gLed->SetLevel(sillbird::Level::LOW);
  }*/
}

extern "C" void app_main() {
  setup();
  while (true) {
    run();
  }
}
