#include "Face.hpp"

#include <iostream>

Face::Face(/* args */)
{
}

Face::~Face()
{
}

bool Face::isEdgeInFace(Edge* edge)
{
    bool startFound = false, endFound = false;
    
    for (Vertex* Vertex : vertices)
    {
        if (Vertex->point == edge->pStartPoint->point)
            startFound = true;
        else if (Vertex->point == edge->pEndPoint->point)
            endFound = true;
        
        if (startFound && endFound)
            break;
    }

    return startFound && endFound;
}

void Face::sortVertices()
{
    Vertex *swap;
    calculateCenter();

    std::cout << std::endl;

    for (int i = 0; i < vertices.size() - 1; i++)
        for (int j = 0; j < vertices.size() - i - 1; j++)
            if (comparePointOrder(&center, vertices[i], vertices[j]))
            {
                swap = vertices[i];
                vertices[i] = vertices[j];
                vertices[j] = swap;
            }
}

void Face::calculateCenter()
{
    center.point.x = 0.0;
    center.point.y = 0.0;
    center.point.z = 0.0;

    for (Vertex *vertex : vertices)
    {
        center.point.x += vertex->point.x;
        center.point.y += vertex->point.y;
        center.point.z += vertex->point.z;
    }

    center.point.x /= vertices.size();
    center.point.y /= vertices.size();
    center.point.z /= vertices.size();
}

Vertex* Face::getThirdVertex(Edge* edge)
{
    Vertex *foundVertex;

    for (int i = 0; i< vertices.size(); i++)
        if (*vertices[i] != *(edge->pStartPoint) && *vertices[i] != *(edge->pEndPoint))
            foundVertex = vertices[i];
    
    return foundVertex;
}

int Face::getVertexIndex(Vertex *vertex)
{
    for (int index = 0; index < vertices.size(); index++)
        if (*vertices[index] == *vertex)
            return index;
    
    return -1;
}
