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

    for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
    {
        for (int i = 0; i < 3; i++)
        {
            Edge edge = createEdge(faces[faceIndex], faces[faceIndex].vertices[i], faces[faceIndex].vertices[(i + 1) % 3], unique);
            
            if (unique)
                edges.push_back(edge);
            
        }
    }
}

Edge ObjReader::createEdge(Face &face, Vertex *vertex1, Vertex *vertex2, bool &unique)
{
    std::vector<Edge>::iterator it;
    int index = 0;

    Edge edge;
    edge.pStartPoint = vertex1;
    edge.pEndPoint = vertex2;

    if (isUniqueEdge(&edge, index))
    {
        edge.faces.push_back(&face);
        unique = true;
    }
    else
    {
        // Search the existing edge and add the face
        unique = false;
        it = edges.begin();
        std::advance(it, index);
        (*it).faces.push_back(&face);
    }

    return edge;
}

void ObjReader::setEdgeFaces()
{
    Vertex *thirdVertex;
    float dotProduct;
    int index;

    for (int i = 0; i < edges.size(); i++)
    {
        index = i;
        thirdVertex = edges[i].faces[0]->getThirdVertex(&edges[i]);
        dotProduct = (thirdVertex->point.x - edges[i].pStartPoint->point.x) * 
        (edges[i].pEndPoint->point.y - edges[i].pStartPoint->point.y) * (edges[i].pEndPoint->point.z - edges[i].pStartPoint->point.z) - 
                    (edges[i].pEndPoint->point.x - edges[i].pStartPoint->point.x) * (thirdVertex->point.y - edges[i].pStartPoint->point.y) * (edges[i].pEndPoint->point.z - edges[i].pStartPoint->point.z) - 
                    (edges[i].pEndPoint->point.x - edges[i].pStartPoint->point.x) * (edges[i].pEndPoint->point.y - edges[i].pStartPoint->point.y) * (thirdVertex->point.z - edges[i].pStartPoint->point.z);
        /*
        if (dotProduct <= 0)
        {
            edges[i].leftFace = &faces[0];
            edges[i].rightFace = &faces[1];
        }
        else
        {
            edges[i].leftFace = &faces[1];
            edges[i].rightFace = &faces[0];
        }
        */
        edges[i].leftFace = edges[i].faces[0];
        edges[i].rightFace = edges[i].faces[1];
    }
}

void ObjReader::setEdgeTraverses()
{
    int vertexIndex;
    Edge *foundEdge;

    for (int i = 0; i < edges.size(); i++)
    {
        vertexIndex = edges[i].leftFace->getVertexIndex(edges[i].pStartPoint);
        edges[i].leftTraversePredecessor = findEdge(edges[i].leftFace->vertices[(vertexIndex - 1 + 3) % 3], edges[i].leftFace->vertices[vertexIndex]);
        edges[i].leftTraverseSuccessor = findEdge(edges[i].leftFace->vertices[(vertexIndex + 1) % 3], edges[i].leftFace->vertices[(vertexIndex + 2) % 3]);

        vertexIndex = edges[i].rightFace->getVertexIndex(edges[i].pEndPoint);
        edges[i].rightTraversePredecessor = findEdge(edges[i].rightFace->vertices[(vertexIndex - 1 + 3) % 3], edges[i].rightFace->vertices[vertexIndex]);
        edges[i].rightTraverseSuccessor = findEdge(edges[i].rightFace->vertices[(vertexIndex + 1) % 3], edges[i].rightFace->vertices[(vertexIndex + 2) % 3]);
    }
}

bool ObjReader::isUniqueEdge(Edge* newEdge, int &index)
{
    index = 0;
    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i] == *newEdge)
            return false;
        
        index++;
    }

    return true;
}

Edge* ObjReader::findEdge(Vertex *v1, Vertex *v2)
{
    Edge *foundEdge;
    for (int i = 0; i < edges.size(); i++)
        if (edges[i].containsVertices(v1, v2))
            foundEdge = &edges[i];
    
    return foundEdge;
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
        return std::stoi(vertex);

    vertexIndex = std::stoi(vertex.substr(0, index));
    // In the obj files vertex indexes starts with 1
    vertexIndex--; 
    
    return vertexIndex;
}
