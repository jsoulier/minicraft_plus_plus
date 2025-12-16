#include "physics.hpp"

bool MppPhysicsTest(float x1, float y1, int s1, float x2, float y2, int s2)
{
    return (x1 + s1 > x2) && (x1 < x2 + s2) && (y1 + s1 > y2) && (y1 < y2 + s2);
}