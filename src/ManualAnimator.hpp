#pragma once
#ifndef MANUAL_ANIMATOR
#define MANUAL_ANIMATOR

#include "udpServer.hpp"
#include "Utilities.h"

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <array>
class ManualAnimator
{
private:
    /* data */
    std::unique_ptr<UDPServer> server;
    std::vector<glm::vec3> mixamobones;
    unsigned int temp = 0, head = 0, tail = 0;
    std::array<float, 3> vec3;

public:
    ManualAnimator(UDPServer *server);
    ~ManualAnimator();
    bool stringToBones(const std::string &raw);
    std::vector<glm::vec3> update();
};

#endif // !MANUAL_ANIMATOR