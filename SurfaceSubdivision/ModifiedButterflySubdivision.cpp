#include "ModifiedButterflySubdivision.hpp"

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
                    faces[j].edges[x] = &edges[i];
    
    createNewFaces();

    clear();

    createEdges();
}

void ModifiedButterflySubdivision::clear()
{
    faces.clear();
    edges.clear();

    faces = newFaces;
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

        for (int j = 0; j < vertices1per8.size(); j++)
        {
            vertex.point.x += vertices1per8[j]->point.x / 8;
            vertex.point.y += vertices1per8[j]->point.y / 8;
            vertex.point.z += vertices1per8[j]->point.z / 8;
        }

        for (int j = 0; j < vertices1per16.size(); j++)
        {
            vertex.point.x += vertices1per16[j]->point.x / 16;
            vertex.point.y += vertices1per16[j]->point.y / 16;
            vertex.point.z += vertices1per16[j]->point.z / 16;
        }

        vertices.push_back(vertex);
        edge->generatedVertex = &vertex;
    }
}

void ModifiedButterflySubdivision::createNewFaces()
{
    Face *face = nullptr;

    for (int i = 0; i < faces.size(); i++)
    {
        face = &faces[i];

        // Create new faces using one old vertex and two new vertices
        for (int vertexIndex = 0; vertexIndex < face->vertices.size(); vertexIndex++)
        {
            Face newFace;
            newFace.vertices.push_back(face->vertices[vertexIndex]);

            // Find the 2 edges which contains the current old vertex, then add the generated vertices
            for (int edgeIndex = 0; edgeIndex < face->edges.size(); edgeIndex++)
                if (face->edges[edgeIndex]->containsVertex(face->vertices[vertexIndex]))
                    newFace.vertices.push_back(face->edges[edgeIndex]->generatedVertex);
            
            newFace.sortVertices();
            newFaces.push_back(newFace);
        }
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
            Edge edge = createEdge(faces[faceIndex], faces[faceIndex].vertices[i], faces[faceIndex].vertices[(i + 1) % 3], unique);
            
            if (unique)
                edges.push_back(edge);
            
        }
    }
}

Edge ModifiedButterflySubdivision::createEdge(Face &face, Vertex *vertex1, Vertex *vertex2, bool &unique)
{
    std::vector<Edge>::iterator it;
    int index = 0;

    Edge edge;
    edge.pStartPoint = vertex1;
    edge.pEndPoint = vertex2;

    if (isUniqueEdge(&edge, index))
    {
        edge.faces.push_back(&face);
        face.edges.push_back(&edge);
        unique = true;
    }
    else
    {
        // Search the existing edge and add the face
        unique = false;
        it = edges.begin();
        std::advance(it, index);
        (*it).faces.push_back(&face);
        face.edges.push_back(&(*it));
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