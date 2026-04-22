#pragma once

#include <Level.hxx>
#include <soc/gpio_num.h>

namespace sillbird {

class GPIO_Output {
public:
  GPIO_Output(const gpio_num_t pin, const bool activeLow);
  bool SetLevel(Level level);
  bool Activate(bool activate);

private:
  const gpio_num_t mPin;
  const bool mActiveLow;
};

} // namespace sillbird
