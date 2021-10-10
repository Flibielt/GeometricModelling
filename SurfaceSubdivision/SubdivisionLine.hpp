#ifndef SUBDIVISION_LINE_H
#define SUBDIVISION_LINE_H

#include "glm/glm.hpp"
#include "SubdivisionSurface.hpp"

class SubdivisionLine
{
public:
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

    SubdivisionLine();
    SubdivisionLine(glm::vec3* pStartPoint, glm::vec3* pEndPoint, 
        SubdivisionSurface* pLeftSubdivisionSurface, SubdivisionSurface* pRightSubdivisionSurface,
        glm::vec3* pLeftTraversePred, glm::vec3* pLeftTraverseRight,
        glm::vec3* pRightTraversePred, glm::vec3* pRightTraverseSucc);
    ~SubdivisionLine();
};

#endif