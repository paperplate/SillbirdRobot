set(SOURCES
  "SillbirdRobot.cxx"
  #"GPIO_Input.cxx"
  #"GPIO_Output.cxx"
  "Gpio.cxx"
)

idf_component_register(
  SRCS ${SOURCES}
	REQUIRES esp_driver_rmt esp_driver_gpio esp_event pthread
	INCLUDE_DIRS "."
)

