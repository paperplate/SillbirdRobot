#include <Gpio.hxx>
#include <StepperMotor.hxx>
#include <cstdlib>
#include <esp_log.h>
#include <expected>
#include <freertos/FreeRTOS.h>

// config block
namespace sillbird {
using namespace gpio;
// LED config
constexpr GpioConfig ledConfig{.mPin = 2, .mMode = Mode::Output};
// Button config
constexpr gpio::GpioConfig buttonConfig{
    .mPin = 13, .mMode = Mode::Input, .mPull = Pull::Up};

// Motor config
constexpr GpioConfig mototPin1{.mPin = 25, .mMode = Mode::Output};
constexpr GpioConfig mototPin2{.mPin = 26, .mMode = Mode::Output};
constexpr GpioConfig mototPin3{.mPin = 27, .mMode = Mode::Output};
constexpr GpioConfig mototPin4{.mPin = 14, .mMode = Mode::Output};
} // namespace sillbird

namespace sg = sillbird::gpio;
using gLed = sg::Gpio<sillbird::ledConfig>;
using gButton = sg::Gpio<sillbird::buttonConfig>;
using Motor = sillbird::StepperMotor<
    sg::Gpio<sillbird::mototPin1>, sg::Gpio<sillbird::mototPin2>,
    sg::Gpio<sillbird::mototPin3>, sg::Gpio<sillbird::mototPin4>>;

class Application final {
public:
  Application(Motor motor) : mMotor(motor) {}
  std::expected<void, sillbird::gpio::GpioError> setup() {
    using namespace sillbird::gpio;
    if (!gLed::Init()) {
      ESP_LOGI(__func__, "Init GPIO led: Failed");
      return std::unexpected(GpioError::InitFailed);
    }
    ESP_LOGI(__func__, "Init GPIO led: Success");
    if (!gButton::Init()) {
      ESP_LOGI(__func__, "Init GPIO button: Failed");
      return std::unexpected(GpioError::InitFailed);
    }
    ESP_LOGI(__func__, "Init GPIO button: Success");
    return {};
  }

  void run() {
    using namespace sillbird::gpio;
    while (true) {
      if (gButton::GetLevel() == Level::Low) {
        gLed::SetLevel(Level::High);
        mMotor.Move(sillbird::stepperMotor::MoveTypes::FULL, 200, 10);
      } else {
        gLed::SetLevel(Level::Low);
      }
      vTaskDelay(pdMS_TO_TICKS(200));
    }
  }

private:
  Motor mMotor;
};

extern "C" void app_main() {
  auto motor = Motor::Init();
  if (!motor) {
    exit(EXIT_FAILURE);
  }

  Application app(motor.value());
  if (!app.setup()) {
    exit(EXIT_FAILURE);
  }
  app.run();
}
