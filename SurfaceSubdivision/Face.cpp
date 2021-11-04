#include "Face.hpp"


Face::Face(/* args */)
{
}

Face::~Face()
{
}

bool Face::isEdgeInFace(Edge* edge)
{
    bool startFound = false, endFound = false;
    
    for (Vertex* Vertex : vertices)
    {
        if (Vertex->point == edge->pStartPoint->point)
            startFound = true;
        else if (Vertex->point == edge->pEndPoint->point)
            endFound = true;
        
        if (startFound && endFound)
            break;
    }

    return startFound && endFound;
}
