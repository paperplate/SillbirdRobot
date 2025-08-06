set(SOURCES 
	"main.cxx"
	"SillbirdRobot/GPIO_Input.cxx"
	"SillbirdRobot/GPIO_Output.cxx"
)

idf_component_register(SRCS ${SOURCES}
	REQUIRES driver esp_event
	INCLUDE_DIRS "." 
	"../include/SillbirdRobot"
)
