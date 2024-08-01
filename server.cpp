#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "prometheus/counter.h"
#include "prometheus/exposer.h"
#include "prometheus/registry.h"
#include "prometheus/histogram.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket, prometheus::Counter& request_counter, prometheus::Histogram& response_histogram) {
    char buffer[BUFFER_SIZE] = {0};

    auto start_time = std::chrono::steady_clock::now();
    ssize_t valread = read(client_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        std::cout << "Received: " << buffer << std::endl;
        send(client_socket, buffer, strlen(buffer), 0);
    }
    close(client_socket);
    
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> response_time = end_time - start_time;
    response_histogram.Observe(response_time.count());
    request_counter.Increment();
}

int main() {
    using namespace prometheus;

    // Set up Prometheus metrics
    Exposer exposer{"127.0.0.1:8081"};
    auto registry = std::make_shared<Registry>();
    auto& request_counter_family = BuildCounter()
                                .Name("requests_total")
                                .Help("Total number of requests processed")
                                .Register(*registry);
    auto& response_histogram_family = BuildHistogram()
                                   .Name("response_time_seconds")
                                   .Help("Response times in seconds")
                                   .Register(*registry);

    exposer.RegisterCollectable(registry);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Get references to the actual Counter and Histogram from the Families
        auto& request_counter = request_counter_family.Add({});
        // Specify bucket boundaries when adding Histogram metric
        auto& response_histogram = response_histogram_family.Add({}, Histogram::BucketBoundaries{0.001, 0.01, 0.1, 1.0});

        std::thread(handle_client, new_socket, std::ref(request_counter), std::ref(response_histogram)).detach();
    }

    close(server_fd);
    return 0;
}

