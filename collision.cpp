#include "polygon.h"
#include <cmath>
#include "collision.h"

/* 
Polygon vs Polygon collision detection
Based on the SAT algorithm.
/*  
We need to be able to knwo which are the min and max projections of a polygon on a given axis.

define a private function called FindMinMaxProjection:
Takes a polygond and a axis as input and returns a 
MinMax struct with the min and max projections of the polygon on the axis.
*/
struct MinMax {
    float min;
    float max;
};

MinMax FindMinMaxProjection(const Polygon& polygon, const sf::Vector2f& axis) {
    // The projection its the dot product between the polygon corners and the axis
    float projection = polygon.mCorners[0].x * axis.x + polygon.mCorners[0].y * axis.y;
    MinMax minMax = { projection, projection };

    for (size_t i = 1; i < polygon.mCorners.size(); ++i) {
        projection = polygon.mCorners[i].x * axis.x + polygon.mCorners[i].y * axis.y;

        if (projection < minMax.min) {
            minMax.min = projection;
        }
        if (projection > minMax.max) {
            minMax.max = projection;
        }
    }
    return minMax;
}

/* 
Now that i have a way to find the min and max projections of a polygon on a given axis,
i can define the SAT algorithm to detect collisions between two polygons.

Basicly, i need to check if the projections of the polygons on each normal of the polygons overlap.

I will return a 3-uple with
    - a boolean that indicates if the polygons are colliding
    - the penetration depth
    - the normal of the collision
 */

void PolyvsPoly(Polygon & polyA, Polygon &polyB, Mainfold &mainfold){
    bool isColliding = true;
    float penetration = INFINITY;
    sf::Vector2f normal;
    // for the normals of polyA find the minmax projection of A and B, see if they overlap
    for (size_t i = 0; i < polyA.mNormals.size(); ++i) {
        sf::Vector2f axis = polyA.mNormals[i];
        MinMax minMaxA = FindMinMaxProjection(polyA, axis);
        MinMax minMaxB = FindMinMaxProjection(polyB, axis);

        if (minMaxA.min > minMaxB.max || minMaxB.min > minMaxA.max) {
            isColliding = false;
            break;
        }
        else {
            float overlapA = minMaxA.max - minMaxB.min;
            float overlapB = minMaxB.max - minMaxA.min;
            if (overlapA < overlapB) {
                penetration = overlapA;
                normal = axis;
            }
            else {
                penetration = overlapB;
                normal = -axis;
            }
        }
    }

    // for the normals of polyB find the minmax projection of A and B, see if they overlap
    for (size_t i = 0; i < polyB.mNormals.size(); ++i) {
        sf::Vector2f axis = polyB.mNormals[i];
        MinMax minMaxA = FindMinMaxProjection(polyA, axis);
        MinMax minMaxB = FindMinMaxProjection(polyB, axis);

        if (minMaxA.min > minMaxB.max || minMaxB.min > minMaxA.max) {
            isColliding = false;
            break;
        }
        else {
            float overlapA = minMaxA.max - minMaxB.min;
            float overlapB = minMaxB.max - minMaxA.min;
            if (overlapA < overlapB) {
                penetration = overlapA;
                normal = axis;
            }
            else {
                penetration = overlapB;
                normal = -axis;
            }
        }
    }
    // save the values for the mainfold
    mainfold.isColliding = isColliding;
    mainfold.penetration = penetration;
    mainfold.normal = normal;
}