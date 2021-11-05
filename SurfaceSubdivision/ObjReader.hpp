#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"

class ObjReader
{
private:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    Vertex parseVertex(std::string line);
    Face parseFace(std::string line);
    Edge createEdge(Face &face, Vertex *vertex1, Vertex *vertex2, bool &unique);
    Edge* findEdge(Vertex *v1, Vertex *v2);

    bool isUniqueEdge(Edge* newEdge, int &index);
    void createEdges();
    void setEdgeFaces();
    void setEdgeTraverses();
public:
    ObjReader(/* args */);
    ~ObjReader();

    void readFile(std::string fileName);
    std::vector<Vertex> getVertices();
    std::vector<Edge> getEdges();
    std::vector<Face> getFaces();
};

#endif