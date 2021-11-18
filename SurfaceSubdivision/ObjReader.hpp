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
    
public:
    ObjReader(/* args */);
    ~ObjReader();

    void readFile(std::string fileName);
};

#endif