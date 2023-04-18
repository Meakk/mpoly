#include "polynomials.h"

#include <iostream>

int main()
{
  auto rootsDg2 = poly::polynomial<double, 2>({ 1., 0., -2. }).find_roots(-10., 10.);

  std::cout << "degree 2:\n";
  for (auto r : rootsDg2)
  {
    std::cout << r << std::endl;
  }

  auto rootsDg3 = poly::polynomial<double, 3>({ 1., -3., 1., 2. }).find_roots(-10., 10.);

  std::cout << "degree 3:\n";
  for (auto r : rootsDg3)
  {
    std::cout << r << std::endl;
  }

  auto rootsDg4 = poly::polynomial<double, 4>({ 2., -2., -3., 1., 2. }).find_roots(-10., 10.);

  std::cout << "degree 4:\n";
  for (auto r : rootsDg4)
  {
    std::cout << r << std::endl;
  }

  return 0;
}
