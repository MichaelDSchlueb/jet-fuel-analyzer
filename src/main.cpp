#include <aws/crt/Api.h>
#include <aws/crt/auth/Credentials.h>
#include <iostream>
#include <thread>
#include <future>

int main(int argc char* argv) {
    Aws::Crt::ApiHandle apiHandle;
    //apiHandle.InitializeLogging(Aws::Crt::LogLevel::Trace, stderr);
    Aws::Crt::Allocator* allocator = Aws::Crt::ApiAllocator();

    std::cout << "[TRACK] Checking default credentials chain assembly..." << std::endl;

    Aws::Crt::Auth::CredentialsProviderChainDefaultConfig config;
    auto provider = Aws::Crt::Auth::CredentialsProvider::CreateCredentialsProviderChainDefault(config, allocator);

    if (!provider) {
        std::cerr << "[FAIL] Provider allocation returned nullptr immediately." << std::endl;
        return 1;
    }

    std::cout << "[TRACK] Provider allocated safely. Attempting asynchronous credential pull..." << std::endl;

    std::promise<bool> done;
    auto future = done.get_future();

    // Force the provider to asynchronously fetch the EC2 IAM Role token
    provider->GetCredentials([&done](const std::shared_ptr<Aws::Crt::Auth::Credentials>& credentials, int errorCode) {
        if (errorCode != 0 || !credentials) {
            std::cerr << "[FAIL] Asynchronous retrieval failed. AWS Error Code: " << errorCode << std::endl;
            done.set_value(false);
            return;
        }
        std::cout << "[SUCCESS] Token grabbed successfully!" << std::endl;
        std::cout << "Access Key Present: " << (credentials->GetAccessKeyId().len > 0 ? "Yes" : "No") << std::endl;
        done.set_value(true);
    });

    // Wait up to 5 seconds for the IMDS network hop to finish or time out
    if (future.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
        std::cerr << "[TIMEOUT] IMDS did not respond in time." << std::endl;
    }

    std::cout << "[TRACK] Clean shutdown sequence." << std::endl;
    return 0;
}
