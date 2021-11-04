#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Vertex.hpp"
#include "Face.hpp"

class Face;

class Edge
{
private:
    
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

    Edge(/* args */);
    ~Edge();

    bool operator==(const Edge& other);
    bool operator!=(const Edge& other);
};


#endif