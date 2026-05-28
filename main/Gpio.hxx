#pragma once

#include <Hal.hxx>
#include <Types.hxx>

#include <expected>

namespace sillbird::gpio {

template <GpioConfig config>
concept OutputCapable =
    config.mMode == Mode::InputOutput || config.mMode == Mode::Output;

template <GpioConfig config>
concept InputCapable =
    config.mMode == Mode::InputOutput || config.mMode == Mode::Input;

template <GpioConfig config> class Gpio {
public:
  Gpio() = delete;

  [[nodiscard]] static std::expected<void, GpioError> Init() {
    if (!hal::Init(config)) {
      return std::unexpected(GpioError::InitFailed);
    }
    return {};
  }

  static constexpr GpioConfig Config = config;

  static void SetLevel(Level level)
    requires OutputCapable<config>
  {
    hal::SetLevel(config.mPin, level);
  }

  [[nodiscard]] static Level GetLevel()
    requires InputCapable<config>
  {
    return hal::GetLevel(config.mPin);
  }
};

} // namespace sillbird::gpio
