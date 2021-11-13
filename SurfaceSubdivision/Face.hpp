#ifndef FACE_H
#define FACE_H

#include <vector>

#include "Edge.hpp"
#include "Vertex.hpp"
#include "MathUtil.hpp"

class Edge;

class Face
{
private:
    Vertex center;

    void calculateCenter();
public:
    std::vector<int> verticesIndex;
    std::vector<int> edgesIndex;
    bool isEdgeInFace(Edge* edge);
    bool isEdgeInFace(int index);
    void sortVertices();

    int getThirdVertex(int index);
    int getVertexPosition(int vertexIndex);

    Face(/* args */);
    ~Face();
};

#endif