# CMake file for Vulkan Tutorial
# by Shelby Jueden (AkBKukU)

cmake_minimum_required(VERSION 3.1)

<<<<<<< HEAD
project(VKDemo)
=======
project(VkEngine)
>>>>>>> Project name fix

# Set the name of the ouput binary
set(
	BIN_NAME
<<<<<<< HEAD
	"vkdemo"
=======
	"VkEngine"
>>>>>>> Project name fix
)

# Add all files from a src folder
file(GLOB SOURCES "src/*.cpp")

# Add vulkan dependencies using enviornment variable from LunarG SDK Setup
include_directories("$ENV{VULKAN_SDK}/include")
link_directories("$ENV{VULKAN_SDK}/lib") 
link_directories("$ENV{VULKAN_SDK}/etc/explicit_layer.d")

# Add GLFW
find_package(glfw3 3.2 REQUIRED)

# Flags for debug build
set(
	CMAKE_CXX_FLAGS_DEBUG 
	"${CMAKE_CXX_FLAGS_DEBUG} -d -Wall"
)
# Flags for Release build
set(
	CMAKE_CXX_FLAGS_RELEASE 
	"${CMAKE_CXX_FLAGS_RELEASE} -Wall"
)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# Setup binary output
add_executable(${BIN_NAME} ${SOURCES})

# Link dependencies
target_link_libraries(${BIN_NAME} glfw)
target_link_libraries(${BIN_NAME} vulkan)