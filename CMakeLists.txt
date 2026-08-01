cmake_minimum_required(VERSION 3.12)
project(JetFuelAnalyzer CXX)

# Require modern C++ for AWS SDK compatibility
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Point to where we compiled and installed the AWS SDK
set(AWS_IOT_SDK_ROOT "/opt/aws-iot-sdk")

# 2. Expose the SDK header files and your project's header directory
include_directories(
    ${AWS_IOT_SDK_ROOT}/include
    src
)

# 3. Define your analyzer executable target and ALL source files
# (Crucial: Added your Aircraft and Engine implementations here!)
add_executable(jet_fuel_analyzer_node
    main.cpp
    Aircraft.cpp
    EngineInstruments.cpp
)

# 4. Bind everything together securely with a strict static link order
# Dependencies resolve from top to bottom (Common and s2n at the bottom)
target_link_libraries(jet_fuel_analyzer_node
    PRIVATE
    /opt/aws-iot-sdk/lib64/libaws-crt-cpp.a
    /opt/aws-iot-sdk/lib64/libaws-c-mqtt.a
    /opt/aws-iot-sdk/lib64/libaws-c-s3.a
    /opt/aws-iot-sdk/lib64/libaws-c-auth.a
    /opt/aws-iot-sdk/lib64/libaws-c-http.a
    /opt/aws-iot-sdk/lib64/libaws-c-io.a
    /opt/aws-iot-sdk/lib64/libaws-c-cal.a
    /opt/aws-iot-sdk/lib64/libaws-c-compression.a
    /opt/aws-iot-sdk/lib64/libaws-c-sdkutils.a
    /opt/aws-iot-sdk/lib64/libaws-checksums.a
    /opt/aws-iot-sdk/lib64/libaws-c-event-stream.a
    /opt/aws-iot-sdk/lib64/libaws-c-common.a
    /opt/aws-iot-sdk/lib64/libs2n.a
    /opt/aws-iot-sdk/lib64/libcrypto.a
    ssl
    crypto
    pthread
    dl
    rt
)
