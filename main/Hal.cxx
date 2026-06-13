#include "esp_err.h"
#include "soc/clk_tree_defs.h"
#include <Hal.hxx>
#include <Types.hxx>

#include <array>
#include <cstddef>
#include <cstdint>
#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <esp_attr.h>
#include <esp_lcd_io_i2c.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>
#include <hal/gpio_types.h>
#include <vector>

namespace sillbird {

namespace hal {
void DelayMs(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }
} // namespace hal

namespace gpio::hal {
static gpio_mode_t MapMode(Mode mode) {
  switch (mode) {
  case Mode::Input:
    return GPIO_MODE_INPUT;
  case Mode::Output:
    return GPIO_MODE_OUTPUT;
  case Mode::InputOutput:
    return GPIO_MODE_INPUT_OUTPUT;
  default:
    return GPIO_MODE_DISABLE;
  }
}

bool Init(const GpioConfig &config) {
  gpio_config_t ioConf = {
      .pin_bit_mask = (1ULL << config.mPin),
      .mode = MapMode(config.mMode),
      .pull_up_en =
          (config.mPull == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
      .pull_down_en = (config.mPull == Pull::Down) ? GPIO_PULLDOWN_ENABLE
                                                   : GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  gpio_reset_pin(static_cast<gpio_num_t>(config.mPin));
  return (gpio_config(&ioConf) == ESP_OK);
}

void SetLevel(uint8_t pin, Level level) {
  gpio_set_level(static_cast<gpio_num_t>(pin), static_cast<int>(level));
}

Level GetLevel(uint8_t pin) {
  return static_cast<Level>(gpio_get_level(static_cast<gpio_num_t>(pin)));
}
} // namespace gpio::hal

namespace i2c::hal {
// internal state
// esp32 has 2 i2c ports
struct BusConfig {
  i2c_master_bus_handle_t handle;
  uint32_t speedHz;
};
static std::array<BusConfig, 2> gBusHandles = {BusConfig{nullptr, 0},
                                               BusConfig{nullptr, 0}};

struct RegisteredDevice {
  uint8_t port;
  std::byte address;
  i2c_master_dev_handle_t handle;
};

static std::vector<RegisteredDevice> gActiveDevices;

bool Init(const I2cConfig &config) {
  if (config.port >= 2) {
    return false;
  }

  i2c_master_bus_config_t conf = {
      .i2c_port = config.port,
      .sda_io_num = static_cast<gpio_num_t>(config.sdaPin),
      .scl_io_num = static_cast<gpio_num_t>(config.sclPin),
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags = {}};
  conf.flags.enable_internal_pullup = true;

  esp_err_t err = i2c_new_master_bus(&conf, &gBusHandles[config.port].handle);
  if (err == ESP_OK) {
    gBusHandles[config.port].speedHz = config.freqHz;
    return true;
  }
  return false;
}

static i2c_master_dev_handle_t GetOrRegisterDevice(uint8_t port,
                                                   std::byte address) {
  for (const auto &active : gActiveDevices) {
    if ((active.port == port) && (active.address == address)) {
      return active.handle;
    }
  }

  if (port >= gBusHandles.size()) {
    return nullptr;
  }

  i2c_device_config_t deviceConfig = {.dev_addr_length = I2C_ADDR_BIT_LEN_7,
                                      .device_address =
                                          std::to_integer<uint16_t>(address),
                                      .scl_speed_hz = gBusHandles[port].speedHz,
                                      .scl_wait_us = 20000,
                                      .flags = {}};

  i2c_master_dev_handle_t newHandle;
  esp_err_t err = i2c_master_bus_add_device(gBusHandles[port].handle,
                                            &deviceConfig, &newHandle);

  if (err == ESP_OK) {
    gActiveDevices.push_back({port, address, newHandle});
    return newHandle;
  }

  return nullptr;
}

bool Write(uint8_t port, std::byte addr, const std::vector<std::byte> &data) {
  i2c_master_dev_handle_t deviceHandle = GetOrRegisterDevice(port, addr);
  if (deviceHandle == nullptr) {
    return false;
  }

  return i2c_master_transmit(
             deviceHandle,
             reinterpret_cast<uint8_t *>(const_cast<std::byte *>(data.data())),
             data.size(), 100) == ESP_OK;
}
} // namespace i2c::hal
} // namespace sillbird
