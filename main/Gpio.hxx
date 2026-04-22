#pragma once

#include <functional>

namespace sillbird {

class Gpio {
public:
  enum class Mode { Input, Output, InputOutput };
  enum class Pull { None, Up, Down };
  enum class IntrType { Disable, Rising, Falling, AnyEdge };
  enum class Level { High = 1, Low = 0 };

  explicit Gpio(const int pin);
  ~Gpio();

  bool Init(Mode mode, Pull pull = Pull::None,
            IntrType intrType = IntrType::Disable);

  void SetLevel(Level level);
  Level GetLevel() const;

  void OnInterrupt(std::function<void()> callback);

private:
  const int mPin;
  std::function<void()> mIsrCallback;

  static void IsrRouter(void *arg);
};

} // namespace sillbird
