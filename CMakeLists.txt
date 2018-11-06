CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(cv_tutorial)
FIND_PACKAGE(OpenCV 	REQUIRED )
include_directories ("${OpenCV_INCLUDE_DIRS}")
#Required for uco thinstation. May not be required in your own pc
SET(CMAKE_CXX_FLAGS)
LINK_LIBRARIES(${OpenCV_LIBS} )
add_executable(calibracion camera_calibration.cpp)
add_executable(augReal augReal.cpp funcionesAuxiliares.cpp)

