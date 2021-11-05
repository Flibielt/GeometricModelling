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
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    bool isEdgeInFace(Edge* edge);
    void sortVertices();

    Face(/* args */);
    ~Face();
};

#endif