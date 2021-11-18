#include "ModifiedButterflySubdivision.hpp"

#include <iostream>

ModifiedButterflySubdivision::ModifiedButterflySubdivision(/* args */)
{
}

ModifiedButterflySubdivision::~ModifiedButterflySubdivision()
{
}


void ModifiedButterflySubdivision::subdivide()
{
    int oldEdgeCount, newEdgeCount;
    int oldVertexCount, newVertexCount;

    oldEdgeCount = edges.size();
    oldVertexCount = vertices.size();

    splitEdges();

    newVertexCount = vertices.size();

    createNewFaces();

    clear();    
    
    createEdges();

    setEdgeFaces();
    setEdgeTraverses();
    setEdgesForFaces();

    newEdgeCount = edges.size();
    newVertexCount = vertices.size();
    std::cout << "Old edge count: " << oldEdgeCount << std::endl << "New edge count: " << newEdgeCount << std::endl;
    std::cout << "Old vertex count: " << oldVertexCount << std::endl << "New vertex count: " << newVertexCount << std::endl;
    int i = 0;
}

void ModifiedButterflySubdivision::clear()
{
    int faceCount, newFaceCount;
    faceCount = faces.size();
    newFaceCount = newFaces.size();
    faces.clear();
    edges.clear();

    
    for (int i = 0; i < newFaces.size(); i++)
        faces.push_back(newFaces[i]);
    
    
    newFaces.clear();
    
}

void ModifiedButterflySubdivision::splitEdges()
{
    std::vector<int> vertices1per8;
    std::vector<int> vertices1per16;

    for (int i = 0; i < edges.size(); i++)
    {
        Vertex vertex;
        vertex.point.x = 0.0;
        vertex.point.y = 0.0;
        vertex.point.z = 0.0;

        vertices1per8 = edges[i].get1per8Vertices();
        vertices1per16 = edges[i].get1per16Vertices();

        vertex.point.x += vertices[edges[i].startVertexIndex].point.x / (float)2;
        vertex.point.y += vertices[edges[i].startVertexIndex].point.y / (float)2;
        vertex.point.z += vertices[edges[i].startVertexIndex].point.z / (float)2;

        vertex.point.x += vertices[edges[i].endVertexIndex].point.x / (float)2;
        vertex.point.y += vertices[edges[i].endVertexIndex].point.y / (float)2;
        vertex.point.z += vertices[edges[i].endVertexIndex].point.z / (float)2;

        for (int j = 0; j < vertices1per8.size(); j++)
        {
            vertex.point.x += vertices[vertices1per8[j]].point.x / (float)8;
            vertex.point.y += vertices[vertices1per8[j]].point.y / (float)8;
            vertex.point.z += vertices[vertices1per8[j]].point.z / (float)8;
        }

        for (int j = 0; j < vertices1per16.size(); j++)
        {
            vertex.point.x -= vertices[vertices1per16[j]].point.x / (float)16;
            vertex.point.y -= vertices[vertices1per16[j]].point.y / (float)16;
            vertex.point.z -= vertices[vertices1per16[j]].point.z / (float)16;
        }

        vertex.generated = true;

        vertices.push_back(vertex);
        edges[i].generatedVertexIndex = vertices.size() - 1;

    }
}

void ModifiedButterflySubdivision::createNewFaces()
{
    // Vertex *vertex;

    for (int i = 0; i < faces.size(); i++)
    {
        //face = &faces[i];

        // Create new faces using one old vertex and two new vertices
        for (int vertexIndex = 0; vertexIndex < faces[i].verticesIndex.size(); vertexIndex++)
        {
            Face newFace;
            //vertex = faces[i].verticesIndex[vertexIndex];
            newFace.verticesIndex.push_back(faces[i].verticesIndex[vertexIndex]);
            // vertex = nullptr;

            // Find the 2 edges which contains the current old vertex, then add the generated vertices
            for (int edgeIndex = 0; edgeIndex < faces[i].edgesIndex.size(); edgeIndex++)
                if (edges[faces[i].edgesIndex[edgeIndex]].containsVertex(faces[i].verticesIndex[vertexIndex]))
                    newFace.verticesIndex.push_back(edges[faces[i].edgesIndex[edgeIndex]].generatedVertexIndex);
            
            newFace.sortVertices();
            newFaces.push_back(newFace);
        }

        // Create a new face using the 3 generated points
        Face newFace;
        for (int edgeIndex = 0; edgeIndex < faces[i].edgesIndex.size(); edgeIndex++)
            newFace.verticesIndex.push_back(edges[faces[i].edgesIndex[edgeIndex]].generatedVertexIndex);
        
        newFace.sortVertices();
        newFaces.push_back(newFace);
    }
}

void ModifiedButterflySubdivision::createEdges()
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

Edge ModifiedButterflySubdivision::createEdge(int faceIndex, int vertex1Index, int vertex2Index, bool &unique)
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

void ModifiedButterflySubdivision::setEdgeFaces()
{
    int thirdVertexIndex;
    float dotProduct;

    for (int i = 0; i < edges.size(); i++)
    {
        thirdVertexIndex = faces[edges[i].faces[0]].getThirdVertex(i);
        int startIndex, stopIndex;
        startIndex = edges[i].startVertexIndex;
        stopIndex = edges[i].endVertexIndex;
        dotProduct = (vertices[thirdVertexIndex].point.x - vertices[startIndex].point.x) * 
        (vertices[stopIndex].point.y - vertices[startIndex].point.y) * (vertices[stopIndex].point.z - vertices[startIndex].point.z) - 
                    (vertices[stopIndex].point.x - vertices[startIndex].point.x) * (vertices[thirdVertexIndex].point.y - vertices[startIndex].point.y) * (vertices[stopIndex].point.z - vertices[startIndex].point.z) - 
                    (vertices[stopIndex].point.x - vertices[startIndex].point.x) * (vertices[stopIndex].point.y - vertices[startIndex].point.y) * (vertices[thirdVertexIndex].point.z - vertices[startIndex].point.z);

        edges[i].leftFaceIndex = edges[i].faces[0];
        edges[i].rightFaceIndex = edges[i].faces[1];
    }
}

void ModifiedButterflySubdivision::setEdgeTraverses()
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

bool ModifiedButterflySubdivision::isUniqueEdge(Edge* newEdge, int &index)
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

bool ModifiedButterflySubdivision::isUniqueEdge(int vertex1, int vertex2, int &index)
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

int ModifiedButterflySubdivision::findEdge(int vertex1Index, int vertex2Index)
{
    for (int i = 0; i < edges.size(); i++)
        if (edges[i].containsVertices(vertex1Index, vertex2Index))
            return i;
    
    return -1;
}

void ModifiedButterflySubdivision::setEdgesForFaces()
{
    for (int edgeIndex = 0; edgeIndex < edges.size(); edgeIndex++)
    {
        faces[edges[edgeIndex].leftFaceIndex].edgesIndex.push_back(edgeIndex);
        faces[edges[edgeIndex].rightFaceIndex].edgesIndex.push_back(edgeIndex);
    }
}

Vertex* ModifiedButterflySubdivision::findVertex(Vertex *searched)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == *searched)
        {
            vertices[i].index = i;
            return &vertices[i];
        }
    }

    return nullptr;
}

Vertex* ModifiedButterflySubdivision::findVertex(Vertex *searched, int &index)
{
    index = 0;

    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == *searched)
        {
            vertices[i].index = i;
            return &vertices[i];
        }
        
        index++;
    }

    index = -1;
    return nullptr;
}