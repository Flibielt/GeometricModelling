#include "LoopSubdivision.hpp"

LoopSubdivision::LoopSubdivision(/* args */)
{
}

LoopSubdivision::~LoopSubdivision()
{
}

void LoopSubdivision::subdivide()
{
    std::cout << "Old vertex count: " << vertices.size() << std::endl;
    std::cout << "Old edge count: " << faces.size() << std::endl;
    std::cout << "Old face count: " << edges.size() << std::endl;

    splitEdges();

    // todo: Move the vertices to a better position

    clear();
    createEdges();

    setEdgeFaces();
    setEdgeTraverses();
    setEdgesForFaces();

    std::cout << "New vertex count: " << vertices.size() << std::endl;
    std::cout << "New edge count: " << faces.size() << std::endl;
    std::cout << "New face count: " << edges.size() << std::endl;
}

void LoopSubdivision::splitEdges()
{
    for (int i = 0; i < edges.size(); i++)
    {
        Vertex vertex;
        std::vector<int> vertices3per8;
        std::vector<int> vertices1per8;

        vertex.point.x = 0;
        vertex.point.y = 0;
        vertex.point.z = 0;
        vertex.generated = true;

        vertices3per8.push_back(edges[i].startVertexIndex);
        vertices3per8.push_back(edges[i].endVertexIndex);

        vertices1per8.push_back(edges[edges[i].leftTraversePredecessorIndex].startVertexIndex);
        vertices1per8.push_back(edges[edges[i].rightTraversePredecessorIndex].startVertexIndex);

        for (int j = 0; j < vertices3per8.size(); j++)
        {
            vertex.point.x += ((float)3 * vertices[j].point.x) / (float)8;
            vertex.point.y += ((float)3 * vertices[j].point.y) / (float)8;
            vertex.point.z += ((float)3 * vertices[j].point.z) / (float)8;
        }

        for (int j = 0; j < vertices1per8.size(); j++)
        {
            vertex.point.x += vertices[j].point.x / (float)8;
            vertex.point.y += vertices[j].point.y / (float)8;
            vertex.point.z += vertices[j].point.z / (float)8;
        }

        vertices.push_back(vertex);
        edges[i].generatedVertexIndex = vertices.size() - 1;
    }
}

void LoopSubdivision::createNewFaces()
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

void LoopSubdivision::clear()
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
