/*  
The base class for all shapes. It contains the shared 
properties and data that all shapes have in common to perform the pyhsics calculations. 


*/

#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

struct Material {
    float density;
    float restitution;
    //float friction;
};
// Define some common materials as constants of the file
const Material mat_rock = {0.6f, 0.1f};
const Material mat_wood = {0.3f, 0.2f};
const Material mat_metal = {1.2f, 0.05f};
const Material mat_bouncyBall = {0.3f, 0.8f};
const Material mat_superBall = {0.3f, 0.95f};
const Material mat_pillow = {0.1f, 0.2f};
const Material mat_static = {0.0f, 0.4f};

class Shape {
    // virtual funciton that returns the type of the shape
    public:
    enum type {
        sCircle,
        sPolygon
    };

    sf::Vector2f position; // center
    sf::Vector2f velocity;

    float rotation; // radians
    float angularVelocity; // velocity of rotation

    sf::Vector2f force; // accomulated forces
    float torque; // torque applied to the shape

    Material material; // defines density and restitution
    
    // mass data, sett by shape according to the material
    float mass;
    float inv_mass;
    float inertia;
    float inv_inertia;

    // color of the polygon
    sf::Color mColor;


    virtual ~Shape() = default;

    // Scale the shape
    virtual void Scale(float factor) = 0;

    // Rotate the shape
    virtual void Rotate(float angle) = 0;

    // Set position of the shape
    virtual void setPosition(sf::Vector2f _position) = 0;

    virtual void render(sf::RenderWindow& window) = 0;

    virtual void computeMass() = 0;

    // change color
    virtual void setColor(sf::Color _color) = 0;

    virtual type getType() = 0;
};


#endif