#include <cstdint> 
#include <vector>
#include "Shape.hpp"
class Object
{
public:
    Object(Shape shape) : shape(shape) {};
    ~Object() {};
    uint32_t VBO, VAO, EBO;
    Shape shape;
    
private:

};