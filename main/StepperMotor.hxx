#pragma once

#include "soc/gpio_num.h"
#include <driver/rmt_encoder.h>

#include <GPIO_Input.hxx>
#include <GPIO_Output.hxx>

#include <memory>
#include <stdint.h>

namespace sillbird {
class StepperMotor {
public:
  StepperMotor(const gpio_num_t dirPin);
  struct CurveEncoderConfig {
    uint32_t resolution;   // Encoder resolution Hz
    uint32_t samplePoints; // Sample points used for deceleration phase. |
                           // endFreq - startFreq | >= samplePoints
    uint32_t startFreq;    // Start freq on the curve Hz
    uint32_t endFreq;      // End freq on the curve Hz
  };

  struct UniformEncoderConfig {
    uint32_t resolution; // Encoder resolution in Hz
  };

  bool NewCurveEncoder(const CurveEncoderConfig &config);
  bool NewUniformEncoder(const UniformEncoderConfig &config);

private:
  std::unique_ptr<GPIO_Output> mDirPin;
};
} // namespace sillbird
