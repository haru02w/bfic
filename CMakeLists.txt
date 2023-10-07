cmake_minimum_required(VERSION 3.5)
project(bfic C)

# Set the C standard required
set(CMAKE_C_STANDARD 11)
# set clang-format flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set cmake debug target
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS 
	"${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wuninitialized \
	-Wmissing-prototypes")

# Library
file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(bfic ${SOURCES})
