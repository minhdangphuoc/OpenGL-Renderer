#include <cstdint> 
#include <vector>
#include "Shape.hpp"
class Object
{
public:
    Object(Shape shape) : shape(shape) {};
    ~Object() {};
    Shape shape;
    
private:

};