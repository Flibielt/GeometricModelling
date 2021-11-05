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
    std::vector<Edge>::iterator it;
    int index;
    bool unique;

    for (Face f : faces)
    {
        for (int i = 0; i < 3; i++)
        {
            Edge edge = createEdge(&f, f.vertices[i], f.vertices[(i + 1) % 3], unique);
            
            if (unique)
                edges.push_back(edge);
            
        }
    }
}

Edge ObjReader::createEdge(Face *face, Vertex *vertex1, Vertex *vertex2, bool &unique)
{
    std::vector<Edge>::iterator it;
    int index = 0;

    Edge edge;
    edge.pStartPoint = vertex1;
    edge.pEndPoint = vertex2;

    if (isUniqueEdge(&edge, index))
    {
        edge.faces.push_back(face);
        unique = true;
    }
    else
    {
        // Search the existing edge and add the face
        unique = false;
        it = edges.begin();
        std::advance(it, index);
        (*it).faces.push_back(face);
    }

    return edge;
}

void ObjReader::connectEdgesWithFaces()
{
    Face *face1, *face2;
    bool face1Found = false;

    for (Edge edge : edges)
    {
        face1Found = false;
        for (Face face : faces)
        {
            if (!face1Found && face.isEdgeInFace(&edge))
            {
                face1 = &face;
                face1Found = true;
            }
            else if (face.isEdgeInFace(&edge))
            {
                face2 = &face;
                break;
            }
        }
    }
}

bool ObjReader::isUniqueEdge(Edge* newEdge, int &index)
{
    index = 0;
    for (Edge edge : edges)
    {
        if (edge == *newEdge)
            return false;
        
        index++;
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
