#include "Vertex.hpp"

Vertex::Vertex(/* args */)
{
}

Vertex::~Vertex()
{
}

bool Vertex::operator==(const Vertex& v1)
{
    return this->point == v1.point;
}

bool Vertex::operator!=(const Vertex& v1)
{
    return !(this == &v1);
}
