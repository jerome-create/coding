#include "mobile.h"

static void step_back(S2d &pos, S2d &dir, double step, double radius)
{
    double remain = step;
    double arena = r_max - radius;
    while (remain > epsil_zero)
    {
        S2d candidate = add(pos, scale(dir, remain));
        if (norm(candidate) <= arena + epsil_zero)
        {
            pos = candidate;
            remain = 0;
        }
        else
        {
            double pd = dot(pos, dir);
            double disc = pd * pd - (norm(pos) * norm(pos) - arena * arena);
            if (disc < 0)
                disc = 0;
            double t = -pd + std::sqrt(disc);
            S2d hit = add(pos, scale(dir, t));
            S2d normal = normalize(hit);
            pos = hit;
            dir = reflect(dir, normal);
            remain -= t;
        }
    }
}

void compute_faiseur_elements(Faiseur &f)
{
    f.elements.clear();
    f.elements.push_back(f.head);
    S2d dir{-std::cos(f.angle), -std::sin(f.angle)}; // backward direction
    S2d pos = f.head;
    for (unsigned i = 1; i < f.nbe; ++i)
    {
        step_back(pos, dir, f.displacement, f.radius);
        f.elements.push_back(pos);
    }
}
