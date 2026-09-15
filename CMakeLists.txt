cmake_minimum_required(VERSION 3.14)
project(JetFuelAnalyzer CXX)

# Require C++17 for CPR and JSON libraries
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ------------------------------------------------------------------------------
# 1. Fetch JSON & HTTP Libraries
# ------------------------------------------------------------------------------
# nlohmann/json for parsing Open-Meteo payload
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# CPR (C++ Requests wrapper for cURL)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.x
)
FetchContent_MakeAvailable(cpr)

# ------------------------------------------------------------------------------
# 2. Platform-Specific Setup
# ------------------------------------------------------------------------------
if(UNIX AND NOT APPLE)
    set(AWS_IOT_SDK_ROOT "/opt/aws-iot-sdk")
    include_directories(${AWS_IOT_SDK_ROOT}/include)
endif()

include_directories(src)

# ------------------------------------------------------------------------------
# 3. Target Definition
# ------------------------------------------------------------------------------
add_executable(jet_fuel_analyzer_node
    main.cpp
    Aircraft.cpp
    EngineInstruments.cpp
)

# ------------------------------------------------------------------------------
# 4. Link Libraries
# ------------------------------------------------------------------------------
# Link HTTP and JSON modules cross-platform
target_link_libraries(jet_fuel_analyzer_node
    PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# Link AWS SDK and Linux system dependencies when building on EC2
if(UNIX AND NOT APPLE)
    target_link_libraries(jet_fuel_analyzer_node
        PRIVATE
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