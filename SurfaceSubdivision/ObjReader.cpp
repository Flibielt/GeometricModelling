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
        if (index > 0)
        {
            vertexIndex = getVertexIndex(segment);
            face.verticesIndex.push_back(vertexIndex);
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
            int vertex1Index, vertex2Index;
            int vertex1, vertex2;

            vertex1Index = i;
            vertex2Index = (i + 1) % 3;

            vertex1 = faces[faceIndex].verticesIndex[vertex1Index];
            vertex2 = faces[faceIndex].verticesIndex[vertex2Index];
            
            Edge edge = createEdge(faceIndex, vertex1, vertex2, unique);
            
            if (unique)
                edges.push_back(edge);
            
        }
    }
}

Edge ObjReader::createEdge(int faceIndex, int vertex1Index, int vertex2Index, bool &unique)
{
    int edgeIndex = 0;
    Edge edge;

    if (isUniqueEdge(vertex1Index, vertex2Index, edgeIndex))
    {
        edge.startVertexIndex = vertex1Index;
        edge.endVertexIndex = vertex2Index;

        edge.faces.push_back(faceIndex);
        unique = true;
    }
    else
    {
        // Search the existing edge and add the face
        unique = false;
        edges[edgeIndex].faces.push_back(faceIndex);
    }
    
    return edge;
}

void ObjReader::setEdgeFaces()
{
    int thirdVertexIndex;
    int startPointIndex, endPointIndex;
    float dotProduct;

    for (int i = 0; i < edges.size(); i++)
    {
        startPointIndex = edges[i].startVertexIndex;
        endPointIndex = edges[i].endVertexIndex;

        thirdVertexIndex = faces[edges[i].faces[0]].getThirdVertex(i);

        dotProduct = (vertices[thirdVertexIndex].point.x - vertices[startPointIndex].point.x) * 
            (vertices[endPointIndex].point.y - vertices[startPointIndex].point.y) * (vertices[endPointIndex].point.z - vertices[startPointIndex].point.z) - 
            (vertices[endPointIndex].point.x - vertices[startPointIndex].point.x) * (vertices[thirdVertexIndex].point.y - vertices[startPointIndex].point.y) * (vertices[endPointIndex].point.z - vertices[startPointIndex].point.z) - 
            (vertices[endPointIndex].point.x - vertices[startPointIndex].point.x) * (vertices[endPointIndex].point.y - vertices[startPointIndex].point.y) * (vertices[thirdVertexIndex].point.z - vertices[startPointIndex].point.z);

        edges[i].leftFaceIndex = edges[i].faces[0];
        edges[i].rightFaceIndex = edges[i].faces[1];
    }
}

void ObjReader::setEdgeTraverses()
{
    int vertexPosition;
    Edge *foundEdge;

    for (int i = 0; i < edges.size(); i++)
    {
        int leftFaceIndex, rightFaceIndex;
        leftFaceIndex = edges[i].leftFaceIndex;
        rightFaceIndex = edges[i].rightFaceIndex;
        
        vertexPosition = faces[leftFaceIndex].getVertexPosition(edges[i].startVertexIndex);
        edges[i].leftTraversePredecessorIndex = findEdge(faces[leftFaceIndex].verticesIndex[(vertexPosition - 1 + 3) % 3], faces[leftFaceIndex].verticesIndex[vertexPosition]);
        edges[i].leftTraverseSuccessorIndex = findEdge(faces[leftFaceIndex].verticesIndex[(vertexPosition + 1) % 3], faces[leftFaceIndex].verticesIndex[(vertexPosition + 2) % 3]);

        vertexPosition = faces[rightFaceIndex].getVertexPosition(edges[i].endVertexIndex);
        edges[i].rightTraversePredecessorIndex = findEdge(faces[rightFaceIndex].verticesIndex[(vertexPosition - 1 + 3) % 3], faces[rightFaceIndex].verticesIndex[vertexPosition]);
        edges[i].rightTraverseSuccessorIndex = findEdge(faces[rightFaceIndex].verticesIndex[(vertexPosition + 1) % 3], faces[rightFaceIndex].verticesIndex[(vertexPosition + 2) % 3]);
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

bool ObjReader::isUniqueEdge(int vertex1, int vertex2, int &index)
{
    index = -1;
    for (int i = 0; i < edges.size(); i++)
    {
        if (edges[i].startVertexIndex == vertex1 && edges[i].endVertexIndex == vertex2)
        {
            index = i;
            return false;
        }
        else if (edges[i].endVertexIndex == vertex1 && edges[i].startVertexIndex == vertex2)
        {
            index = i;
            return false;
        }
    }

    return true;
}

int ObjReader::findEdge(int vertex1Index, int vertex2Index)
{
    for (int i = 0; i < edges.size(); i++)
        if (edges[i].containsVertices(vertex1Index, vertex2Index))
            return i;
    
    return -1;
}

void ObjReader::setEdgesForFaces()
{
    for (int edgeIndex = 0; edgeIndex < edges.size(); edgeIndex++)
    {
        faces[edges[edgeIndex].leftFaceIndex].edgesIndex.push_back(edgeIndex);
        faces[edges[edgeIndex].rightFaceIndex].edgesIndex.push_back(edgeIndex);
    }
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
