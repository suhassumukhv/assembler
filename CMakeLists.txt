# check for version compatibility
cmake_minimum_required(VERSION 3.0)

# initialize project names
project(assembler)

# include the include files
include_directories(include)

# set the executable creation path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# adding rule for executable
file(GLOB SOURCES src/*)
add_executable(ac ${SOURCES})

# adding rule for installation
install(TARGETS ac DESTINATION /bin)

# target for uninstalling
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/cmake_uninstall.cmake 
							COMMENT "Uninstalling installed targets...")
							