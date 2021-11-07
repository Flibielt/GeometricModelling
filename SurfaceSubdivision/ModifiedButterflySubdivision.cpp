#include "ModifiedButterflySubdivision.hpp"

void ModifiedButterflySubdivision::subdivide()
{
    splitEdges();
    createNewFaces();
    createEdges();
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

        vertices1per8 = edge->get1per8Vertices();
        vertices1per16 = edge->get1per16Vertices();

        for (int j = 0; j < vertices1per8.size(); j++)
        {
            vertex.point.x += (1 / 8) * vertices1per8[j]->point.x;
            vertex.point.y += (1 / 8) * vertices1per8[j]->point.y;
            vertex.point.z += (1 / 8) * vertices1per8[j]->point.z;
        }

        for (int j = 0; j < vertices1per16.size(); j++)
        {
            vertex.point.x += (1 / 16) * vertices1per8[j]->point.x;
            vertex.point.y += (1 / 16) * vertices1per8[j]->point.y;
            vertex.point.z += (1 / 16) * vertices1per8[j]->point.z;
        }

        vertices.push_back(vertex);
        edge->generatedVertex = &vertex;
    }
}

void ModifiedButterflySubdivision::createNewFaces()
{
}

void ModifiedButterflySubdivision::createEdges()
{
}
