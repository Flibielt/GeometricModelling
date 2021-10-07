#ifndef SubdivisionLine_H
#define SubdivisionLine_H

#include "glm/glm.hpp"
#include "SubdivisionSurface.h"

class SubdivisionLine
{
private:
    glm::vec3* start_point;
    glm::vec3* end_point;

    // Neighbouring surfaces
    SubdivisionSurface* subdivision_surface_left;
    SubdivisionSurface* subdivision_surface_right;
    
    // Left traverse
    glm::vec3* left_traverse_pred;
    glm::vec3* left_traverse_succ;

    // Right traverse
    glm::vec3* right_traverse_pred;
    glm::vec3* right_traverse_succ;
public:
    SubdivisionLine(/* args */);
    ~SubdivisionLine();
};

SubdivisionLine::SubdivisionLine(/* args */)
{
}

SubdivisionLine::~SubdivisionLine()
{
}


#endif