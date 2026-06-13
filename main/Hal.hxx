#pragma once

#include <Types.hxx>
#include <cstddef>
#include <cstdint>
#include <vector>

// Hardware Abstraction Layer (HAL) forward declarations
namespace sillbird {

namespace hal {
void DelayMs(uint32_t ms);
} // namespace hal

namespace gpio::hal {
bool Init(const GpioConfig &config);
void SetLevel(uint8_t pin, Level level);
Level GetLevel(uint8_t pin);
} // namespace gpio::hal

namespace i2c::hal {
bool Init(const I2cConfig &config);
bool Write(uint8_t port, std::byte addr, const std::vector<std::byte> &data);
} // namespace i2c::hal

} // namespace sillbird
