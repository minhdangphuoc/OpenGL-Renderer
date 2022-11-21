#pragma once
#ifndef UDP_SERVER
#define UDP_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>
#include <string>
#include <thread>

class UDPServer
{
public:
    UDPServer *getInstance()
    {
        if (instance == nullptr)
            instance = new UDPServer();
        return instance;
    }

    // Supporting Funtions
    void runServer( 
        const std::string &UDPAddress = "192.168.1.1", // UDP Address
        const std::string &UDPPort = "8080",           // UDP Port
        const uint32_t &bufferSize = 64 * 1024         // Buffer Size
    )
    {
    }

    std::string receiveMessage()
    {
        return "Test Msg";
    }

    void sendMessage(const std::string &msg)
    {
        try
        {
            std::cout << "This is dummy message" << std::endl;
        }
        catch (const std::runtime_error &err)
        {
            std::cerr << err.what() << std::endl;
        }
    }

protected:
    // Singleton
    UDPServer() = default;
    UDPServer *instance = nullptr;

    // Variables
    std::string UDPAddress = "192.168.1.1";
    std::string UDPPort = "8080";
    uint32_t bufferSize = 64 * 1024;
};

#endif // !UDP_SERVER