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

#include "data.hpp"

class ObjReader
{
private:

    Vertex parseVertex(std::string line);
    Face parseFace(std::string line);
    Edge createEdge(int faceIndex, int vertex1Index, int vertex2Index, bool &unique);
    int findEdge(int vertex1Index, int vertex2Index);

    bool isUniqueEdge(Edge* newEdge, int &index);
    bool isUniqueEdge(int vertex1, int vertex2, int &index);
    void createEdges();
    void setEdgeFaces();
    void setEdgeTraverses();
    void setEdgesForFaces();
public:
    ObjReader(/* args */);
    ~ObjReader();

    void readFile(std::string fileName);
};

#endif