#ifndef MOBILE_H
#define MOBILE_H

#include <vector>
#include "tools.h"
#include "constantes.h"

struct Particule
{
    S2d pos;
    double angle{};
    double displacement{};
    unsigned counter{};
};

struct Faiseur
{
    S2d head;
    double angle{};
    double displacement{};
    double radius{};
    unsigned nbe{};
    std::vector<S2d> elements; // include head
};

void compute_faiseur_elements(Faiseur &f);

#endif // MOBILE_H
