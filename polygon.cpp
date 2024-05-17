/* 
This is the definition of the Polygon struct.
 */

#include "polygon.h"
#include <cmath>

Polygon::Polygon(const std::vector<sf::Vector2f>& _corners, sf::Color _color) {
  mCorners = _corners;
  mColor = _color;
  // calculate the center of the polygon using the corners
  mCenter = sf::Vector2f(0.0f, 0.0f);
  for (size_t i = 0; i < mCorners.size(); ++i) {
    mCenter += mCorners[i];
  }
  mCenter /= static_cast<float>(mCorners.size());

  calculateNormals(); // Call the private function to calculate normals
}

void Polygon::calculateNormals() {
  mNormals.reserve(mCorners.size()); // Pre-allocate space

  for (size_t i = 0; i < mCorners.size(); ++i) {
    // Use size_t for array/vector indices (unsigned)
    sf::Vector2f edge = mCorners[i] - mCorners[(i + mCorners.size() - 1) % mCorners.size()];
    sf::Vector2f perp(edge.y, -edge.x);

    // Normalize the perp vector
    float length = std::sqrt(std::abs(perp.x * perp.x) + std::abs(perp.y * perp.y));
    if (length > 0.0f) { // Avoid division by zero
      perp.x /= length;
      perp.y /= length;
    }

    mNormals.push_back(perp);
  }
}

/* 
Modidy so the render function renders the polygon
using the center (position) as the reference point. 

Because all the pyhsics calculations modify the position of the polygon. 

Maybe i can save the prevouis position and the actual. So the pyhsics calculations
modify the actual position. And when i render i calculate the transaltion made 
from old_position to actuil_position. And that same translation is applied to the corners.
 */
void Polygon::render(sf::RenderWindow& window) {
  sf::ConvexShape shape;
  shape.setPointCount(mCorners.size());
  for (size_t i = 0; i < mCorners.size(); ++i) {
    shape.setPoint(i, mCorners[i]);
  }
  shape.setFillColor(mColor);
  window.draw(shape);
}

void Polygon::renderNormals(sf::RenderWindow& window) {
  for (size_t i = 0; i < mCorners.size(); ++i) {
    sf::VertexArray normalLine(sf::LinesStrip, 2);
    normalLine[0].position = mCorners[i];
    normalLine[0].color = sf::Color::Red; // Set normal line start color (black)
    normalLine[1].position = mCorners[i] + 20.0f * mNormals[i]; // Extend normal line by 20 units
    normalLine[1].color = sf::Color::Red; // Set normal line end color (red)
    window.draw(normalLine);
  }
}

void Polygon::Translate(sf::Vector2f tvector){
  mCenter += tvector;
  for (size_t i = 0; i < mCorners.size(); ++i) {
    mCorners[i] += tvector;
  }
}

void Polygon::setColor(sf::Color _color) {
  mColor = _color;
}

void Polygon::Scale(float factor) {
  for (size_t i = 0; i < mCorners.size(); ++i) {
    mCorners[i] = mCenter + factor * (mCorners[i] - mCenter);
  }
}

Shape::type Polygon::getType() {
  return sPolygon;
}