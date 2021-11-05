#include "Edge.hpp"


Edge::Edge(/* args */)
{
}

Edge::~Edge()
{
}

void Edge::calculateCenter()
{
    center.point.x = (pStartPoint->point.x + pEndPoint->point.x) / 2;
    center.point.y = (pStartPoint->point.y + pEndPoint->point.y) / 2;
    center.point.z = (pStartPoint->point.z + pEndPoint->point.z) / 2;
}

bool Edge::containsVertices(Vertex *v1, Vertex *v2)
{
    if (*(this->pStartPoint) == *v1 && *(this->pEndPoint) == *v2)
        return true;
    else if (*(this->pEndPoint) == *v1 && *(this->pStartPoint) == *v2)
        return true;
    
    return false;
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
