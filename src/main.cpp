#include "segment.hpp"
#include <iostream>

int main(void)
{
  point l_source(0,0);
  point l_dest(10,5);
  segment l_seg(l_source,l_dest);

  double l_x = 5;
  double l_y = l_seg.get_y(l_x);
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;

  l_y = 2.5;
  l_x = l_seg.get_x(l_y);
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;
  
  std::cout << "Appartient : " << (l_seg.belong(point(l_x,l_y)) ? "oui" : "non") << std::endl ;
}
