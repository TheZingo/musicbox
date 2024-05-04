cmake_minimum_required(VERSION 3.16)

project(Musicbox VERSION 0.0.1
        DESCRIPTION "RFID-based music player using bluetooth speakers"
        HOMEPAGE_URL "https://ptmm.de/about/musicbox")

set(PROJECT_LONG_DESCRIPTION "Musicbox providing an easy to use interface of putting RFID-enriched figures \
on the box to select, start and stop music, audio books, podcasts or sound effects.")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")


add_executable(MusicBox 
        RFIDHandler.cpp 
        LEDHandler.cpp
        BluetoothHandler.cpp 
        WebHandler.cpp 
        ContentHandler.cpp 
        MusicHandler.cpp
        TagHandler.cpp
        Speaker.cpp
        State.cpp
        main.cpp)
        
# https://github.com/luvbum/RPi-RFID.git
target_sources(MusicBox PRIVATE thirdparty/MFRC522.cpp)
TARGET_LINK_LIBRARIES(MusicBox PRIVATE ${CMAKE_THREAD_LIBS_INIT})

include(FetchContent)

# card reader interface: BCM2835
add_definitions(-DBCM2835_HAVE_LIBCAP)
find_library(BCM2835_LIBRARY bcm2835)
if(NOT BCM2835_LIBRARY)
   message(FATAL_ERROR "bcm2835 library not found")
endif()
target_link_libraries(MusicBox PRIVATE ${BCM2835_LIBRARY})

find_library(CAP_LIBRARY cap)
if(NOT CAP_LIBRARY)
 message(FATAL_ERROR "cap library not found")
endif()
target_link_libraries(MusicBox PRIVATE ${CAP_LIBRARY})

# Audio Output: pulse audio
find_library(PULSE_LIBRARY pulse)
if(NOT PULSE_LIBRARY)
   message(FATAL_ERROR "pulse library not found")
endif()
include_directories(/usr/include/pulse)
target_link_libraries(MusicBox PRIVATE ${PULSE_LIBRARY})

find_library(PULSE_SIMPLE_LIBRARY pulse-simple)

if(NOT PULSE_SIMPLE_LIBRARY)
 message(FATAL_ERROR "pulse-simple library not found")
endif()

target_link_libraries(MusicBox PRIVATE ${PULSE_SIMPLE_LIBRARY})

# MP3 Decoding: mpg123
find_library(MPG123_LIBRARY mpg123)
if(NOT MPG123_LIBRARY)
   message(FATAL_ERROR "libmpg123 not found")
endif()
target_link_libraries(MusicBox PRIVATE ${MPG123_LIBRARY})
include_directories(/usr/include/mpg123)

# sdbus-c++ (bluetooth handling)
find_package(sdbus-c++ REQUIRED)
target_link_libraries(MusicBox PRIVATE SDBusCpp::sdbus-c++)

## LED ring: rpi_ws281x
message(STATUS "Fetching rpi_ws281x Source...")
FetchContent_Declare(rpi_ws281x
        GIT_REPOSITORY https://github.com/jgarff/rpi_ws281x
        GIT_TAG v1.0.0
        )
FetchContent_GetProperties(rpi_ws281x)
if(NOT rpi_ws281x_POPULATED)
  FetchContent_Populate(rpi_ws281x)
  add_subdirectory(${rpi_ws281x_SOURCE_DIR} ${rpi_ws281x_BINARY_DIR})
  # the source dir ${rpi_ws281x_SOURCE_DIR} contains a "version" file
  # which conflics with boost asio source /usr/include/boost/config/detail/select_stdlib_config.hpp
  # so creating a temp directory and copy all headers to prevent the conflict
  set(TEMP_HEADER_DIR "${CMAKE_BINARY_DIR}/rpi_ws281x_temp_header")
  file(MAKE_DIRECTORY ${TEMP_HEADER_DIR})
  file(GLOB RPI_WS281X_HEADERS "${rpi_ws281x_SOURCE_DIR}/*.h")
  file(COPY ${RPI_WS281X_HEADERS} DESTINATION ${TEMP_HEADER_DIR})
  target_include_directories(MusicBox PRIVATE ${TEMP_HEADER_DIR})
  target_link_libraries(MusicBox PRIVATE ws2811)
endif(NOT rpi_ws281x_POPULATED)

# web interface: Crow
set(CROW_BUILD_EXAMPLES Off)
set(CROW_BUILD_TOOLS Off)
set(CROW_BUILD_TESTS Off)
set(CROW_BUILD_DOCS Off)

message(STATUS "Fetching crow Source...")
FetchContent_Declare(crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.0+5
        )
FetchContent_GetProperties(crow)
if(NOT crow_POPULATED)
  FetchContent_Populate(crow)
  #add_library(Crow INTERFACE)
  target_include_directories(MusicBox PRIVATE
     ${crow_SOURCE_DIR}/include
  )
  #target_link_libraries(Crow INTERFACE asio)
endif(NOT crow_POPULATED)


# JSON Handling: nlohmann/json
message(STATUS "Fetching JSON Lib (nlohmann) Source...")
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(MusicBox PRIVATE nlohmann_json::nlohmann_json)


