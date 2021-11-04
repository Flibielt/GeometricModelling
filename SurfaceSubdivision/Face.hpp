#ifndef FACE_H
#define FACE_H

#include <vector>

#include "Edge.hpp"
#include "Vertex.hpp"

class Edge;

class Face
{
private:
    /* data */
public:
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    bool isEdgeInFace(Edge* edge);

    Face(/* args */);
    ~Face();
};

#endif