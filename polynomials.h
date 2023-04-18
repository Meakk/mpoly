#include <array>
#include <cmath>
#include <functional>
#include <type_traits>
#include <vector>

namespace poly
{

namespace details
{
template<typename Real, std::size_t Degree>
class polynomial_base
{
public:
  polynomial_base() = default;

  polynomial_base(std::array<Real, Degree+1> coefficients)
  : mCoefficients(std::move(coefficients))
  {
  }

  Real evaluate(Real x)
  {
    Real val = this->mCoefficients[0];
    for (auto it = this->mCoefficients.cbegin() + 1; it != this->mCoefficients.cend(); it++)
    {
      val = val * x + (*it);
    }
    return val;
  }

protected:
  std::array<Real, Degree+1> mCoefficients;
};
}

template<typename Real, std::size_t Degree>
class polynomial : public details::polynomial_base<Real, Degree>
{
public:
  polynomial() = default;

  polynomial(std::array<Real, Degree+1> coefficients)
  : details::polynomial_base<Real, Degree>(std::move(coefficients))
  {
    std::array<Real, Degree> d;
    for (std::size_t deg = 0; deg < Degree; deg++)
    {
      d[deg] = coefficients[deg] * static_cast<Real>(Degree - deg);
    }
    this->mDerivative = polynomial<Real, Degree-1>(d);
  }

  std::vector<Real> find_roots(Real lower_bound, Real upper_bound)
  {
    auto crits = this->mDerivative.find_roots(lower_bound, upper_bound);

    if (crits.size() == 0)
    {
      if (same_sign(lower_bound, upper_bound))
        return {};

      return { find_root_monotonic(lower_bound, upper_bound) };
    }

    std::vector<Real> roots;

    if (!same_sign(lower_bound, crits.front()))
      roots.push_back(find_root_monotonic(lower_bound, crits.front()));

    for (size_t i = 0; i < crits.size() - 1; i++)
    {
      if (!same_sign(crits[i], crits[i + 1]))
        roots.push_back(find_root_monotonic(crits[i], crits[i + 1]));
    }

    if (!same_sign(crits.back(), upper_bound))
      roots.push_back(find_root_monotonic(crits.back(), upper_bound));

    return roots;
  }

private:
  Real find_root_monotonic(Real lower_bound, Real upper_bound)
  {
    Real guess = lower_bound - this->evaluate(lower_bound) / this->mDerivative.evaluate(lower_bound);

    if (guess >= upper_bound || guess <= lower_bound)
    {
      guess = lower_bound + 0.5 * (upper_bound - lower_bound);
    }

    Real val = this->evaluate(guess);

    if (std::abs(val) < 1e-5)
    {
      return guess;
    }

    if (!same_sign(lower_bound, guess))
      return find_root_monotonic(lower_bound, guess);

    return find_root_monotonic(guess, upper_bound);
  }

  inline bool same_sign(Real x0, Real x1) {
    Real y0 = this->evaluate(x0);
    Real y1 = this->evaluate(x1);
    return (y0 > Real(0) && y1 > Real(0)) || (y0 < Real(0) && y1 < Real(0));
  }

  polynomial<Real, Degree-1> mDerivative;
};

template<typename Real>
class polynomial<Real, 2> : public details::polynomial_base<Real, 2>
{
public:
  polynomial() = default;

  polynomial(std::array<Real, 3> coefficients)
  : details::polynomial_base<Real, 2>(std::move(coefficients))
  {
    // derivative is not needed for degree 2
  }

  std::vector<Real> find_roots(Real lower_bound, Real upper_bound)
  {
    Real delta = this->mCoefficients[1] * this->mCoefficients[1] - 4. * this->mCoefficients[0] * this->mCoefficients[2];

    if (delta < 0)
      return {};

    if (delta == 0)
    {
      Real x = -this->mCoefficients[1] / (2. * this->mCoefficients[0]);
      if (x < lower_bound || x > upper_bound)
        return {};
      return { x };
    }
    Real s = std::sqrt(delta);
    Real x0 = (-this->mCoefficients[1] - s) / (2. * this->mCoefficients[0]);
    Real x1 = (-this->mCoefficients[1] + s) / (2. * this->mCoefficients[0]);

    std::vector<Real> values;
    values.reserve(2);

    if (x0 >= lower_bound && x0 <= upper_bound)
      values.push_back(x0);
    if (x1 >= lower_bound && x1 <= upper_bound)
      values.push_back(x1);

    return values;
  }
};
}
