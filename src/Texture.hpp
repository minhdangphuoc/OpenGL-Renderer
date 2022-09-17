#pragma once
#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <iostream>
#include <cstdint> 
#include <glad/glad.h> 
#include <string>

class Texture
{
public:
    uint32_t id;
    std::string type;
    std::string path;
};

#endif // __TEXTURE_HPP__