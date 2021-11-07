#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Vertex.hpp"
#include "Face.hpp"

class Face;

class Edge
{
private:
    Vertex center;
public:
    Vertex* pStartPoint;
    Vertex* pEndPoint;

    std::vector<Face*> faces;
    Face* leftFace;
    Face* rightFace;

    Edge* leftTraversePredecessor;
    Edge* leftTraverseSuccessor;

    Edge* rightTraversePredecessor;
    Edge* rightTraverseSuccessor;

    std::vector<Vertex*> get1per8Vertices();
    std::vector<Vertex*> get1per16Vertices();

    Edge(/* args */);
    ~Edge();

    void calculateCenter();
    bool containsVertices(Vertex *v1, Vertex *v2);

    bool operator==(const Edge& other);
    bool operator!=(const Edge& other);
};


#endif