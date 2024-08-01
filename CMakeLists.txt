cmake_minimum_required(VERSION 3.11)
project(PrometheusClientServer)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

# Fetch the Prometheus C++ client library
FetchContent_Declare(
  prometheus_cpp
  GIT_REPOSITORY https://github.com/jupp0r/prometheus-cpp.git
  GIT_TAG        v0.12.3 # Specify the desired version
)
FetchContent_MakeAvailable(prometheus_cpp)

# Add executables for client and server
add_executable(client client.cpp)
add_executable(server server.cpp)

# Link Prometheus libraries to both client and server
target_link_libraries(client PRIVATE prometheus-cpp::core prometheus-cpp::pull)
target_link_libraries(server PRIVATE prometheus-cpp::core prometheus-cpp::pull)

# Optionally, you can set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

