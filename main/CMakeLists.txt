set(SOURCES
  "SillbirdRobot.cxx"
  "Gpio.hxx"
  "StepperMotor.hxx"
  "Hal.cxx"
  "Hal.hxx"
  "Types.hxx"
)

idf_component_register(
  SRCS ${SOURCES}
	REQUIRES esp_driver_gpio esp_event pthread
	INCLUDE_DIRS "."
)

