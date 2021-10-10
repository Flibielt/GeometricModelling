#include "SubdivisionLine.hpp"

SubdivisionLine::SubdivisionLine()
{
}

SubdivisionLine::SubdivisionLine(glm::vec3* pStartPoint, glm::vec3* pEndPoint, 
        SubdivisionSurface* pLeftSubdivisionSurface, SubdivisionSurface* pRightSubdivisionSurface,
        glm::vec3* pLeftTraversePred, glm::vec3* pLeftTraverseRight,
        glm::vec3* pRightTraversePred, glm::vec3* pRightTraverseSucc)
{
    this->pStartPoint = pStartPoint;
    this->pEndPoint = pEndPoint;

    this->pLeftSubdivisionSurface = pLeftSubdivisionSurface;
    this->pRightSubdivisionSurface = pRightSubdivisionSurface;

    this->pLeftTraversePred = pLeftTraversePred;
    this->pLeftTraverseSucc = pLeftTraverseSucc;

    this->pRightTraversePred = pRightTraversePred;
    this->pRightTraverseSucc = pRightTraverseSucc;
}

SubdivisionLine::~SubdivisionLine()
{
}
