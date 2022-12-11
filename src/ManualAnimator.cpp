#include "ManualAnimator.hpp"

ManualAnimator::ManualAnimator(UDPServer *server)
{
    this->server.reset(server);
}

ManualAnimator::~ManualAnimator()
{
}

std::vector<glm::vec3> ManualAnimator::update()
{
    stringToBones(*server->getMsg());
    return mixamobones;
}

bool ManualAnimator::stringToBones(const std::string &raw = "0.33, 0.4, 0.44, 0.1, 0.33, 0.4")
{
    temp = 0;
    head = 0;
    tail = 0;
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
                    vec3[temp] = std::stof(raw.substr(head, tail)) * 5.0f;
                    temp++;
                    head = tail + 1;
                }
            }
            mixamobones.push_back(glm::vec3(vec3[0], vec3[1], vec3[2]));
            temp = 0;
        }

        // Debug
        temp = 0;
        std::cout << "--------" << std::endl;
        for (auto it : mixamobones)
        {

            if (temp >= 11 && temp <= 16)
                std::cout << temp << ":" << printVec3(it) << std::endl;
            temp++;
        }
    }
    else
        return false;

    return true;
}