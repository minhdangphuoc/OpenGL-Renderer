#pragma once
#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Utilities.h"
  

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const std::string vertexPath, const std::string fragmentPath);
    // use/activate the shader
    void use() { glUseProgram(ID); };
    // utility uniform functions
    void setBool(const std::string &name, bool value) const 
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
private:

void checkCompileErrors(unsigned int shader, std::string type);

};
  
#endif // __SHADER_HPP__
