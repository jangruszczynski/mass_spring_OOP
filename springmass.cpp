/** file: springmass.cpp
 ** brief: SpringMass simulation implementation
 ** author: Andrea Vedaldi
 **/

#include "springmass.h"

#include <iostream>

/* ---------------------------------------------------------------- */
// class Mass
/* ---------------------------------------------------------------- */

Mass::Mass()
: position(), velocity(), force(), mass(1), radius(1)
{ }

Mass::Mass(Vector2 position, Vector2 velocity, double mass, double radius)
: position(position), velocity(velocity), force(), mass(mass), radius(radius),
xmin(-1), xmax(1), ymin(-1), ymax(1)
{ }

void Mass::setForce(Vector2 f)
{
  force = f ;
}

void Mass::addForce(Vector2 f)
{
  force = force + f ;
}

Vector2 Mass::getForce() const
{
  return force ;
}

Vector2 Mass::getPosition() const
{
  return position ;
}

Vector2 Mass::getVelocity() const
{
  return velocity ;
}

double Mass::getRadius() const
{
  return radius ;
}

double Mass::getMass() const
{
  return mass ;
}

double Mass::getEnergy(double gravity) const
{
  double energy = 0 ;
 
  energy = 0.5 * getMass() * getVelocity().norm2();
  energy = energy + getMass() * gravity * position.y;
  
  return energy ;
}

void Mass::step(double dt)
{
	double xp = position.x + velocity.x * dt + 0.5 * (force.x / mass) * dt * dt ;
	double yp = position.y + velocity.y * dt + 0.5 * (force.y / mass) * dt * dt ;

	if (xmin + radius <= xp && xp <= xmax - radius) {
    position.x = xp ;
	velocity.x = velocity.x - force.x * dt ;
	} 
	else {
    velocity.x = -velocity.x ;
	}
	if (ymin + radius <= yp && yp <= ymax - radius) {
    position.y = yp ;
    velocity.y = velocity.y - force.y * dt ;
	} 
	else {
    velocity.y = -velocity.y ;
  }

}

/* ---------------------------------------------------------------- */
// class Spring
/* ---------------------------------------------------------------- */

Spring::Spring(Mass * mass1, Mass * mass2, double naturalLength, double stiffness, double damping)
: mass1(mass1), mass2(mass2), naturalLength(naturalLength), stiffness(stiffness), damping(damping)
{ }

Mass * Spring::getMass1() const
{
  return mass1 ;
}

Mass * Spring::getMass2() const
{
  return mass2 ;
}

Vector2 Spring::getForce() const
{
  Vector2 F , xa, xb, u, va, vb ;
  xa = mass1->getPosition() ;
  xb = mass2->getPosition() ;
  va = mass1->getVelocity() ;
  vb = mass2->getVelocity() ;
  u = (xb-xa)/getLength() ;

  F = stiffness * ((getLength() - naturalLength) * u) + damping * (dot((vb - va),u) * u);

  return F ;
}

double Spring::getLength() const
{
  Vector2 u = mass2->getPosition() - mass1->getPosition() ;
  return u.norm() ;
}

double Spring::getEnergy() const {
  double length = getLength() ;
  double dl = length - naturalLength;
  return 0.5 * stiffness * dl * dl ;
}

std::ostream& operator << (std::ostream& os, const Mass& m)
{
  os<<"("
  <<m.getPosition().x<<","
  <<m.getPosition().y<<")" ;
  return os ;
}

std::ostream& operator << (std::ostream& os, const Spring& s)
{
  return os<<"$"<<s.getLength() ;
}

/* ---------------------------------------------------------------- */
// class SpringMass : public Simulation
/* ---------------------------------------------------------------- */

SpringMass::SpringMass(Mass * m1, Mass * m2, Spring * spring, double gravity)
: m1(m1), m2(m2), spring(spring) ,gravity(gravity)
{ }

void SpringMass::display()
{
  std::cout<<m1 -> getPosition().x<<" "<<m1 -> getPosition().y<<" "<<m2 -> getPosition().x<<" "<<m2 -> getPosition().y<<std::endl ;
}

double SpringMass::getEnergy() const
{
  double energy = 0 ;

  return energy ;
}

void SpringMass::step(double dt)
{
  Vector2 F1, F2;
  Vector2 g(0,-gravity) ;
  m1 -> setForce(g) ;
  m2 -> setForce(g) ;
  F1 = spring -> getForce() ;
  F2 = -1 * F1 ;
  m1 -> addForce(F1) ;
  m2 -> addForce(F2) ;
  m1 -> step(dt) ;
  m2 -> step(dt) ;
}

