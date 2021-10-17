#ifndef SUBDIVISION_SURFACE_H
#define SUBDIVISION_SURFACE_H

#include "glm/glm.hpp"
#include <vector>

class SubdivisionSurface
{
public:
    std::vector<glm::vec3> points;

    SubdivisionSurface();
    SubdivisionSurface(glm::vec3 pPoint1, glm::vec3 pPoint2, glm::vec3 pPoint3);
    ~SubdivisionSurface();
};


#endif