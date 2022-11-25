#include "ManualAnimator.hpp"

ManualAnimator::ManualAnimator(UDPServer *server)
{
    this->server.reset(server);
}

ManualAnimator::~ManualAnimator()
{
}

std::vector<glm::vec3> *ManualAnimator::update()
{
    stringToBones(*server->getMsg());
    return &mixamobones;
}

void ManualAnimator::stringToBones(const std::string &raw = "0.33, 0.4, 0.44, 0.1, 0.33, 0.4")
{
    unsigned int temp = 0, head = 0, tail = 0;
    std::array<float, 3> vec3;

    mixamobones.clear();

    if (raw.size() != 0)
    {
        while (tail < raw.length())
        {
            while (temp < 3)
            {
                tail++;
                if (raw[tail] == ',')
                {
                    vec3[temp] = std::stof(raw.substr(head, tail));
                    temp++;
                    head = tail + 1;
                }
            }
            mixamobones.push_back(glm::vec3(vec3[0], vec3[1], vec3[2]));
            temp = 0;
        }

        // Debug
        std::cout << "--------" << std::endl;
        for (auto it : mixamobones)
        {
            std::cout << printVec3(it) << std::endl;
        }
    }
}