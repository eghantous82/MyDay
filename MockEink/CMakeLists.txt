# Set the minimum required version of CMake.
# It's good practice to specify the version you're building against.
cmake_minimum_required(VERSION 3.10)

# Define the project name and the languages used.
project(EpaperSim LANGUAGES CXX)

# --- Find and configure all required libraries ---

# Find OpenCV. vcpkg provides the configuration file for this.
# This assumes OpenCV is installed via vcpkg.
find_package(OpenCV REQUIRED)

find_package(CURL REQUIRED)

# --- Build the executable ---

# Add your executable and specify the source files.
add_executable(epaper_sim main.cpp MockEpaperDisplay.cpp Adafruit_GFX.cpp)

# --- Link the executable to the libraries ---

# Link the executable with OpenCV libraries.
# The `OpenCV_LIBS` variable handles all necessary OpenCV dependencies.
target_link_libraries(epaper_sim PRIVATE
    ${OpenCV_LIBS} CURL::libcurl
)

# Manually add the include directories from the found packages.
# This is a necessary step when not using imported targets.
target_include_directories(epaper_sim PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)
