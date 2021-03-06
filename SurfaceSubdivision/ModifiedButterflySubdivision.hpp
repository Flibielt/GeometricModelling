#ifndef MODIFIED_BUTTERFLY_SUBDIVISION_H
#define MODIFIED_BUTTERFLY_SUBDIVISION_H

#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"

class ModifiedButterflySubdivision
{
private:
    std::vector<Face> newFaces;
    std::vector<Edge> newEdges;

    /**
     * @brief Create new edges.
     * 
     * Each edge will be split to two. The new vertex by the edge is splited will be saved to the vertices.
     * 
     */
    void splitEdges();

    /**
     * @brief Create new faces using the old and new vertices.
     * 
     * 
     * From 1 old face create 4 new faces. 3 using 1 old vertex and 2 new vertices and 1 with the 3 new vertices.
     * 
     * The old faces will be deleted.
     */
    void createNewFaces();

    /**
     * @brief Clear the old faces and edges
     * 
     */
    void clear();
public:
    ModifiedButterflySubdivision(/* args */);
    ~ModifiedButterflySubdivision();

    void subdivide();
};

#endif
