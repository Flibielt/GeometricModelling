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
    Vertex *pStartPoint;
    Vertex *pEndPoint;
    Vertex *generatedVertex;

    std::vector<Face*> faces;
    Face *leftFace;
    Face *rightFace;

    Edge *leftTraversePredecessor;
    Edge *leftTraverseSuccessor;

    Edge *rightTraversePredecessor;
    Edge *rightTraverseSuccessor;

    /**
     * @brief Get vertices for subdivision. These vertices will be multiplied by 1/8.
     * 
     * @return std::vector<Vertex*> The vertices
     */
    std::vector<Vertex*> get1per8Vertices();
    /**
     * @brief Get vertices for subdivision. These vertices will be multiplied by 1/16.
     * 
     * @return std::vector<Vertex*> The vertices
     */
    std::vector<Vertex*> get1per16Vertices();

    Edge(/* args */);
    ~Edge();

    void calculateCenter();
    bool containsVertex(Vertex *v);
    bool containsVertices(Vertex *v1, Vertex *v2);

    bool operator==(const Edge& other);
    bool operator!=(const Edge& other);
};


#endif