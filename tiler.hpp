#ifndef TILER_HPP 
#define TILER_HPP 

#include <string>

struct Param
{
  int x;
  int y;
  bool is_active;
  bool is_static;
};

std::string serParam(Param p);

#endif 
