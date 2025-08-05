set(SOURCES 
	"main.cxx"
	"SillbirdRobot/sillbirdGPIO.cxx"
)

idf_component_register(SRCS ${SOURCES}
	REQUIRES driver
	INCLUDE_DIRS "." 
	"../include/SillbirdRobot"
)
