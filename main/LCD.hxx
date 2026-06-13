#pragma once

#include <Hal.hxx>
#include <Types.hxx>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

namespace sillbird::lcd {

template <typename I2cBusType, std::byte I2cAddress> class Lcd {
public:
  Lcd() = delete;

  [[nodiscard]] static std::expected<void, HardwareError> Init() {
    using namespace i2c;
    sillbird::hal::DelayMs(50); // give lcd time to boot

    SendCommand(std::byte{0x33});
    SendCommand(std::byte{0x32});
    SendCommand(std::byte{0x28});
    SendCommand(std::byte{0x0C});
    Clear();

    return {};
  }

  static void Print(std::string_view text) {
    for (const char c : text) {
      SendData(std::byte{c});
    }
  }

  static void Clear() {
    SendCommand(std::byte{0x01});
    hal::DelayMs(2);
  }

private:
  static void WriteByte(std::byte val, std::byte mode) {
    auto high = val & std::byte{0xF0};
    auto low = (val << 4) & std::byte{0xF0};

    const auto b3 = std::byte{0x08};
    const auto b4 = std::byte{0x04};
    std::vector<std::byte> buf = {high | mode | b3 | b4, high | mode | b3 & ~b4,
                                  low | mode | b3 | b4, low | mode | b3 & ~b4};

    I2cBusType::Write(I2cAddress, buf);
  }

  static void SendCommand(std::byte cmd) { WriteByte(cmd, std::byte{0}); }
  static void SendData(std::byte data) { WriteByte(data, std::byte{1}); }
};

} // namespace sillbird::lcd
