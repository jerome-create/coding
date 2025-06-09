#include "tools.h"

bool circle_inside(const Circle &circle, double arena_radius, bool use_eps)
{
    double limit = arena_radius - circle.r;
    if (use_eps)
        limit += epsil_zero;
    return norm(circle.c) <= limit;
}

bool circles_intersect(const Circle &c1, const Circle &c2, bool use_eps)
{
    double limit = c1.r + c2.r;
    if (use_eps)
        limit += epsil_zero;
    return distance(c1.c, c2.c) <= limit;
}

S2d reflect(const S2d &v, const S2d &normal)
{
    S2d n = normalize(normal);
    double d = dot(v, n);
    return sub(v, scale(n, 2.0 * d));
}
