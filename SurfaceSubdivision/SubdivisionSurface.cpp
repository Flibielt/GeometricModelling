#include "SubdivisionSurface.hpp"

SubdivisionSurface::SubdivisionSurface()
{
}

SubdivisionSurface::SubdivisionSurface(glm::vec3* pPoint1, glm::vec3* pPoint2, glm::vec3* pPoint3)
{
    points.push_back(pPoint1);
    points.push_back(pPoint2);
    points.push_back(pPoint3);
}

SubdivisionSurface::~SubdivisionSurface()
{
    points.clear();
    points.shrink_to_fit();
}
