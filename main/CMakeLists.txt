set(SOURCES
  "SillbirdRobot.cxx"
  "Gpio.hxx"
  "StepperMotor.hxx"
  "Hal.cxx"
  "Hal.hxx"
  "Types.hxx"
  "I2cBus.hxx"
  "LCD.hxx"
)

idf_component_register(
  SRCS ${SOURCES}
	REQUIRES esp_driver_gpio esp_driver_i2c esp_event esp_lcd
	INCLUDE_DIRS "."
)

