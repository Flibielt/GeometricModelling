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
    for (int index : verticesIndex)
    {
        if (index == edge->startVertexIndex)
            return true;
        else if (index == edge->endVertexIndex)
            return true;
    }

    return false;
}

void Face::sortVertices()
{
    int swap;
    calculateCenter();

    std::cout << std::endl;

    for (int i = 0; i < verticesIndex.size() - 1; i++)
        for (int j = 0; j < verticesIndex.size() - i - 1; j++)
            if (comparePointOrder(&center, verticesIndex[i], verticesIndex[j]))
            {
                swap = verticesIndex[i];
                verticesIndex[i] = verticesIndex[j];
                verticesIndex[j] = swap;
            }
}

void Face::calculateCenter()
{
    center.point.x = 0.0;
    center.point.y = 0.0;
    center.point.z = 0.0;

    for (int index : verticesIndex)
    {
        center.point.x += vertices[index].point.x;
        center.point.y += vertices[index].point.y;
        center.point.z += vertices[index].point.z;
    }

    center.point.x /= vertices.size();
    center.point.y /= vertices.size();
    center.point.z /= vertices.size();
}

int Face::getThirdVertex(int edgeIndex)
{
    int foundVertexIndex;

    for (int i = 0; i< verticesIndex.size(); i++)
        if (verticesIndex[i] != edges[edgeIndex].startVertexIndex && verticesIndex[i] != edges[edgeIndex].endVertexIndex)
            foundVertexIndex = verticesIndex[i];
    
    return foundVertexIndex;
}

int Face::getVertexPosition(int vertexIndex)
{
    for (int i = 0; i < verticesIndex.size(); i++)
    {
        if (verticesIndex[i] == vertexIndex)
        {
            return i;
        }
    }
    return -1;
}
