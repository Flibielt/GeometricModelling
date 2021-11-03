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

    bool isUniqueEdge(Edge* newEdge);
    void createEdges();
public:
    ObjReader(/* args */);
    ~ObjReader();

    void readFile(std::string fileName);
    std::vector<Vertex> getVertices();
    std::vector<Edge> getEdges();
    std::vector<Face> getFaces();
};

#endif