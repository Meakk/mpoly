#include "polynomials.h"

#include <iostream>

int main()
{
  poly::polynomials<double, 2> p({ 1., 0., -2.});
  auto roots = p.find_roots(-10., 10.);

  for (auto r : roots)
  {
    std::cout << r << std::endl;
  }
}
