#include "polygon.h"
#include <cmath>

struct Mainfold
{
    bool isColliding = false;
    float penetration = 0;
    sf::Vector2f normal = sf::Vector2f(0.0f, 0.0f);
    Shape *shapeA;
    Shape *shapeB;

    Mainfold(Shape *a, Shape *b) : shapeA(a), shapeB(b) {}
};

void PolyvsPoly(Polygon &polyA, Polygon &polyB, Mainfold &mainfold);

