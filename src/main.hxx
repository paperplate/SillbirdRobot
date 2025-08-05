#pragma once

#include "sillbirdGPIO.hxx"

class Main final {
public:
    void run();

    sillbird::GPIO_Output led { GPIO_NUM_2 };
    sillbird::GPIO_Input button { GPIO_NUM_13 };
};
