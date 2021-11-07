#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>

#include "glm/glm.hpp"

class Vertex
{
private:
    /* data */
public:
    glm::vec3 point;
    bool generated;
    
    Vertex(/* args */);
    ~Vertex();

    bool operator==(const Vertex& v1);
    bool operator!=(const Vertex& v1);

    Vertex operator+=(const Vertex& v1);
    Vertex operator-=(const Vertex& v1);

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v);
};

#endif