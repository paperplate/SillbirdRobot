#pragma once

#include <cstdint>

namespace sillbird {

namespace gpio {
enum class Mode { Input, Output, InputOutput };
enum class Pull { None, Up, Down };
enum class IntrType { Disable, Rising, Falling, AnyEdge };
enum class Level { High = 1, Low = 0 };
enum class GpioError { Ok, InitFailed, InvalidPin };

struct GpioConfig {
  uint8_t mPin;
  Mode mMode;
  Pull mPull = Pull::None;
};

} // namespace gpio

namespace stepperMotor {
enum class MoveTypes { WAVE, HALF, FULL };
}

enum class HardwareError { Ok, InitFailed, InvalidPin, BusError };

namespace i2c {
struct I2cConfig {
  uint8_t port;
  uint8_t sdaPin;
  uint8_t sclPin;
  uint32_t freqHz;
};

} // namespace i2c

} // namespace sillbird
