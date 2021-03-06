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
    for (int i = 0; i < faces.size(); i++)
    {

        // Create new faces using one old vertex and two new vertices
        for (int vertexIndex = 0; vertexIndex < faces[i].verticesIndex.size(); vertexIndex++)
        {
            Face newFace;
            newFace.verticesIndex.push_back(faces[i].verticesIndex[vertexIndex]);

            // Find the 2 edges which contains the current old vertex, then add the generated vertices
            for (int edgeIndex = 0; edgeIndex < faces[i].edgesIndex.size(); edgeIndex++)
                if (edges[faces[i].edgesIndex[edgeIndex]].containsVertex(faces[i].verticesIndex[vertexIndex]))
                    newFace.verticesIndex.push_back(edges[faces[i].edgesIndex[edgeIndex]].generatedVertexIndex);
            
            // newFace.sortVertices();
            newFaces.push_back(newFace);
        }

        // Create a new face using the 3 generated points
        Face newFace;
        for (int edgeIndex = 0; edgeIndex < faces[i].edgesIndex.size(); edgeIndex++)
            newFace.verticesIndex.push_back(edges[faces[i].edgesIndex[edgeIndex]].generatedVertexIndex);
        
        // newFace.sortVertices();
        newFaces.push_back(newFace);
    }
}
