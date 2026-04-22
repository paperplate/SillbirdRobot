#pragma once

#include <Level.hxx>
#include <soc/gpio_num.h>

namespace sillbird {

class GPIO_Input {
public:
  GPIO_Input(const gpio_num_t pin, const bool activeLow);
  Level GetLevel();
  bool IsActive();

private:
  const gpio_num_t mPin;
  const bool mActiveLow;
};

} // namespace sillbird
