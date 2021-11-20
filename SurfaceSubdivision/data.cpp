#include "data.hpp"

void createEdges()
{
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

Edge createEdge(int faceIndex, int vertex1Index, int vertex2Index, bool &unique)
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

void setEdgeFaces()
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

void setEdgeTraverses()
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

bool isUniqueEdge(Edge* newEdge, int &index)
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

bool isUniqueEdge(int vertex1, int vertex2, int &index)
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

int findEdge(int vertex1Index, int vertex2Index)
{
    for (int i = 0; i < edges.size(); i++)
        if (edges[i].containsVertices(vertex1Index, vertex2Index))
            return i;
    
    return -1;
}

void setEdgesForFaces()
{
    for (int edgeIndex = 0; edgeIndex < edges.size(); edgeIndex++)
    {
        faces[edges[edgeIndex].leftFaceIndex].edgesIndex.push_back(edgeIndex);
        faces[edges[edgeIndex].rightFaceIndex].edgesIndex.push_back(edgeIndex);
    }
}

void createObjFile()
{
    std::string fileName = "iteratoin_" + std::to_string(iteration) + ".obj";
    std::ofstream objFile(fileName);
    objFile.clear();

    std::cout << "Write to: iteration_" << std::to_string(iteration) << ".obj" << std::endl;

    for (int i = 0; i < vertices.size(); i++)
        objFile << "v " << vertices[i].point.x << " " << vertices[i].point.y << " "  << vertices[i].point.z << std::endl;

    for (int i = 0; i < faces.size(); i++)
        objFile << "f " << faces[i].verticesIndex[0] + 1 << " " << faces[i].verticesIndex[1] + 1 << " "  
            << faces[i].verticesIndex[2] + 1 << std::endl;

    objFile.close();
    std::cout << "Finished write" << std::endl;
}
