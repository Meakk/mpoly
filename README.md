# mpoly - a simple header-only C++ polynomial root finder

## Usage

There's a single `polynomial` class available with two functions: `evaluate` and `find_roots`

Here's an example:

```cpp
// define the cubic polynomial: x^3 - 3x^2 + x + 2
auto cubic = mpoly::polynomial<double, 3>({ 1., -3., 1., 2. });

// find all roots between -10 and 10
// returns a vector containing all the roots in the interval
auto roots = cubic.find_roots(-10., 10.);

// That's it! You can double check the roots using evaluate if needed
for (double x : roots)
{
    double y = cubic.evaluate(x);
    // y should be close to zero
}
```

## How to use the library in your project

### Clean and modern CMake way

Add this piece of code in your CMake file:

```cmake
find_package(mpoly REQUIRED)

target_link_library(your_target PRIVATE mpoly::mpoly)
```

You should be able to include `mpoly.h` in your code

### Quick and dirty way

Just copy `mpoly.h` in your source tree

## How fast and stable is it?

It hasn't been tested extensively but it should be fairly stable and fast if you are looking for roots inside a known interval.  
If you tested it and found a bug, feel free to open an issue or a pull request, I'm happy to take a look.

## Sources

- "A Fast & Robust Solution for Cubic & Higher-Order Polynomials" by Cem Yuksel
- [Numerically stable quadratic solutions](https://math.stackexchange.com/questions/866331/numerically-stable-algorithm-for-solving-the-quadratic-equation-when-a-is-very)