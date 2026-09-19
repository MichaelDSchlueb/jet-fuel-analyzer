cmake_minimum_required(VERSION 3.14)
project(JetFuelAnalyzer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Dependency: JSON (Always needed for parsing) ---
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# --- Dependency: CPR (Linux / Production Only) ---
if(UNIX AND NOT APPLE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG        1.10.x
    )
    FetchContent_MakeAvailable(cpr)
endif()

# --- Target Configuration ---
include_directories(src)

if(UNIX AND NOT APPLE)
    set(AWS_IOT_SDK_ROOT "/opt/aws-iot-sdk")
    include_directories(${AWS_IOT_SDK_ROOT}/include)
endif()

add_executable(jet_fuel_analyzer_node
    main.cpp
    Aircraft.cpp
    EngineInstruments.cpp
    WeatherClient.cpp
)

# Common Libraries
target_link_libraries(jet_fuel_analyzer_node PRIVATE nlohmann_json::nlohmann_json)

# Windows Native Libraries (No downloads or cURL required)
if(WIN32)
    target_link_libraries(jet_fuel_analyzer_node PRIVATE wininet ws2_32)
endif()

# Linux AWS & CPR Linking
if(UNIX AND NOT APPLE)
    target_link_libraries(jet_fuel_analyzer_node
        PRIVATE
        cpr::cpr
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-crt-cpp.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-mqtt.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-s3.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-auth.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-http.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-io.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-cal.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-compression.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-sdkutils.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-checksums.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-event-stream.a
        ${AWS_IOT_SDK_ROOT}/lib64/libaws-c-common.a
        ${AWS_IOT_SDK_ROOT}/lib64/libs2n.a
        ${AWS_IOT_SDK_ROOT}/lib64/libcrypto.a
        ssl
        crypto
        pthread
        dl
        rt
    )
endif()