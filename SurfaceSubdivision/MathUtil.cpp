#include "MathUtil.hpp"

bool comparePointOrder(Vertex *center, Vertex *v1, Vertex *v2)
{
    float angle1, angle2;
    float distance1, distance2;

    angle1 = calculateAngle(center, v1);
    angle2 = calculateAngle(center, v2);

    if (angle1 < angle2)
        return true;
    
    distance1 = calculateDistance(center, v1);
    distance2 = calculateDistance(center, v2);

    return angle1 == angle2 && distance1 < distance2;
}

float calculateDistance(Vertex *v1, Vertex *v2)
{
    return sqrt(v1->point.x * v2->point.x + v1->point.y * v2->point.y + v1->point.z * v2->point.z);
}

float calculateDotProduct(Vertex *v1, Vertex *v2)
{
    return (v1->point.x * v2->point.x) + (v1->point.y * v2->point.y) + (v1->point.z * v2->point.z);
}

float calculateMagnitude(Vertex *v)
{
    return sqrt(pow(v->point.x, 2) + pow(v->point.y, 2) + pow(v->point.z, 2));
}

float calculateAngle(Vertex *v1, Vertex *v2)
{
    // Transformed dot product
    return acos(calculateDotProduct(v1, v2) / (calculateMagnitude(v1) * calculateMagnitude(v2)));
}
