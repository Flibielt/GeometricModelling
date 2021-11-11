#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "data.hpp"
#include "Vertex.hpp"

/**
 * @brief Compare the vertices by angle and distance
 * 
 * @param center The center vertex
 * @param v1 The first vertex index
 * @param v2 The seconf vertex index
 * @return true The first vertex order is less than the second vertex
 * @return false The first vertex order is more than the second vertex
 */
bool comparePointOrder(Vertex *center, int vertexIndex1, int vertexIndex2);

/**
 * @brief Calculate the distance of the vertices
 * 
 * @param v1 The first vertex
 * @param v2 The second vertex
 * @return float The distance
 */
float calculateDistance(Vertex *v1, Vertex *v2);

/**
 * @brief Calculate the angle between two vertices with the transformed dot product
 * 
 * @param v1 The first vertex
 * @param v2 The second vertex
 * @return float The angle
 */
float calculateAngle(Vertex *v1, Vertex *v2);

/**
 * @brief Calculate the dot product of two vertices
 * 
 * @param v1 The first vertex
 * @param v2 The second vertex
 * @return float The dot product
 */
float calculateDotProduct(Vertex *v1, Vertex *v2);

/**
 * @brief Calculate the magnitude of a vertex
 * 
 * @param v The vertex
 * @return float The magnitude
 */
float calculateMagnitude(Vertex *v);

#endif