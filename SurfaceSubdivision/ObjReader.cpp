#include "ObjReader.hpp"

int getVertexIndex(std::string vertex);

ObjReader::ObjReader(/* args */)
{
}

ObjReader::~ObjReader()
{
}

void ObjReader::readFile(std::string fileName)
{
    std::string line;
    std::fstream file;
    file.open(fileName, std::ios::in);

    if (file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            // Vertex
            // Fomat: v X Y Z
            if (line.rfind("v ", 0) == 0)
            {
                Vertex vertex = parseVertex(line);
                vertices.push_back(vertex);
            }
            // Face
            // Format: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            else if (line.rfind("f ", 0) == 0) {
                Face face = parseFace(line);
                face.sortVertices();
                faces.push_back(face);
            }
        }
    }
    
    file.close();

    createEdges();
    setEdgeFaces();
    setEdgeTraverses();

    setEdgesForFaces();
}

Vertex ObjReader::parseVertex(std::string line)
{
    Vertex vertex;
    std::stringstream text(line);
    std::string segment;

    int index = 0;
    float coordinate;
    while (std::getline(text, segment, ' '))
    {
        if (segment.length() < 1)
            continue;
        
        if (index == 1)
        {
            coordinate = std::stof(segment);
            vertex.point.x = coordinate;
        }
        else if (index == 2)
        {
            coordinate = std::stof(segment);
            vertex.point.y = coordinate;
        }
        else if (index == 3)
        {
            coordinate = std::stof(segment);
            vertex.point.z = coordinate;
        }

        index++;
    }

    return vertex;
}

Face ObjReader::parseFace(std::string line)
{
    Face face;
    std::stringstream text(line);
    std::string segment;

    int index = 0;
    int vertexIndex;
    while (std::getline(text, segment, ' '))
    {
        if (segment.length() < 1)
            continue;
        
        if (index > 0)
        {
            vertexIndex = getVertexIndex(segment);
            face.verticesIndex.push_back(vertexIndex);
        }

        index++;
    }

    return face;
}

int getVertexIndex(std::string vertex)
{
    int vertexIndex = 0;
    std::size_t index = vertex.find("/");

    if (index == std::string::npos)
        return std::stoi(vertex);

    vertexIndex = std::stoi(vertex.substr(0, index));
    // In the obj files vertex indexes starts with 1
    vertexIndex--; 
    
    return vertexIndex;
}
