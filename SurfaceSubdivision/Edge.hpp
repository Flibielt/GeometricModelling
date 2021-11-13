#ifndef EDGE_H
#define EDGE_H

#include <vector>

#include "Vertex.hpp"
#include "Face.hpp"
#include "data.hpp"

class Face;

class Edge
{
private:
    Vertex center;
public:
    int startVertexIndex, endVertexIndex;
    int generatedVertexIndex;

    std::vector<int> faces;
    int leftFaceIndex, rightFaceIndex;

    int leftTraversePredecessorIndex, leftTraverseSuccessorIndex;
    int rightTraversePredecessorIndex, rightTraverseSuccessorIndex;

    /**
     * @brief Get vertices for subdivision. These vertices will be multiplied by 1/8.
     * 
     * @return std::vector<int> The indexes of the vertices
     */
    std::vector<int> get1per8Vertices();

    /**
     * @brief Get vertices for subdivision. These vertices will be multiplied by 1/16.
     * 
     * @return std::vector<int> The indexes of the vertices
     */
    std::vector<int> get1per16Vertices();

    Edge(/* args */);
    ~Edge();

    void calculateCenter();
    bool containsVertex(Vertex *v);
    bool containsVertex(int index);
    bool containsVertices(Vertex *v1, Vertex *v2);
    bool containsVertices(int index1, int index2);

    bool operator==(const Edge& other);
    bool operator!=(const Edge& other);
};


#endif