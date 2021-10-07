#ifndef SUBDIVISION_LINE_H
#define SUBDIVISION_LINE_H

#include "glm/glm.hpp"
#include "SubdivisionSurface.h"

class SubdivisionLine
{
private:
    glm::vec3* pStartPoint;
    glm::vec3* pEndPoint;

    // Neighbouring surfaces
    SubdivisionSurface* pLeftSubdivisionSurface;
    SubdivisionSurface* pRightSubdivisionSurface;
    
    // Left traverse
    glm::vec3* pLeftTraversePred;
    glm::vec3* pLeftTraverseSucc;

    // Right traverse
    glm::vec3* pRightTraversePred;
    glm::vec3* pRightTraverseSucc;
public:
    SubdivisionLine(/* args */);
    ~SubdivisionLine();

};

#endif