#ifndef DATA_H
#define DATA_H

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"

#include <vector>

// Needed tp predefine the edge and face class
class Edge;
class Face;

extern std::vector<Vertex> vertices;
extern std::vector<Edge> edges;
extern std::vector<Face> faces;

#endif