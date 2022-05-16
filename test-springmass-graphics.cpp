/** file: test-springmass-graphics.cpp
 ** brief: Tests the spring mass simulation with graphics
 ** author: Andrea Vedaldi
 **/

#include "graphics.h"
#include "springmass.h"

#include <iostream>
#include <sstream>
#include <iomanip>

/* ---------------------------------------------------------------- */
class SpringMassDrawable : public SpringMass, public Drawable
/* ---------------------------------------------------------------- */
{



private:
  Figure figure ;

public:
  SpringMassDrawable(Mass * m1, Mass * m2, Spring *spring, double gravity)
  : SpringMass( m1, m2, spring, gravity), figure("Spring mass")
  {
    figure.addDrawable(this) ;
  }

  void draw() {
    figure.drawCircle(m1->getPosition().x,m1->getPosition().y,m1->getRadius()) ;
	figure.drawCircle(m2->getPosition().x,m2->getPosition().y,m2->getRadius()) ;
	figure.drawLine(m1->getPosition().x, m1->getPosition().y,m2->getPosition().x, m2->getPosition().y,2);
  }
  

  void display() {
    figure.update() ;
  }
} ;



int main(int argc, char** argv)
{
  glutInit(&argc,argv) ;

 

  
  const double mass = 0.05 ;
  const double radius = 0.02 ;
  const double naturalLength = 0.95 ;

  Mass m1(Vector2(-.5,0), Vector2(), mass, radius) ;
  Mass m2(Vector2(+.5,0), Vector2(), mass, radius) ;
  Spring spring(&m1, &m2, 3, 2);
 SpringMassDrawable springmass(&m1, &m2,&spring, 9.81);
  run(&springmass, 1/120.0) ;
}
