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

message(STATUS "Checking if install file exists...")
if(EXISTS ${CMAKE_BINARY_DIR}/install_manifest.txt)
	message(STATUS "Install file found.")
else()
	message(FATAL_ERROR "Install file not found. Uninstall failed.")
endif()

file(READ ${CMAKE_BINARY_DIR}/install_manifest.txt files)
string(REGEX REPLACE "\n" ";" files ${files})

foreach(file ${files})
	message(STATUS "Uninstalling ${file}...")
	if(IS_SYMLINK ${file} OR EXISTS ${file})
		execute_process(COMMAND ${CMAKE_COMMAND} -E remove -f ${file})
		if(IS_SYMLINK ${file} OR EXISTS ${file})
			message(STATUS "Could not uninstall ${file}.")
		else()
			message(STATUS "Successfully uninstalled ${file}")
		endif()
	else()
		message(STATUS "Cannot uninstall ${file}. File not found.")
	endif()
endforeach()

message(STATUS "Uninstall process complete.")
