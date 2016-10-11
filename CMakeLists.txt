# This file is a part of assembler

# assembler is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# assembler is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with assembler. If not, see <http://www.gnu.org/licenses/>.

# AUTHOR: Suhas Sumukh, 2016


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

# adding rule for installing man page	
file(GLOB MANPAGE ${CMAKE_SOURCE_DIR}/doc/man/*)
install(FILES ${MANPAGE} DESTINATION /usr/share/man/man1)
