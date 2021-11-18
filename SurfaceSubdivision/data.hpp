#ifndef DATA_H
#define DATA_H

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"

#include <vector>

// Needed to predefine the edge and face class
class Edge;
class Face;

extern std::vector<Vertex> vertices;
extern std::vector<Edge> edges;
extern std::vector<Face> faces;

Edge createEdge(int faceIndex, int vertex1Index, int vertex2Index, bool &unique);
int findEdge(int vertex1Index, int vertex2Index);
bool isUniqueEdge(int vertex1, int vertex2, int &index);
void createEdges();
void setEdgeFaces();
void setEdgeTraverses();
void setEdgesForFaces();

#endif