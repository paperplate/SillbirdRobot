#pragma once

#include <Gpio.hxx>
#include <Hal.hxx>
#include <Types.hxx>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <sys/types.h>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace sillbird {

template <typename... Pins>
  requires(gpio::OutputCapable<Pins::Config> && ...)
class StepperMotor {
public:
  using PinTuple = std::tuple<Pins...>;

  static_assert(sizeof...(Pins) == 4, "Stepper Motor requires 4 output pins");

  [[nodiscard]] static std::expected<StepperMotor, gpio::GpioError> Init() {
    bool success = (... && Pins::Init().has_value());
    if (!success) {
      return std::unexpected(gpio::GpioError::InitFailed);
    }
    using namespace gpio;
    SetLevels({false, false, false, false});
    return StepperMotor{};
  }

  void Move(stepperMotor::MoveTypes moveType, int steps, uint32_t delayMs) {
    int dir = steps > 0 ? 1 : -1;
    int absSteps = std::abs(steps);

    const auto mSeq = mMoveTypes.at(moveType);

    for (int s = 0; s < absSteps; s++) {
      if (dir > 0) {
        mCurrentStep = (mCurrentStep + 1) % 8;
      } else {
        mCurrentStep = (mCurrentStep + 7) % 8;
      }

      SetLevels(mSeq[mCurrentStep]);
      hal::DelayMs(delayMs);
    }
    Stop();
  }

  void Stop() {
    using namespace gpio;
    SetLevels({false, false, false, false});
  }

private:
  uint8_t mCurrentStep = 0;

  const std::unordered_map<stepperMotor::MoveTypes,
                           std::array<std::array<bool, 4>, 8>>
      mMoveTypes{
          {stepperMotor::MoveTypes::FULL,
           {{{1, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 1},
             {1, 0, 0, 1},
             {1, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 1},
             {1, 0, 0, 1}}}},
          {stepperMotor::MoveTypes::HALF,
           {{{1, 0, 0, 0},
             {1, 1, 0, 0},
             {0, 1, 0, 0},
             {0, 1, 1, 0},
             {0, 0, 1, 0},
             {0, 0, 1, 1},
             {0, 0, 0, 1},
             {1, 0, 0, 1}}}},
          {stepperMotor::MoveTypes::WAVE,
           {{{1, 0, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 1},
             {1, 0, 0, 0},
             {0, 1, 0, 0},
             {0, 0, 1, 0},
             {0, 0, 0, 1}}}},
      };

  StepperMotor() = default;

  static void SetLevels(const std::array<bool, 4> &levels) {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      (std::tuple_element_t<Is, PinTuple>::SetLevel(
           static_cast<gpio::Level>(levels[Is])),
       ...);
    }(std::make_index_sequence<4>{});
  }
};
} // namespace sillbird
