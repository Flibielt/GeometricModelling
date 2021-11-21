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

    // Approximate the vertices' position
    approximate();

    // Create new vertices by splitting the edges
    splitEdges();

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

void LoopSubdivision::approximate()
{
    for (int vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++)
    {
        Vertex s, newValue;
        int n = 0;
        float alpha = 0.0, beta = 0.0;
        
        s.point.x = 0;
        s.point.y = 0;
        s.point.z = 0;

        for (int edgeIndex = 0; edgeIndex < edges.size(); edgeIndex++)
        {
            int tmpVertexIndex = 0;
            if (edges[edgeIndex].containsVertex(vertexIndex)) 
            {
                tmpVertexIndex = edges[edgeIndex].getOtherVertexIndex(vertexIndex);

                s.point.x += vertices[tmpVertexIndex].point.x;
                s.point.y += vertices[tmpVertexIndex].point.y;
                s.point.z += vertices[tmpVertexIndex].point.z;

                n++;
            }
        }

        beta = pow((float)3 / (float)8 + cos(2 * M_PI / (float)n), 2);
        alpha = ((float)5 / (float)8 - beta) / (float)n;

        newValue.point.x = (1 - n * alpha) * vertices[vertexIndex].point.x + alpha * s.point.x;
        newValue.point.y = (1 - n * alpha) * vertices[vertexIndex].point.y + alpha * s.point.y;
        newValue.point.z = (1 - n * alpha) * vertices[vertexIndex].point.z + alpha * s.point.z;

        vertices[vertexIndex].point.x = newValue.point.x;
        vertices[vertexIndex].point.y = newValue.point.y;
        vertices[vertexIndex].point.z = newValue.point.z;
    }
}
