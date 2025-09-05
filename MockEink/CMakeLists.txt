# Set the minimum required version of CMake.
# It's good practice to specify the version you're building against.

cmake_minimum_required(VERSION 3.10)
project(EpaperSim LANGUAGES CXX)


# --- Find and configure all required libraries ---
find_package(OpenCV REQUIRED)  # Requires OpenCV via vcpkg or system
find_package(CURL REQUIRED)


# --- Build the executable ---
add_executable(epaper_sim main.cpp)
target_sources(epaper_sim PRIVATE
    MockEpaperDisplay.cpp
    Adafruit_GFX.cpp
    MockDataRetriever.cpp
    ../lib/Api/src/MlbApi.cpp
    ../lib/Api/src/MarketApi.cpp
    ../lib/Api/src/GoogleScriptApi.cpp
    ../lib/Api/src/BlynkApi.cpp
    ../lib/Secrets/src/Secrets.cpp
    ../lib/Application/src/Application.cpp
)

# Compiler options (disable warning C4180 for MSVC)
target_compile_options(epaper_sim PRIVATE /wd4180)

# Link the executable with OpenCV and CURL
target_link_libraries(epaper_sim PRIVATE ${OpenCV_LIBS} CURL::libcurl)

# Add include directories
target_include_directories(epaper_sim PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ArduinoJson/src
)
