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
    splitEdges();

    for (int i = 0; i < edges.size(); i++)
        for (int j = 0; j < faces.size(); j++)
            for (int x = 0; x < faces[j].edges.size(); x++)
                if (*faces[j].edges[x] == edges[i])
                {
                    faces[j].edges[x] = &edges[i];
                    faces[j].edges[x]->pStartPoint = edges[i].pStartPoint;
                    faces[j].edges[x]->pEndPoint = edges[i].pEndPoint;
                    faces[j].edges[x]->generatedVertex = edges[i].generatedVertex;
                }
    
    createNewFaces();

    int oldEdgeCount, newEdgeCount;
    oldEdgeCount = edges.size();

    clear();
    
    
    createEdges();

    setEdgeFaces();
    setEdgeTraverses();
    setEdgesForFaces();

    newEdgeCount = edges.size();
    std::cout << "Old edge count: " << oldEdgeCount << std::endl << "New edge count: " << newEdgeCount << std::endl;
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
    Edge *edge = nullptr;
    std::vector<Vertex*> vertices1per8;
    std::vector<Vertex*> vertices1per16;

    for (int i = 0; i < edges.size(); i++)
    {
        edge = &edges[i];
        Vertex vertex;
        vertex.point.x = 0.0;
        vertex.point.y = 0.0;
        vertex.point.z = 0.0;

        vertices1per8 = edge->get1per8Vertices();
        vertices1per16 = edge->get1per16Vertices();

        vertex.point.x += edge->pStartPoint->point.x / (float)2;
        vertex.point.y += edge->pStartPoint->point.y / (float)2;
        vertex.point.z += edge->pStartPoint->point.z / (float)2;

        vertex.point.x += edge->pEndPoint->point.x / (float)2;
        vertex.point.y += edge->pEndPoint->point.y / (float)2;
        vertex.point.z += edge->pEndPoint->point.z / (float)2;

        for (int j = 0; j < vertices1per8.size(); j++)
        {
            vertex.point.x += vertices1per8[j]->point.x / (float)8;
            vertex.point.y += vertices1per8[j]->point.y / (float)8;
            vertex.point.z += vertices1per8[j]->point.z / (float)8;
        }

        for (int j = 0; j < vertices1per16.size(); j++)
        {
            vertex.point.x -= vertices1per16[j]->point.x / (float)16;
            vertex.point.y -= vertices1per16[j]->point.y / (float)16;
            vertex.point.z -= vertices1per16[j]->point.z / (float)16;
        }

        vertex.generated = true;

        vertices.push_back(vertex);
        edge->generatedVertex = &vertices[vertices.size() - 1];
        edge->generatedVertexIndex = vertices.size() - 1;

        int index;
        findVertex(edge->pStartPoint, index);
        edge->startPointIndex = index;

        findVertex(edge->pEndPoint, index);
        edge->endPointIndex = index;
    }
}

void ModifiedButterflySubdivision::createNewFaces()
{
    Vertex *vertex;

    for (int i = 0; i < faces.size(); i++)
    {
        //face = &faces[i];

        // Create new faces using one old vertex and two new vertices
        for (int vertexIndex = 0; vertexIndex < faces[i].vertices.size(); vertexIndex++)
        {
            Face newFace;
            vertex = faces[i].vertices[vertexIndex];
            newFace.vertices.push_back(findVertex(vertex));
            vertex = nullptr;

            // Find the 2 edges which contains the current old vertex, then add the generated vertices
            for (int edgeIndex = 0; edgeIndex < faces[i].edges.size(); edgeIndex++)
                if (faces[i].edges[edgeIndex]->containsVertex(faces[i].vertices[vertexIndex]))
                {
                    //vertex = face->edges[edgeIndex]->generatedVertex;
                    vertex = &vertices[faces[i].edges[edgeIndex]->generatedVertexIndex];
                    newFace.vertices.push_back(findVertex(vertex));

                    vertex = nullptr;
                }
            
            newFace.sortVertices();
            newFaces.push_back(newFace);
        }

        // Create a new face using the 3 generated points
        Face newFace;
        for (int edgeIndex = 0; edgeIndex < faces[i].edges.size(); edgeIndex++)
        {
            //vertex = faces[i].edges[edgeIndex]->generatedVertex,
            vertex = &vertices[faces[i].edges[edgeIndex]->generatedVertexIndex];
            newFace.vertices.push_back(findVertex(vertex));
        }
        newFace.sortVertices();
        newFaces.push_back(newFace);
    }
}

void ModifiedButterflySubdivision::createEdges()
{
    int index;
    bool unique;

    for (int i = 0; i < faces.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Edge edge = createEdge(faces[i], faces[i].vertices[j], faces[i].vertices[(j + 1) % 3], unique);

            if (unique)
                edges.push_back(edge);
        }
    }
}

Edge ModifiedButterflySubdivision::createEdge(Face &face, Vertex *vertex1, Vertex *vertex2, bool &unique)
{
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
        edges[index].faces.push_back(&face);
    }

    return edge;
}

void ModifiedButterflySubdivision::setEdgeFaces()
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

        edges[i].leftFace = edges[i].faces[0];
        edges[i].rightFace = edges[i].faces[1];
    }
}

void ModifiedButterflySubdivision::setEdgeTraverses()
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

Edge* ModifiedButterflySubdivision::findEdge(Vertex *v1, Vertex *v2)
{
    Edge *foundEdge;
    for (int i = 0; i < edges.size(); i++)
        if (edges[i].containsVertices(v1, v2))
            foundEdge = &edges[i];
    
    return foundEdge;
}

void ModifiedButterflySubdivision::setEdgesForFaces()
{
    Edge *edge;
    for (int edgeIndex = 0; edgeIndex < edges.size(); edgeIndex++)
    {
        edge = &edges[edgeIndex];

        edge->leftFace->edges.push_back(edge);
        edge->rightFace->edges.push_back(edge);
    }
}

Vertex* ModifiedButterflySubdivision::findVertex(Vertex *searched)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == *searched)
            return &vertices[i];
    }

    return nullptr;
}

Vertex* ModifiedButterflySubdivision::findVertex(Vertex *searched, int &index)
{
    index = 0;

    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] == *searched)
            return &vertices[i];
        
        index++;
    }

    index = -1;
    return nullptr;
}