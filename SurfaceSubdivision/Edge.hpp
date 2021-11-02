#ifndef EDGE_H
#define EDGE_H

#include "Vertex.hpp"
#include "Face.hpp"

class Face;

class Edge
{
private:
    /* data */
public:
    Vertex* pStartPoint;
    Vertex* pEndPoint;

    Face* leftFace;
    Face* rightFace;

    Edge* leftTraversePredecessor;
    Edge* leftTraverseSuccessor;

    Edge* rightTraversePredecessor;
    Edge* rightTraverseSuccessor;

    Edge(/* args */);
    ~Edge();

    bool isEqual(Vertex point1, Vertex point2);
};


#endif