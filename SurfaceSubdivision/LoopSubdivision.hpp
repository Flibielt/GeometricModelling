#ifndef LOOP_SUBDIVISION
#define LOOP_SUBDIVISION

#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "data.hpp"

class LoopSubdivision
{
private:
    std::vector<Edge> newEdges;
    std::vector<Face> newFaces;

    void splitEdges();
    void createNewFaces();
    void clear();
public:
    LoopSubdivision(/* args */);
    ~LoopSubdivision();

    void subdivide();
};

#endif