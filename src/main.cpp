/*
    This file is part of test_geometry
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "segment.hpp"
#include "polygon.hpp"
#include "simple_gui.h"
#include "convex_shape.hpp"
#include <iostream>

#include <vector>
#include <math.h>

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
  
  std::cout << "Belong : " << (l_seg.belong(point(l_x,l_y)) ? "oui" : "non") << std::endl ;

  point l_up(7,7);
  point l_down(5,0);
  point l_on(5,2.5);

  std::cout << "Vectorial product point Up : " << l_seg.get_side(l_up) << std::endl ;
  std::cout << "Vectorial product point Down : " << l_seg.get_side(l_down) << std::endl ;
  std::cout << "Vectorial product point On : " << l_seg.get_side(l_on) << std::endl ;

  point l_A(0,0);
  point l_B(5,0);
  point l_C(5,3);
  point l_C_bis(3,1);
  point l_D(2,3);
  point l_E(5,1);

  std::vector<point> l_list;
  l_list.push_back(l_B);
  l_list.push_back(l_E);
  l_list.push_back(l_D);
  l_list.push_back(l_A);

  polygon l_polygon2(l_list);

  l_list.clear();
  l_list.push_back(point(0,0));
  l_list.push_back(point(5,0));
  l_list.push_back(point(5,1));
  l_list.push_back(point(4,1));
  l_list.push_back(point(4,2));
  l_list.push_back(point(6,2));
  l_list.push_back(point(6,3));
  l_list.push_back(point(0,3));
  polygon l_polygon(l_list);

  std::cout << "Convex ? " << ( l_polygon.is_convex() ? "yes" : "no" ) << std::endl ;

  point l_UN_ZERO(1,0);
  point l_ZERO_UN(0,1);
  point l_ZERO_ZERO(0,0);
  point l_UN_UN(1,1);
  segment l_seg1(l_ZERO_ZERO,l_UN_ZERO);
  segment l_seg2(l_UN_ZERO,l_ZERO_UN);
  double l_scalar = l_seg1.scalar_product(l_seg2);
  std::cout << "Scalar product = " << l_scalar << std::endl ;

  std::cout << "Square size seg1 : " << l_seg1.get_square_size() << std::endl ;
  std::cout << "Square size seg2 : " << l_seg2.get_square_size() << std::endl ;

  double l_angle_rad = acos(l_scalar / (pow(l_seg1.get_square_size() * l_seg2.get_square_size(),0.5)));
  std::cout << "Angle radian : " << l_angle_rad << std::endl ;
  std::cout << "Angle degre : " << l_angle_rad * 180.0 / M_PI << std::endl ;
  std::cout << (l_UN_ZERO < l_ZERO_UN) << std::endl ;
  std::cout << (l_ZERO_UN < l_UN_ZERO) << std::endl ;
  std::cout << (l_ZERO_ZERO < l_ZERO_ZERO) << std::endl ;

  simple_gui l_gui;
  l_gui.createWindow(640,480);

  uint32_t l_nb_segments = l_polygon.get_nb_segment();
  for(uint32_t l_index = 0 ; l_index < l_nb_segments ; ++l_index)
    {
      l_gui.draw_line(l_polygon.get_segment(l_index).get_source().get_x()*30+10,
		      l_polygon.get_segment(l_index).get_source().get_y()*30+10,
		      l_polygon.get_segment(l_index).get_dest().get_x()*30+10,
		      l_polygon.get_segment(l_index).get_dest().get_y()*30+10,
		      l_gui.getColorCode(255,0,0)
		      );
    }
  l_gui.refresh();

  l_polygon.cut_in_convex_polygon();

  convex_shape l_convex_shape(point(3,0),point(0,10),point(6,9));
  std::cout << "Contains (0,0) ? " << l_convex_shape.contains(point(0,0)) << std::endl ;
  std::cout << "Contains (3,5) ? " << l_convex_shape.contains(point(3,5)) << std::endl ;
  sleep(10);
}
