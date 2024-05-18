/* 
This is a physics 2-D simulator based on convex polygons and circles. 

Using SAT (Separating Axis Theorem) to detect collisions between polygons and circles.

I will use SFML library to draw the shapes and simulate the physics.
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "polygon.h"
#include "collision.h"
 const float PI = 3.14159265358979323846f;

std::vector<sf::Vector2f> createRegularPentagon(float sideLength,sf::Vector2f c) {
    std::vector<sf::Vector2f> vertices;
    float R = sideLength / (2 * sin(PI / 5)); // Radius of the circumscribed circle

    for (int i = 0; i < 5; ++i) {
        float theta = 2 * PI * i / 5;
        float x = R * cos(theta) + c.x;
        float y = R * sin(theta) + c.y ;
        // variate the y axis with a random number beewten 100 and 200
        //y += rand() % 200 + 200;
        //x += rand() % 200 + 200;
        vertices.push_back(sf::Vector2f(x, y));
    }
    return vertices;
}
// First star creating the main function and creating some windows and shapes

// square of 5cm x 5cm corners
std::vector<sf::Vector2f> squareCorners = {
    sf::Vector2f(300, 300),
    sf::Vector2f(400, 300),
    sf::Vector2f(400, 400),
    sf::Vector2f(300, 400)
};

// create a polygon with random values of its corner in the ranges of 200 to 500. 

sf::Vector2f center = sf::Vector2f(200, 200);
std::vector<sf::Vector2f> randomCorners = createRegularPentagon(50, center);

Polygon polygon3(randomCorners, sf::Color::Red, mat_rock);

// create a triangle
std::vector<sf::Vector2f> corners2 = {
    sf::Vector2f(300, 300),
    sf::Vector2f(400, 300),
    sf::Vector2f(350, 400)
};
Polygon polygon2(corners2, sf::Color::Green);

// create a star polygon
std::vector<sf::Vector2f> corners3 = {
    sf::Vector2f(400, 400),
    sf::Vector2f(500, 300),
    sf::Vector2f(450, 500),
    sf::Vector2f(400, 450),
    sf::Vector2f(350, 500)
};

Polygon polygon(corners3, sf::Color::Green);

//create a Mainfold with the shapes
Mainfold coll = Mainfold(&polygon, &polygon2);

const float fps = 24.0f;
const float dt = 1.0f / fps;
sf::Time dtTime = sf::seconds(dt);
sf::Time accumulatorTime = sf::seconds(0.0f);

float factor = 1.01;

void updatePositions(Mainfold&coll,sf::RenderWindow& window){
    
    Polygon *polygon = dynamic_cast<Polygon*>(coll.shapeA);
    Polygon *polygon2 = dynamic_cast<Polygon*>(coll.shapeB);
    bool cond = true;
    if (coll.isColliding) {
            polygon->setColor(sf::Color::Blue);
            sf::VertexArray normalLine(sf::LinesStrip, 2);
            normalLine[0].position = polygon->position;
            normalLine[0].color = sf::Color::Red; // Set normal line start color (black)
            normalLine[1].position = polygon->position + 1000.0f * coll.normal; // Extend normal line by 20 units
            normalLine[1].color = sf::Color::Red; // Set normal line end color (red)
            // print in the stout the normal of the collision
            window.draw(normalLine);
            cond = false;
        }else{
            polygon->setColor(sf::Color::White);
        }
        // end the current frame

        // rescale the polygon gradualy to a factor of 2.5 depending on the time, 
        //then scale it down to the original value
        if(cond) {
            //polygon.Scale(factor);
            //polygon2->Scale(factor);
            factor += 0.01;
            polygon2->Rotate(0.01);
            
            sf::Vector2f newpos = polygon3.position + sf::Vector2f(1, 1); 
            polygon3.Rotate(0.01);
            polygon3.setPosition(newpos);

            sf::Vector2f newpos2 = polygon->position + sf::Vector2f(-0.3, -1);
            polygon->setPosition(newpos2);
        }
        //polygon3.Rotate(0.01);
}


int main()
{
    // create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics 2D Simulator");

    // create a clock to keep track of the time
    sf::Clock clock;
    sf::Time frameStart = clock.getElapsedTime();

    std::cout << dtTime.asSeconds() << std::endl;
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        // get the current time
        sf::Time currentTime = clock.getElapsedTime();

        // calculate the time since the last frame
        accumulatorTime += currentTime - frameStart;
        frameStart = currentTime;

        //std::cout << accumulatorTime.asSeconds() << std::endl;
        if (accumulatorTime.asSeconds() > 0.2f){
            accumulatorTime = sf::seconds(0.2f);
        }

        while(sf::operator>=(accumulatorTime, dtTime)){
            // update the physics
            updatePositions(coll,window);
            // update the time
            accumulatorTime -= dtTime;
        }
        // clear the window with black color

        // render the polygon
        polygon.render(window);

        polygon2.render(window);

        polygon3.render(window);
        // render the normals of the polygon
        polygon.renderNormals(window);

        polygon2.renderNormals(window);

        polygon3.renderNormals(window);

        PolyvsPoly(polygon, polygon2, coll);

        window.display();
        // sf::sleep(sf::milliseconds(1000/fps));
    }

}