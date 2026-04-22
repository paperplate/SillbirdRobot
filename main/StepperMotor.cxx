#include <StepperMotor.hxx>
#include <cstdint>
#include <soc/gpio_num.h>

namespace {
constexpr uint32_t gMotorResolution = 1000000; // 1MHz resolution
constexpr int gMotorEnableLevel = 0;           // enabled on low
constexpr int gMotorSpinDirClockwise = 0;
constexpr int gMotorSpinDirCounterClockwise = 1; // !gMotorSpinDirClockwise
static const char *TAG = "StepperMotor";
} // namespace

namespace sillbird {
StepperMotor::StepperMotor(const gpio_num_t dirPin) {
  mDirPin = std::make_unique<GPIO_Output>(dirPin, true);
}

bool StepperMotor::NewCurveEncoder(const CurveEncoderConfig &config) {
  return true;
}

bool StepperMotor::NewUniformEncoder(const UniformEncoderConfig &config) {
  return true;
}
} // namespace sillbird
