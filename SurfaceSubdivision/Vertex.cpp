#include "Vertex.hpp"

Vertex::Vertex(/* args */)
{
    this->generated = false;
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

Vertex Vertex::operator+=(const Vertex& v1)
{
    this->point.x = this->point.x + v1.point.x;
    this->point.y = this->point.y + v1.point.y;
    this->point.z = this->point.z + v1.point.z;

    return *this;
}

Vertex Vertex::operator-=(const Vertex& v1)
{
    this->point.x = this->point.x - v1.point.x;
    this->point.y = this->point.y - v1.point.y;
    this->point.z = this->point.z - v1.point.z;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
    os << "X: " << v.point.x << ", Y: " << v.point.y << ", Z: " << v.point.z;
    return os;
}
