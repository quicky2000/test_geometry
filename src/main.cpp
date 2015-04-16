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
#include "shape.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include <unistd.h>

using namespace geometry;

typedef int64_t my_type;

void draw_point(simple_gui & p_gui,const point<my_type> & p, const uint32_t & p_color_code)
{
  p_gui.set_pixel_without_lock(p.get_x()+10,p.get_y()+10,p_color_code);
}

void draw_internal_shape(simple_gui & p_gui,const shape<my_type> & p_shape,const uint32_t & p_color_code)
{
  for(my_type l_y = 0 ; l_y < 350 ; l_y += 10)
    {
      for(my_type l_x = 0 ; l_x < 350 ; l_x += 10)
        {
          point<my_type> l_point(l_x,l_y);
          if(p_shape.contains(l_point,false))
            {
#ifdef VERBOSE
              std::cout << l_point << " : IN" << std::endl ;
#endif
              draw_point(p_gui,l_point,p_gui.getColorCode(0,255,0));
              p_gui.refresh();
            }
        }
    }
}
void draw_shape(simple_gui & p_gui,const shape<my_type> & p_shape, const uint32_t & p_color_code)
{
  uint32_t l_nb_segments = p_shape.get_nb_segment();
  for(uint32_t l_index = 0 ; l_index < l_nb_segments ; ++l_index)
    {
      p_gui.draw_line(p_shape.get_segment(l_index).get_source().get_x()+10,
		      p_shape.get_segment(l_index).get_source().get_y()+10,
		      p_shape.get_segment(l_index).get_dest().get_x()+10,
		      p_shape.get_segment(l_index).get_dest().get_y()+10,
		      p_color_code
		      );
    }
}

void clear_gui(simple_gui & p_gui)
{
  p_gui.set_rectangle_without_lock(0,0,640,480,p_gui.getColorCode(0,0,0));
  p_gui.refresh();
}

void basic_tests(void)
{
  std::cout << "------- TEST CHECK_CONVEXITY ---------" << std::endl ;
  my_type l_orient = 0;
  bool l_result = segment<my_type>::check_convex_continuation(2,l_orient,true);
  std::cout << "Check[0] : " << l_result << std::endl ;
  assert(l_result);

  l_result = segment<my_type>::check_convex_continuation(-2,l_orient,false);
  std::cout << "Check[1] : " << l_result << std::endl ;
  assert(!l_result);

  l_result = segment<my_type>::check_convex_continuation(2,l_orient,true);
  std::cout << "Check[0] : " << l_result << std::endl ;
  assert(l_result);

  l_result = segment<my_type>::check_convex_continuation(0,l_orient,false);
  std::cout << "Check[1] : " << l_result << std::endl;
  assert(l_result);

  l_result = segment<my_type>::check_convex_continuation(-2,l_orient,false);
  std::cout << "Check[2] : " << l_result << std::endl ;
  assert(!l_result);

  std::cout << "------- TEST SEGMENT ---------" << std::endl ;
  point<my_type> l_source(0,0);
  point<my_type> l_dest(100,50);
  segment<my_type> l_seg(l_source,l_dest);

  my_type l_x = 50;
  my_type l_y = l_seg.get_y(l_x);
  std::cout << "Compute Y from X according to segment : " << std::endl ;
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;

  l_y = 25;
  l_x = l_seg.get_x(l_y);
  
  std::cout << "Compute X from Y according to segment : " << std::endl ;
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;

  l_result = l_seg.belong(point<my_type>(l_x,l_y));
  std::cout << "Belong : " << (l_result ? "yes" : "no") << std::endl ;
  assert(l_result);
  
  l_y = 75;
  l_x = l_seg.get_x(l_y);
  
  std::cout << "Compute X from Y according to segment : " << std::endl ;
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;
  
  l_result = l_seg.belong(point<my_type>(l_x,l_y));
  std::cout << "Belong : " << (l_result ? "yes" : "no") << std::endl ;
  assert(!l_result);

  segment<my_type> l_horizontal_segment(point<my_type>(0,0),point<my_type>(20,0));
  l_x = 10;
  l_y = l_horizontal_segment.get_y(l_x);
  std::cout << "Compute Y from X according to horizontal segment : " << std::endl ;
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;
  l_result = l_horizontal_segment.belong(point<my_type>(l_x,l_y));
  std::cout << "Belong : " << (l_result ? "yes" : "no") << std::endl ;
  assert(l_result);
  l_result = l_horizontal_segment.belong(point<my_type>(l_x,10));
  std::cout << "Belong outside : " << (l_result ? "yes" : "no") << std::endl ;
  assert(!l_result);

  segment<my_type> l_vertical_segment(point<my_type>(0,0),point<my_type>(0,20));
  l_y = 10;
  l_x = l_vertical_segment.get_x(l_y);
  std::cout << "Compute X from Y according to vertical segment : " << std::endl ;
  std::cout << "(" << l_x << "," << l_y << ")" << std::endl ;
  l_result = l_vertical_segment.belong(point<my_type>(l_x,l_y));
  std::cout << "Belong : " << (l_result ? "yes" : "no") << std::endl ;
  assert(l_result);
  l_result = l_vertical_segment.belong(point<my_type>(2,l_y));
  std::cout << "Belong outside : " << (l_result ? "yes" : "no") << std::endl ;
  assert(!l_result);

  std::cout << "--------- TEST VECTORIAL PRODUCT ------------" << std::endl ;
  point<my_type> l_up(70,70);
  point<my_type> l_down(50,0);
  point<my_type> l_on(50,25);

  std::cout << "Vectorial product point Up : " << l_seg.get_side(l_up) << std::endl ;
  std::cout << "Vectorial product point Down : " << l_seg.get_side(l_down) << std::endl ;
  std::cout << "Vectorial product point On : " << l_seg.get_side(l_on) << std::endl ;


  std::cout << "--------- TEST SCALAR PRODUCT ------------" << std::endl ;
  point<my_type> l_UN_ZERO(10,0);
  point<my_type> l_ZERO_UN(0,10);
  point<my_type> l_ZERO_ZERO(0,0);
  point<my_type> l_UN_UN(10,10);
  segment<my_type> l_seg1(l_ZERO_ZERO,l_UN_ZERO);
  segment<my_type> l_seg2(l_UN_ZERO,l_ZERO_UN);
  my_type l_scalar = l_seg1.scalar_product(l_seg2);
  std::cout << "Scalar product = " << l_scalar << std::endl ;

  std::cout << "Square size seg1 : " << l_seg1.get_square_size() << std::endl ;
  std::cout << "Square size seg2 : " << l_seg2.get_square_size() << std::endl ;

  std::cout << "--------- TEST ANGLES ------------" << std::endl ;
  double l_angle_rad = acos(l_scalar / (pow(l_seg1.get_square_size() * l_seg2.get_square_size(),0.5)));
  std::cout << "Angle radian : " << l_angle_rad << std::endl ;
  std::cout << "Angle degre : " << l_angle_rad * 180.0 / M_PI << std::endl ;
  std::cout << (l_UN_ZERO < l_ZERO_UN) << std::endl ;
  std::cout << (l_ZERO_UN < l_UN_ZERO) << std::endl ;
  std::cout << (l_ZERO_ZERO < l_ZERO_ZERO) << std::endl ;
}

void test_polygon(simple_gui & p_gui,const std::vector<point<my_type>> & p_list)
{
  clear_gui(p_gui);
  polygon<my_type> l_polygon(p_list);
  draw_shape(p_gui,l_polygon,p_gui.getColorCode(255,0,0));
  p_gui.refresh();

  sleep(2);

  bool l_is_convex = l_polygon.is_convex();
  std::cout << "Convex ? " << ( l_is_convex ? "yes" : "no" ) << std::endl ;
  if(!l_is_convex)
    {
      l_polygon.cut_in_convex_polygon();
      draw_shape(p_gui,l_polygon.get_convex_shape(),p_gui.getColorCode(0,0,255));
      p_gui.refresh();
    }
  sleep(2);

  draw_internal_shape(p_gui,l_polygon,p_gui.getColorCode(0,255,0));
  sleep(5);
}

int main(void)
{

  basic_tests();

  simple_gui l_gui;
  l_gui.createWindow(640,480);

  std::cout << "----------- DRAW TRAPEZ ----------" << std::endl ;
  test_polygon(l_gui,{point<my_type>(250,0),point<my_type>(250,50),point<my_type>(100,150),point<my_type>(0,0)});

  std::cout << "------- TEST CONCAV POLYGON ------------" << std::endl ;
  test_polygon(l_gui,{ point<my_type>(0,0),point<my_type>(100,0),point<my_type>(100,50),point<my_type>(170,50),point<my_type>(170,0),point<my_type>(250,0),point<my_type>(250,150),point<my_type>(0,150)});

  test_polygon(l_gui,{point<my_type>(0,0),point<my_type>(250,0),point<my_type>(250,50),point<my_type>(200,50),point<my_type>(200,100),point<my_type>(300,100),point<my_type>(300,150),point<my_type>(0,150)});

  test_polygon(l_gui,{point<my_type>(0,0),point<my_type>(250,0),point<my_type>(250,50),point<my_type>(200,50),point<my_type>(200,100),point<my_type>(300,100),point<my_type>(300,150),point<my_type>(150,150),point<my_type>(100,100),point<my_type>(50,150),point<my_type>(0,150)});


  //  l_polygon3.contains(point<my_type>(10,10));
  //  l_polygon3.contains(point<my_type>(20,30));
  //  l_polygon3.contains(point<my_type>(45,5));
  //  l_polygon3.contains(point<my_type>(52,6));
  //sleep(10);
  
  std::cout << "------- TEST CONVEX SHAPE ------------" << std::endl ;
  convex_shape<my_type> l_convex_shape(point<my_type>(50,0),point<my_type>(250,0),point<my_type>(250,250));
  clear_gui(l_gui);
  draw_shape(l_gui,l_convex_shape,l_gui.getColorCode(255,0,0));
  l_gui.refresh();
  std::cout << "Contains (0,0) ? " << l_convex_shape.contains(point<my_type>(0,0)) << std::endl ;
  std::cout << "Contains (30,10) ? " << l_convex_shape.contains(point<my_type>(150,50)) << std::endl ;
  std::cout << "Add (120,50) keep convexity : " << l_convex_shape.add(point<my_type>(600,250)) << std::endl ;
  sleep(2);
  std::cout << "Add (10,50) keep convexity : " << l_convex_shape.add(point<my_type>(50,250)) << std::endl ;
  l_gui.set_rectangle_without_lock(0,0,640,480,l_gui.getColorCode(0,0,0));
  draw_shape(l_gui,l_convex_shape,l_gui.getColorCode(255,0,0));
  l_gui.refresh();
  sleep(2);
  draw_internal_shape(l_gui,l_convex_shape,l_gui.getColorCode(0,0,255));
  sleep(5);
}
