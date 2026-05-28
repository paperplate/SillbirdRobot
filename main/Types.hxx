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

} // namespace sillbird
