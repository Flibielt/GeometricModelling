#include "Edge.hpp"


Edge::Edge(/* args */)
{
}

Edge::~Edge()
{
}

bool Edge::operator==(const Edge& other)
{
    if (this->pEndPoint == other.pEndPoint && this->pStartPoint == other.pStartPoint)
    {
        return true;
    }
    else if (this->pStartPoint == other.pEndPoint && this->pEndPoint == other.pStartPoint)
    {
        return true;
    }
    
    return false;
}

bool Edge::operator!=(const Edge& other)
{
    return !(this == &other);
}
