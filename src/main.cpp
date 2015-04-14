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
#include <cmath>
#include <unistd.h>

using namespace geometry;

typedef int64_t my_type;

//------------------------------------------------------------------------------
void draw_point(simple_gui & p_gui,const point<my_type> & p, const uint32_t & p_color_code)
{
  p_gui.set_pixel_without_lock(p.get_x()+10,p.get_y()+10,p_color_code);
}

//------------------------------------------------------------------------------
void draw_internal_shape(simple_gui & p_gui,const shape<my_type> & p_shape,const uint32_t & p_color_code)
{
  for(my_type l_y = 0 ; l_y < 480 ; l_y += 10)
    {
      for(my_type l_x = 0 ; l_x < 640 ; l_x += 10)
        {
          point<my_type> l_point(l_x,l_y);
          if(p_shape.contains(l_point,false))
            {
#ifdef VERBOSE
              std::cout << l_point << " : IN" << std::endl ;
#endif
              draw_point(p_gui,l_point,p_gui.getColorCode(0,255,0));
            }
        }
      p_gui.refresh();
    }
}
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void clear_gui(simple_gui & p_gui)
{
  p_gui.set_rectangle_without_lock(0,0,640,480,p_gui.getColorCode(0,0,0));
  p_gui.refresh();
}

//------------------------------------------------------------------------------
void check_intersec(const segment<my_type> & p_seg1,const segment<my_type> & p_seg2, bool p_result)
{
  std::cout << "Check Intersec between " << p_seg1 << " and " <<  p_seg2 << " : " ;
  bool l_result = p_seg1.intersec(p_seg2);
  std::cout << (l_result ? "yes" : "no") << std::endl ;
  assert(p_result == l_result);
}

//------------------------------------------------------------------------------
void check_intersec(const segment<my_type> & p_seg1,const segment<my_type> & p_seg2, bool p_result, bool p_single_intersec,const point<my_type> & p_intersec_point)
{
  bool l_single_intersec;
  point<my_type> l_intersec_point(0,0);
  std::cout << "Check Intersec between " << p_seg1 << " and " <<  p_seg2 << " : " ;
  bool l_result = p_seg1.intersec(p_seg2,l_single_intersec,l_intersec_point);
  std::cout << (l_result ? "yes" : "no");
  if(l_single_intersec)
    {
      std::cout << " => " << l_intersec_point ;
    }
  std::cout << std::endl ;
  assert(p_result == l_result);
  if(l_result)
    {
      assert(p_single_intersec == l_single_intersec);
      if(l_single_intersec)
        {
          assert(p_intersec_point == l_intersec_point);
        }
    }
}
//------------------------------------------------------------------------------
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

  segment<my_type> l_vertical_segment(0,0,0,20);
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


  segment<my_type> l_vertical_segment2(10,0,10,20);
  segment<my_type> l_vertical_segment3(0,100,0,200);
  segment<my_type> l_vertical_segment4(0,150,0,250);
  segment<my_type> l_vertical_segment5(0,50,0,250);

  // Parallels vertical segments
  check_intersec(l_vertical_segment,l_vertical_segment2,false);
  // Aligned but not crossing
  check_intersec(l_vertical_segment,l_vertical_segment3,false);
  // Aligned crossings
  check_intersec(l_vertical_segment3,l_vertical_segment4,true);
  check_intersec(l_vertical_segment3,l_vertical_segment5,true);

  segment<my_type> l_horizontal_segment2(0,10,20,10);
  segment<my_type> l_horizontal_segment3(100,0,200,0);
  segment<my_type> l_horizontal_segment4(150,0,250,0);
  segment<my_type> l_horizontal_segment5(50,0,250,0);

  // Parallels horizontal segments
  check_intersec(l_horizontal_segment,l_horizontal_segment2,false);
  // Aligned but not crossing
  check_intersec(l_horizontal_segment,l_horizontal_segment3,false);
  // Aligned crossings
  check_intersec(l_horizontal_segment3,l_horizontal_segment4,true);
  check_intersec(l_horizontal_segment3,l_horizontal_segment5,true);

  // Other Other
  check_intersec(segment<my_type>(1,1,5,3),segment<my_type>(2,3,4,2),true,true,point<my_type>(3,2));
  check_intersec(segment<my_type>(0,0,2,2),segment<my_type>(1,1,3,3),true,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(0,0,3,3),segment<my_type>(1,1,2,2),true,false,point<my_type>(0,0));
  // Horizontal and vertical segments
  check_intersec(segment<my_type>(0,0,10,0),segment<my_type>(5,10,5,-10),true,true,point<my_type>(5,0));
  check_intersec(segment<my_type>(5,10,5,-10),segment<my_type>(0,0,10,0),true,true,point<my_type>(5,0));
  // Other vs Horizontal
  check_intersec(segment<my_type>(0,0,10,0),segment<my_type>(0,-5,10,5),true,true,point<my_type>(5,0));
  check_intersec(segment<my_type>(0,-5,10,5),segment<my_type>(0,0,10,0),true,true,point<my_type>(5,0));
  // Other vs Vertical
  check_intersec(segment<my_type>(5,5,5,-5),segment<my_type>(0,-5,10,5),true,true,point<my_type>(5,0));
  check_intersec(segment<my_type>(0,-5,10,5),segment<my_type>(5,5,5,-5),true,true,point<my_type>(5,0));

  check_intersec(segment<my_type>(135,0,135,25),segment<my_type>(100,50,170,50),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(0,0,100,0),segment<my_type>(101,0,200,0),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(0,0,100,0),segment<my_type>(100,0,200,0),true,true,point<my_type>(100,0));
  check_intersec(segment<my_type>(0,0,100,0),segment<my_type>(80,0,200,0),true,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(0,0,100,0),segment<my_type>(50,0,700,0),true,false,point<my_type>(0,0));

  check_intersec(segment<my_type>(100,50,170,50),segment<my_type>(135,0,135,25),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(101,0,200,0),segment<my_type>(0,0,100,0),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(100,0,200,0),segment<my_type>(0,0,100,0),true,true,point<my_type>(100,0));
  check_intersec(segment<my_type>(80,0,200,0),segment<my_type>(0,0,100,0),true,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(50,0,700,0),segment<my_type>(0,0,100,0),true,false,point<my_type>(0,0));

  check_intersec(segment<my_type>(175,150,250,150),segment<my_type>(250,50,100,150),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(250,50,100,150),segment<my_type>(175,150,250,150),false,false,point<my_type>(0,0));

  check_intersec(segment<my_type>(175,0,175,50),segment<my_type>(250,50,100,150),false,false,point<my_type>(0,0));
  check_intersec(segment<my_type>(250,50,100,150),segment<my_type>(175,0,175,50),false,false,point<my_type>(0,0));

  check_intersec(segment<my_type>(477,224,504,224),segment<my_type>(491,205,464,243),true,true,point<my_type>(478,224));
  check_intersec(segment<my_type>(491,205,464,243),segment<my_type>(477,224,504,224),true,true,point<my_type>(478,224));

  check_intersec(segment<my_type>(477,224,477,261),segment<my_type>(491,205,464,243),true,true,point<my_type>(477,225));
  check_intersec(segment<my_type>(491,205,464,243),segment<my_type>(477,224,477,261),true,true,point<my_type>(477,225));

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
  
  std::cout << "--------- TEST QUADRAN  ------------" << std::endl ;
  point<my_type> l_reference_point(1,1);
  std::vector<std::pair<point<my_type>,unsigned int>> l_quadran_points = {
    std::pair<point<my_type>,unsigned int>(point<my_type>(0,0),0),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x() - 0.001,0),0),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x(),0),1),
    std::pair<point<my_type>,unsigned int>(point<my_type>(2 * l_reference_point.get_x(),0),1),
    std::pair<point<my_type>,unsigned int>(point<my_type>(0,2 * l_reference_point.get_y()),3),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x() - 0.001,2 * l_reference_point.get_y()),3),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x(),2 * l_reference_point.get_y()),2),
    std::pair<point<my_type>,unsigned int>(point<my_type>(2 * l_reference_point.get_x(),2 * l_reference_point.get_y()),2),
    std::pair<point<my_type>,unsigned int>(point<my_type>(0,l_reference_point.get_y() - 0.001),0),
    std::pair<point<my_type>,unsigned int>(point<my_type>(0,l_reference_point.get_y()),3),
    std::pair<point<my_type>,unsigned int>(point<my_type>(0,2 * l_reference_point.get_y()),3),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x(),l_reference_point.get_y() - 0.001),1),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x(),1),2 * l_reference_point.get_y()),
    std::pair<point<my_type>,unsigned int>(point<my_type>(l_reference_point.get_x(),2 * l_reference_point.get_y()),2),
    std::pair<point<my_type>,unsigned int>(point<my_type>(2 * l_reference_point.get_x(),l_reference_point.get_y() - 0.001),1),
    std::pair<point<my_type>,unsigned int>(point<my_type>(2 * l_reference_point.get_x(),1),2 * l_reference_point.get_y()),
    std::pair<point<my_type>,unsigned int>(point<my_type>(2 * l_reference_point.get_x(),2 * l_reference_point.get_y()),2)
  };

  for(auto l_iter : l_quadran_points)
    {
      unsigned int l_quadran = quad<my_type>::compute_quadran(l_iter.first,l_reference_point);
      std::cout << "Point " << l_iter.first << " is computed in quadran " << l_quadran << "\t expected = " << l_iter.second << std::endl;
      assert(l_iter.second == l_quadran);
    }
}

bool fract(const std::vector<point<my_type>> & p_points, std::vector<point<my_type>> & p_result)
{
  assert(p_points.size() >= 2);
  for(unsigned int l_index = 0; l_index < p_points.size() ; ++l_index)
    {
      my_type l_delta_x = p_points[(l_index + 1) % p_points.size()].get_x() - p_points[l_index].get_x();
      my_type l_delta_y = p_points[(l_index + 1) % p_points.size()].get_y() - p_points[l_index].get_y();
      my_type l_module = l_delta_x * l_delta_x + l_delta_y * l_delta_y;
      if(l_module < 3)
	{
	  return true;
	}
      my_type l_relative_x = l_delta_x /3;
      my_type l_relative_y = l_delta_y /3;

      p_result.push_back(p_points[l_index]);
      my_type l_new_x1 = p_points[l_index].get_x() + l_relative_x;
      my_type l_new_y1 = p_points[l_index].get_y() + l_relative_y;
      p_result.push_back(point<my_type>(l_new_x1,l_new_y1));

      double l_pi = M_PI;
      double l_angle = - l_pi /3;
      my_type l_new_relative_x = l_relative_x * cos(l_angle) - l_relative_y * sin(l_angle);
      my_type l_new_relative_y = l_relative_y * cos(l_angle) + l_relative_x * sin(l_angle);

      my_type l_new_x2 = l_new_x1 + l_new_relative_x;
      my_type l_new_y2 = l_new_y1 + l_new_relative_y;
      p_result.push_back(point<my_type>(l_new_x2,l_new_y2));

      my_type l_new_x3 = p_points[l_index].get_x() + 2 * l_relative_x;
      my_type l_new_y3 = p_points[l_index].get_y() + 2 * l_relative_y;
      p_result.push_back(point<my_type>(l_new_x3,l_new_y3));

    }
  // p_result.push_back(p_points[p_points.size() - 1]);
  return false;
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


  //-------------------------------------------------------------------
  std::vector<point<my_type>> l_points1 = {point<my_type>(55,140),point<my_type>(545,140),point<my_type>(300,460)};
  std::vector<point<my_type>> l_points2;
  std::vector<point<my_type>> * l_old_points = & l_points2;
  std::vector<point<my_type>> * l_new_points = & l_points1;
  bool l_finish = false;
  while(!l_finish)
    {
      std::swap<std::vector<point<my_type>> *>(l_old_points,l_new_points);
      l_new_points->clear();
      //      clear_gui(l_gui);
      //      draw_shape(l_gui,polygon<my_type>(*l_old_points),l_gui.getColorCode(255,255,255));
      //      l_gui.refresh();
      std::cout << "Number of points : " << l_old_points->size() << std::endl ;
      test_polygon(l_gui,*l_old_points);
      l_finish = fract(*l_old_points,*l_new_points);
    }

  clear_gui(l_gui);
  draw_shape(l_gui,polygon<my_type>(*l_old_points),l_gui.getColorCode(255,255,255));
  l_gui.refresh();
}
