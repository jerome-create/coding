#ifndef TOOLS_H
#define TOOLS_H
#include <cmath>

struct S2d
{
    double x{};
    double y{};
};

constexpr double epsil_zero = 1e-6;

inline double norm(const S2d &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline double distance(const S2d &a, const S2d &b)
{
    return norm({a.x - b.x, a.y - b.y});
}

inline S2d add(const S2d &a, const S2d &b)
{
    return {a.x + b.x, a.y + b.y};
}

inline S2d sub(const S2d &a, const S2d &b)
{
    return {a.x - b.x, a.y - b.y};
}

inline S2d scale(const S2d &v, double s)
{
    return {v.x * s, v.y * s};
}

inline S2d normalize(const S2d &v)
{
    double n = norm(v);
    return n > 0 ? scale(v, 1.0 / n) : S2d{0, 0};
}

inline double dot(const S2d &a, const S2d &b)
{
    return a.x * b.x + a.y * b.y;
}

struct Circle
{
    S2d c;
    double r{};
};

bool circle_inside(const Circle &circle, double arena_radius, bool use_eps);
bool circles_intersect(const Circle &c1, const Circle &c2, bool use_eps);
S2d reflect(const S2d &v, const S2d &normal);

#endif // TOOLS_H
