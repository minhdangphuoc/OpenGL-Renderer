#pragma once
#ifndef UDP_SERVER
#define UDP_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>
#include <string>
#include <thread>
#include <atomic>
#include <cstring>
#include <climits>
#define MAXLINE 65536
#define PORT 8080

class UDPServer
{
public:
    static UDPServer *getInstance()
    {
        if (instance == nullptr)
            instance = new UDPServer();
        return instance;
    }

    // Supporting Funtions
    void runServer(
        const std::string &UDPAddress,         // UDP Address
        const uint16_t &UDPPort = 8080,        // UDP Port
        const uint32_t &bufferSize = 64 * 1024 // Buffer Size
    )
    {
        // int flags = fcntl(sockfd, F_GETFL);
        // flags |= O_NONBLOCK;
        // fcntl(sockfd, F_SETFL, flags);

        // Creating socket file descriptor
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        // servaddr.sin_addr.s_addr = inet_addr(UDPAddress.c_str());
        servaddr.sin_addr.s_addr = inet_addr("192.168.0.83");
        servaddr.sin_port = htons(UDPPort);

        // Bind the socket with the server address
        if (bind(sockfd, (const struct sockaddr *)&servaddr,
                 sizeof(servaddr)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        len = sizeof(cliaddr);
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
    }

    void receiveMessage()
    {
        c_buffer = global_buffer;
        this->n = recvfrom(this->sockfd, (char *)this->c_buffer, MAXLINE, 0, (struct sockaddr *)&this->cliaddr, &this->len);
        // std::cout << c_buffer << std::endl;

        // printf("Receive : %s\n", this->c_buffer);

        if (n != UINT_MAX)
        {
            c_buffer[n] = '\0';
            msg = c_buffer;
        }
    }

    static void receiveMessageMT(std::atomic<bool> &program_is_running, unsigned int update_interval_millisecs)
    {
        unsigned int len, n;
        char *c_buffer;
        const auto wait_duration = std::chrono::milliseconds(update_interval_millisecs);
        struct timeval read_timeout;

        while (program_is_running)
        {
            try
            {
                // read_timeout.tv_sec = 0;
                // read_timeout.tv_usec = 10;
                // setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
                // Receive
                len = sizeof(cliaddr);
                n = recvfrom(sockfd, (char *)c_buffer, MAXLINE, MSG_DONTWAIT, (struct sockaddr *)&cliaddr, &len);
                // std::cout<< msg << ", " << n << std::endl;
                if (n != UINT_MAX)
                {
                    c_buffer[n] = '\0';
                    printf("Receive : %s\n", c_buffer);
                    msg = c_buffer;
                }
            }
            catch (const std::runtime_error &err)
            {
                std::cerr << err.what() << std::endl;
            }

            std::this_thread::sleep_for(wait_duration);
        }
    }

    std::string *getMsg()
    {
        receiveMessage();
        return &(this->msg);
    }
    void startRunning()
    {
        // const unsigned int interval = 5; // update after every 50 milliseconds
        // std::thread thread(receiveMessageMT, std::ref(running), interval);
        // thread.join();
    }
    void stopRunning()
    {
        running = false;
    }

protected:
    // Singleton
    UDPServer() = default;
    static inline UDPServer *instance = nullptr;

    // Variables
    std::string UDPAddress = "192.168.0.83";
    uint16_t UDPPort = 8080;
    unsigned int len, n;
    char *c_buffer;
    struct timeval read_timeout;

    //
    static inline std::string msg;
    static inline int sockfd;
    std::atomic<bool> running{true};
    char global_buffer[MAXLINE];
    static inline struct sockaddr_in servaddr, cliaddr;
};

#endif // !UDP_SERVER