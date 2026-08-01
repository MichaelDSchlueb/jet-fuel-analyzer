//#include <aws/iot/MqttClient.h>
//#include <aws/crt/Api.h>
//#include <aws/crt/auth/Credentials.h>
#include <iostream>
//#include <httplib.h>
//#include <nlohmann/json.hpp>
#include <array>
#include <chrono>
#include <thread>
#include <cstdint>
#include <sstream>
#include <memory>
#include "Aircraft.h"

//usin g json = nlohmann::json

// === MULTI-CLOUD TOGGLE ===
#define TARGET_CLOUD_AWS
// #define TARGET_CLOUD_GCP

using UInt32 = std::uint32_t;
using Float32 = float;

// ========================================================
// 1. GLOBAL DATA STRUCTURE (Visible to both clouds)
// ========================================================
struct JetFuelSensorFrame {
    UInt32 timestamp;
    Float32 fuelFlowRate;
    Float32 fuelTemperature;
    UInt32 fuelPressure;
    UInt32 flightId; 
};

// ========================================================
// 2. THE SIMULATION COMPONENT CLASS
// ========================================================
/*class FuelTelemetryComponent {
private:
    static constexpr size_t MAX_BUFFER_SIZE = 100;
    std::array<JetFuelSensorFrame, MAX_BUFFER_SIZE> m_localSensorStorage;
    size_t m_bufferIndex = 0;
    Float32 m_currentTemperature = 15.0f;

#ifdef TARGET_CLOUD_AWS
    std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> m_mqttConnection;
#endif

public:
    // Constructor accepts the live network client link natively
#ifdef TARGET_CLOUD_AWS
    FuelTelemetryComponent(std::shared_ptr<Aws::Crt::Mqtt::MqttConnection> connection) 
        : m_mqttConnection(connection) {}
#else
    FuelTelemetryComponent() {}
#endif

    void Cyclical_Sensor_Read() {
        if (m_bufferIndex >= MAX_BUFFER_SIZE) return;

        m_currentTemperature += 0.05f; 
        
        JetFuelSensorFrame frame;
        frame.timestamp = static_cast<UInt32>(std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
        frame.fuelFlowRate = 42.5f;
        frame.fuelTemperature = m_currentTemperature;
        frame.fuelPressure = 310;
        frame.flightId = 1728; // Airframe linkage tracking key

        m_localSensorStorage[m_bufferIndex] = frame;
        m_bufferIndex++;

        std::cout << "[SENSOR] Frame stored locally: " << frame.fuelTemperature << "°C" << std::endl;
    }

    void Flush_To_Bronze_Layer() {
        if (m_bufferIndex == 0) return;

#ifdef TARGET_CLOUD_AWS
        std::cout << "[AWS MQTT] Marshalling " << m_bufferIndex << " frames into serialized JSON payload..." << std::endl;
        
        // Lightweight in-line serialization loop
        std::stringstream jsonStream;
        jsonStream << "{\n  \"flightId\": " << m_localSensorStorage[0].flightId << ",\n  \"records\": [\n";
        for (size_t i = 0; i < m_bufferIndex; ++i) {
            const auto& frame = m_localSensorStorage[i];
            jsonStream << "    {\"ts\": " << frame.timestamp 
                       << ", \"flow\": " << frame.fuelFlowRate 
                       << ", \"temp\": " << frame.fuelTemperature 
                       << ", \"press\": " << frame.fuelPressure << "}";
            if (i < m_bufferIndex - 1) jsonStream << ",\n";
        }
        jsonStream << "\n  ]\n}";
        
        std::string payload = jsonStream.str();
        
        if (m_mqttConnection && *m_mqttConnection) {
            std::cout << "[AWS MQTT] Publishing stream payload to topic: quilt/telemetry/engine" << std::endl;
            
            Aws::Crt::ByteBuf payloadBuffer = Aws::Crt::ByteBufFromCString(payload.c_str());
            
            // Fire-and-forget payload publish with QoS 1 (At Least Once Delivery assurance)
            m_mqttConnection->Publish("quilt/telemetry/engine", AWS_MQTT_QOS_AT_LEAST_ONCE, false, payloadBuffer,
                [](Aws::Crt::Mqtt::MqttConnection&, uint16_t packetId, int errorCode) {
                    if (errorCode == 0) {
                        std::cout << "[SUCCESS] Telemetry frame package acknowledged by broker. Packet ID: " << packetId << std::endl;
                    } else {
                        std::cerr << "[ERROR] Broker publish failure event token: " << errorCode << std::endl;
                    }
                });
        } else {
            std::cerr << "[WARNING] MQTT Client connection link inactive. Dumping payload preview:\n" << payload << std::endl;
        }
#endif

#ifdef TARGET_CLOUD_GCP
        std::cout << "[GCP GCS] Streaming " << m_bufferIndex << " frames to AirFranceKLM raw bucket placeholder." << std::endl;
#endif

        m_bufferIndex = 0; // Clear local array sequence frame state cleanly
    }
};
*/
// ========================================================
// 3. THE RUNTIME INITIALIZATION LOOP
// ========================================================
int main(int argc, char* argv[]) {

//#ifdef TARGET_CLOUD_AWS
//    Aws::Crt::ApiHandle apiHandle;
//    std::cout << "Initializing Quilt Engine Telemetry Engine (C++)..." << std::endl;

//    const char* tes_uri = std::getenv("AWS_CONTAINER_CREDENTIALS_RELATIVE_URI");
//    if (tes_uri) {
//        std::cout << "[SUCCESS] Greengrass Token Bridge detected at: " << tes_uri << std::endl;
//    } else {
//        std::cout << "[WARNING] Local Greengrass environment variables not found." << std::endl;
//    }

//    Aws::Crt::Allocator* allocator = Aws::Crt::ApiAllocator();
//    Aws::Crt::Auth::CredentialsProviderChainDefaultConfig chainConfig;
//    std::shared_ptr<Aws::Crt::Auth::ICredentialsProvider> provider = 
//        Aws::Crt::Auth::CredentialsProvider::CreateCredentialsProviderChainDefault(chainConfig, allocator);

//    if (!provider) {
//        std::cerr << "[ERROR] Failed to initialize credentials provider chain." << std::endl;
//        return 1;
//    }
//    std::cout << "[SUCCESS] AWS C++ Credentials Provider successfully bound to local provider chain." << std::endl;

    // Build out the core MQTT client instance bootstrap layout 
//    Aws::Crt::Mqtt::MqttClient mqttClient(allocator);
//    auto clientConfigBuilder = Aws::Iot::MqttClientConnectionConfigBuilder::NewDefaultBuilder();
    
    // Set your targeted AWS IoT Core Endpoint target URL here
    // clientConfigBuilder.WithEndpoint("your-endpoint-here.iot.us-east-1.amazonaws.com");
    
//    auto clientConfig = clientConfigBuilder.Build();
//    auto mqttConnection = mqttClient.NewConnection(clientConfig);

//    std::cout << "Ready for telemetry pipeline integration loop." << std::endl;

    // Inject our active network layer connection straight into the simulation entity
//    FuelTelemetryComponent telemetryEngine(mqttConnection);
//#endif

//#ifdef TARGET_CLOUD_GCP
//    std::cout << "Initializing Quilt Engine Telemetry Engine (C++)..." << std::endl;
//    std::cout << "[GCP] Reading environment credentials chain..." << std::endl;
    
//    FuelTelemetryComponent telemetryEngine;
	Aircraft testPlane("N172DN", "DL1728", "MCO", "ATL");

	std::cout << "CLEARED!" << std::endl;
//#endif

//    std::cout << "\nStarting telemetry capture..." << std::endl;
//    for (int cycle = 1; cycle <= 3; ++cycle) {
//        telemetryEngine.Cyclical_Sensor_Read();
//    }
    
//    telemetryEngine.Flush_To_Bronze_Layer();

    // Give asynchronous ack loop callbacks a brief window to safely resolve before killing main thread execution context
//    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
