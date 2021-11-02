#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

class Vertex
{
private:
    /* data */
public:
    glm::vec3 point;
    
    Vertex(/* args */);
    ~Vertex();

    bool operator==(const Vertex& v1);
    bool operator!=(const Vertex& v1);
};

#endif