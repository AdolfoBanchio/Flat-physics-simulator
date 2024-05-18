/* 
This is the definition of the Polygon struct.
 */

#include "polygon.h"
#include <cmath>

Polygon::Polygon(const std::vector<sf::Vector2f>& _corners, 
                sf::Color _color,
                Material _material,
                float _rotation){
  mCorners = _corners;
  mColor = _color;
  material = _material;

  velocity = sf::Vector2f(0.0f, 0.0f);
  angularVelocity = 0.0f;
  force = sf::Vector2f(0.0f, 0.0f);
  torque = 0.0f;

  rotation = _rotation;

  computeMass();
  // calculate the center of the polygon using the corners
  calculateNormals(); // Call the private function to calculate normals
}

/* 
It calculates the centroid of the polygon according to the coordinates of the corners.

Also calculates the mass of the polygon according to the density of the material.
and the inertia of the polygon.
 */
void Polygon::computeMass() {
  position = sf::Vector2f(0, 0);
  float signedArea = 0.0f;
  inertia = 0.0f;

  float Ix = 0.0f;
  float Iy = 0.0f;
  // calculate the signed area, center and inertia
  for (size_t i = 0; i < mCorners.size(); ++i) {
    sf::Vector2f current = mCorners[i];
    sf::Vector2f next = mCorners[(i + 1) % mCorners.size()];
 
    float crossProduct = current.x * next.y - next.x * current.y;
    // print the vectors and the cross product
    /* std::cout << "Current: " << current.x << " " << current.y << "\n";
    std::cout << "Next: " << next.x << " " << next.y << "\n";
    std::cout << "Cross Product: " << crossProduct << "\n"; */
    signedArea += crossProduct;
    position += (current + next) * crossProduct;

    float x2 = current.x * current.x + next.x * current.x + next.x * next.x;
    float y2 = current.y * current.y + next.y * current.y + next.y * next.y;
    Ix += x2 * crossProduct;
    Iy += y2 * crossProduct;
  }

  signedArea = signedArea*0.5f;
  position /= (6.0f * signedArea);

  std::cout << "Position: " << position.x << " " << position.y << "\n";
  // calculate the mass
  mass = material.density * std::abs(signedArea);
  inv_mass = (mass) ? 1.0f / mass : 0.0f;

  // calculate the inertia
  Ix /= 12.0f;
  Iy /= 12.0f;
  
  inertia =  std::abs(Ix + Iy) ;//- mass * (position.x * position.x + position.y * position.y); substracting m * d^2 moves the inertia momentum to the center.
  inv_inertia = (inertia) ? 1.0f / inertia : 0.0f;

  std::cout << "Area: " << signedArea << "\n";
  std::cout << "Mass: " << mass << "\n";
  std::cout << "Inertia: " << inertia << "\n";
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

void Polygon::Scale(float factor) {
  for (size_t i = 0; i < mCorners.size(); ++i) {
    mCorners[i] = position + factor * (mCorners[i] - position);
  }
}

void Polygon::Rotate(float angle) {
  float s = std::sin(angle);
  float c = std::cos(angle);
  rotation += angle;
  for (size_t i = 0; i < mCorners.size(); ++i) {
    sf::Vector2f p = mCorners[i] - position;
    mCorners[i].x = p.x * c - p.y * s + position.x;
    mCorners[i].y = p.x * s + p.y * c + position.y;
  }
  calculateNormals();
}

void Polygon::setPosition(sf::Vector2f pos) {
  sf::Vector2f translation = pos - position;
  for (size_t i = 0; i < mCorners.size(); ++i) {
    mCorners[i] += translation;
  }
  position = pos;
}

void Polygon::setColor(sf::Color _color) {
  mColor = _color;
}


Shape::type Polygon::getType() {
  return sPolygon;
}

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