#pragma once
#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <cstdint> 
#include <vector>
#include <string>
#include <map>

#include "Shape.hpp"

const std::vector<std::string> MaterialNames = {
    "none",
    "emerald",
    "jade",
    "obsidian",
    "pearl",
    "ruby",
    "turquoise",
    "brass",
    "bronze",
    "chrome",
    "copper",
    "gold",
    "silver",
    "black plastic",
    "cyan plastic",
    "green plastic",
    "red plastic",
    "white plastic",
    "yellow plastic",
    "black rubber",
    "cyan rubber",
    "green rubber",
    "red rubber",
    "white rubber",
    "yellow rubber",
};

class Material{
    public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess): ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess){};
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    bool operator!= (const Material & mt)
    {
        return !((this -> ambient == mt.ambient)&&(this -> diffuse == mt.diffuse)&&(this -> specular == mt.specular)&&(this -> shininess == mt.shininess));
    }
};

class Object
{
public:
    Object(Shape shape) : shape(shape) {};
    ~Object() = default;

    Shape shape;
    Material material = Material(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(0.5f,0.5f,0.5f),0.5f);
    unsigned int selectedMaterial = 0;

    static inline std::map<std::string, Material> MaterialPresets;
    static inline int selected = 0;
    static bool initMaterial();
private:

};

#endif