#pragma once

#include <Hal.hxx>
#include <Types.hxx>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <vector>

namespace sillbird::i2c {

template <I2cConfig config>
concept ValidI2cPort = (config.port == 0 || config.port == 1);

template <I2cConfig config>
concept ValidI2cPins = (config.sclPin < 34) && (config.sdaPin < 34) &&
                       (config.sdaPin != config.sclPin);

template <I2cConfig config>
concept ValidI2cFreq = (config.freqHz >= 10000 && config.freqHz <= 1000000);

template <I2cConfig config>
concept ValidI2cConfig =
    ValidI2cPort<config> && ValidI2cFreq<config> && ValidI2cPins<config>;

template <I2cConfig config>
  requires ValidI2cConfig<config>
class I2cBus {
public:
  static constexpr I2cConfig mConfigVal = config;

  I2cBus() = delete;

  [[nodiscard]] static std::expected<void, HardwareError> Init() {
    if (!hal::Init(config)) {
      return std::unexpected(HardwareError::InitFailed);
    }
    return {};
  }

  static bool Write(std::byte address, const std::vector<std::byte> &data) {
    return hal::Write(config.port, address, data);
  }
};

} // namespace sillbird::i2c
