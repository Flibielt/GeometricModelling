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
                faces.push_back(face);
            }
        }
    }
    
    file.close();

    createEdges();
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

    std::vector<Vertex>::iterator it;

    int index = 0;
    int vertexIndex;
    while (std::getline(text, segment, ' '))
    {
        if (index > 0)
        {
            vertexIndex = getVertexIndex(segment);

            it = vertices.begin();
            std::advance(it, vertexIndex);

            face.vertices.push_back(&(*it));
        }

        index++;
    }

    return face;
}

void ObjReader::createEdges()
{
    for (Face f : faces)
    {
        Edge edge1;
        edge1.pStartPoint = f.vertices[0];
        edge1.pEndPoint = f.vertices[1];

        if (isUniqueEdge(&edge1))
            edges.push_back(edge1);

        Edge edge2;
        edge2.pStartPoint = f.vertices[1];
        edge2.pEndPoint = f.vertices[2];
        if (isUniqueEdge(&edge2))
            edges.push_back(edge2);

        Edge edge3;
        edge3.pStartPoint = f.vertices[2];
        edge3.pEndPoint = f.vertices[0];

        if (isUniqueEdge(&edge3))
            edges.push_back(edge3);
    }
}

bool ObjReader::isUniqueEdge(Edge* newEdge)
{
    for (Edge edge : edges)
    {
        if (edge == *newEdge)
            return false;
    }

    return true;
}

std::vector<Vertex> ObjReader::getVertices()
{
    return vertices;
}

std::vector<Edge> ObjReader::getEdges()
{
    return edges;
}

std::vector<Face> ObjReader::getFaces()
{
    return faces;
}


int getVertexIndex(std::string vertex)
{
    int vertexIndex = 0;
    std::size_t index = vertex.find("/");

    if (index == std::string::npos)
    {
        return std::stoi(vertex);
    }

    vertexIndex = std::stoi(vertex.substr(0, index));
    // In the obj files vertex indexes starts with 1
    vertexIndex--; 
    
    return vertexIndex;
}
