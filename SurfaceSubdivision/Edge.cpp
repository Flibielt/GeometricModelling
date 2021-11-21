#include "Edge.hpp"


Edge::Edge(/* args */)
{
    startVertexIndex = -1;
    endVertexIndex = -1;

    leftFaceIndex = -1;
    rightFaceIndex = -1;

    leftTraversePredecessorIndex = -1;
    leftTraverseSuccessorIndex = -1;

    rightTraversePredecessorIndex = -1;
    rightTraverseSuccessorIndex = -1;
}

Edge::~Edge()
{
}

void Edge::calculateCenter()
{
    center.point.x = (vertices[startVertexIndex].point.x + vertices[endVertexIndex].point.x) / 2;
    center.point.y = (vertices[startVertexIndex].point.y + vertices[endVertexIndex].point.y) / 2;
    center.point.z = (vertices[startVertexIndex].point.z + vertices[endVertexIndex].point.z) / 2;
}

bool Edge::containsVertex(Vertex *v)
{
    return vertices[startVertexIndex] == *v || vertices[endVertexIndex] == *v;
}

bool Edge::containsVertex(int index)
{
    return startVertexIndex == index || endVertexIndex == index;
}

bool Edge::containsVertices(Vertex *v1, Vertex *v2)
{
    return containsVertex(v1) && containsVertex(v2);
}

bool Edge::containsVertices(int index1, int index2)
{
    return containsVertex(index1) && containsVertex(index2);
}

std::vector<int> Edge::get1per8Vertices()
{
    std::vector<int> vertices;

    vertices.push_back(edges[this->leftTraversePredecessorIndex].endVertexIndex);
    vertices.push_back(edges[this->rightTraverseSuccessorIndex].endVertexIndex);

    return vertices;
}

std::vector<int> Edge::get1per16Vertices()
{
    std::vector<int> vertices;

    vertices.push_back(edges[edges[this->leftTraversePredecessorIndex].rightTraversePredecessorIndex].endVertexIndex);
    vertices.push_back(edges[edges[this->leftTraverseSuccessorIndex].rightTraverseSuccessorIndex].endVertexIndex);
    vertices.push_back(edges[edges[this->rightTraverseSuccessorIndex].leftTraverseSuccessorIndex].endVertexIndex);
    vertices.push_back(edges[edges[this->rightTraversePredecessorIndex].leftTraverseSuccessorIndex].endVertexIndex);

    return vertices;
}

bool Edge::operator==(const Edge& other)
{
    if (this->endVertexIndex == other.endVertexIndex && this->startVertexIndex == other.startVertexIndex)
        return true;
    else if (this->endVertexIndex == other.endVertexIndex && this->startVertexIndex == other.startVertexIndex)
        return true;
    
    return false;
}

bool Edge::operator!=(const Edge& other)
{
    return !(this == &other);
}
