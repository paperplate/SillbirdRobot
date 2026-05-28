#pragma once

#include <Types.hxx>
#include <cstdint>

// Hardware Abstraction Layer (HAL) forward declarations
namespace sillbird {

namespace hal {
void DelayMs(uint32_t ms);
}

namespace gpio::hal {
bool Init(const GpioConfig &config);
void SetLevel(uint8_t pin, Level level);
Level GetLevel(uint8_t pin);
} // namespace gpio::hal

} // namespace sillbird
