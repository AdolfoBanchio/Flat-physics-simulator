/* 
This is the definition of the Polygon class 
that extends form Shape.

Shape --> Polygon

for now it just stores:
    - corners
    - center
    - normals of each corner

 */

#ifndef POLYGON_H
#define POLYGON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "shape.h"

class Polygon : public Shape{
public: 
    // corners of the polygon
    std::vector<sf::Vector2f> mCorners;

    // normals of the polygon
    std::vector<sf::Vector2f> mNormals;
    
    // Constructor
    Polygon(const std::vector<sf::Vector2f>& _corners, 
            sf::Color _color = sf::Color::White);
    
    // Destructor (optional, compiler usually generates default)
    ~Polygon() = default;   
    
    // Translate the polygon
    void Translate(sf::Vector2f tvector);
    
    // Scale the polygon
    void Scale(float factor);

    // override the render function of shape
    void render(sf::RenderWindow& window);
    
    // change color
    void setColor(sf::Color _color);

    // Render the normals of the polygon
    void renderNormals(sf::RenderWindow& window);

    // Get the type of the shape from the enum class ShapeType
    type getType() override;

private:
    // Function to calculate normals 
    void calculateNormals();
};

#endif