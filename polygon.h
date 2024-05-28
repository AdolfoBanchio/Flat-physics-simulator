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
            sf::Color _color = sf::Color::White,
            Material _material = mat_rock,
            float _rotation = 0.0f);
    
    // Destructor (optional, compiler usually generates default)
    ~Polygon() = default;   
    
    // Scale the polygon
    void Scale(float factor);

    // Rotate the polygon
    void Rotate(float angle);

    // Set position of the polygon
    void setPosition(sf::Vector2f _position);

    void computeMass();

    void render(sf::RenderWindow& window);
    
    void renderNormals(sf::RenderWindow& window);

    void setColor(sf::Color _color);
    // Get the type of the shape from the enum class ShapeType
    type getType() override;

private:
    // Function to calculate normals 
    void calculateNormals();
};

#endif