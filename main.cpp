/* 
This is a physics 2-D simulator based on convex polygons and circles. 

Using SAT (Separating Axis Theorem) to detect collisions between polygons and circles.

I will use SFML library to draw the shapes and simulate the physics.
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "polygon.h"
#include "collision.h"
 
// First star creating the main function and creating some windows and shapes
int main()
{
    // create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics 2D Simulator");

    // create a pentagon
    std::vector<sf::Vector2f> corners = {
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 100),
        sf::Vector2f(250, 150),
        sf::Vector2f(200, 200),
        sf::Vector2f(100, 200)
    };
    Polygon polygon(corners);

    // create a triangle
    std::vector<sf::Vector2f> corners2 = {
        sf::Vector2f(300, 300),
        sf::Vector2f(400, 300),
        sf::Vector2f(350, 400)
    };
    Polygon polygon2(corners2, sf::Color::Green);

    // see if the polygons are colliding
    //create a Mainfold with the shapes
    Mainfold coll = Mainfold(&polygon, &polygon2);
    PolyvsPoly(polygon, polygon2, coll);
    
    // print the polygon corners of the polygon and normals in the stdout
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        // render the polygon
        polygon.render(window);

        polygon2.render(window);

        // render the normals of the polygon
        polygon.renderNormals(window);

        polygon2.renderNormals(window);

        PolyvsPoly(polygon, polygon2, coll);

        bool cond = true;
        //render the normal of the collition if there is one

        window.display();
        
        if (coll.isColliding) {
            polygon.setColor(sf::Color::Blue);
            sf::VertexArray normalLine(sf::LinesStrip, 2);
            normalLine[0].position = polygon.mCenter;
            normalLine[0].color = sf::Color::Red; // Set normal line start color (black)
            normalLine[1].position = polygon.mCenter + 1000.0f * coll.normal; // Extend normal line by 20 units
            normalLine[1].color = sf::Color::Red; // Set normal line end color (red)
            // print in the stout the normal of the collision
            window.draw(normalLine);
            cond = false;
        }else{
            polygon.setColor(sf::Color::White);
        }
        
        // end the current frame

        // rescale the polygon gradualy to a factor of 2.5 depending on the time, 
        //then scale it down to the original value
        if(cond) {
            float factor = 1.01;
            //polygon.Scale(factor);
            polygon2.Scale(factor);

            polygon2.render(window);
            factor += 0.01;

            //polygon2.Translate(sf::Vector2f(0, -1));
            polygon.Translate(sf::Vector2f(0.3, 1));
        }

        // wait 1 second for the next loop
        sf::sleep(sf::seconds(0.2f));   
    }

}