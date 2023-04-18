#include <array>
#include <cmath>
#include <functional>
#include <type_traits>
#include <vector>

namespace poly
{
template<std::size_t Degree>
struct is_degree_2 : std::false_type {};

template<>
struct is_degree_2<2> : std::true_type {};

struct empty{};

template<typename Real, std::size_t Degree>
class polynomials;

template<typename Real, std::size_t Degree>
class derivative
{
protected:
  polynomials<Real, Degree> mDerivative;
};

template<typename Real, std::size_t Degree>
class polynomials : public std::conditional<is_degree_2<Degree>::value, empty, derivative<Real, Degree-1>>::type
{
public:
  polynomials() = default;

  template<std::size_t V = Degree>
  polynomials(std::array<Real, Degree+1> coefficients,
   typename std::enable_if<!is_degree_2<V>::value>::type* = 0)
  : mCoefficients(std::move(coefficients))
  {
    std::array<Real, Degree> d;
    for (std::size_t deg = 0; deg < Degree; deg++)
    {
      d[deg] = coefficients[deg] * static_cast<Real>(Degree - deg);
    }
    this->mDerivative = polynomials<Real, Degree-1>(d);
  }

  template<std::size_t V = Degree>
  polynomials(std::array<Real, Degree+1> coefficients,
   typename std::enable_if<is_degree_2<V>::value>::type* = 0)
  : mCoefficients(std::move(coefficients))
  {
    // derivative is not needed for degree 2
  }

  template<std::size_t V = Degree>
  typename std::enable_if<!is_degree_2<V>::value, std::vector<Real>>::type
  find_roots(Real lower_bound, Real upper_bound)
  {
    auto optims = this->mDerivative.find_roots(lower_bound, upper_bound);

    // std::vector<Real> roots;

    return {};
  }

  template<std::size_t V = Degree>
  typename std::enable_if<is_degree_2<V>::value, std::vector<Real>>::type
  find_roots(Real lower_bound, Real upper_bound)
  {
    Real delta = mCoefficients[1] * mCoefficients[1] - 4. * mCoefficients[0] * mCoefficients[2];

    if (delta < 0)
      return {};

    if (delta == 0)
    {
      Real x = -mCoefficients[1] / (2. * mCoefficients[0]);
      if (x < lower_bound || x > upper_bound)
        return {};
      return { x };
    }
    Real s = std::sqrt(delta);
    Real x0 = (-mCoefficients[1] - s) / (2. * mCoefficients[0]);
    Real x1 = (-mCoefficients[1] + s) / (2. * mCoefficients[0]);

    std::vector<Real> values;
    values.reserve(2);

    if (x0 >= lower_bound && x0 <= upper_bound)
      values.push_back(x0);
    if (x1 >= lower_bound && x1 <= upper_bound)
      values.push_back(x1);

    return values;
  }

  Real evaluate(Real x)
  {
    Real val = mCoefficients[0];
    for (auto it = mCoefficients.cbegin() + 1; it != mCoefficients.cend(); it++)
    {
      val = val * x + (*it);
    }
  }

private:
  std::vector<Real> find_root_monotonic(Real lower_bound, Real upper_bound)
  {
    Real guess = lower_bound - evaluate(lower_bound) / this->mDerivative.evaluate(lower_bound);

    // if (guess > upper_bound)
    // {

    // }
  }

  std::array<Real, Degree+1> mCoefficients;
};
}
