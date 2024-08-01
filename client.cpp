#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    std::string message;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    while (true) {
        std::cout << "Enter message to send to server (type 'exit' to quit): ";
        std::getline(std::cin, message);
        
        if (message == "exit") {
            break;
        }

        send(sock, message.c_str(), message.size(), 0);
        std::cout << "Message sent to server: " << message << std::endl;

        ssize_t valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            std::cout << "Received from server: " << buffer << std::endl;
        }

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
    }

    close(sock);
    return 0;
}

