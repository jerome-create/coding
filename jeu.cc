#include "jeu.h"
#include "message.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

static vector<string> tokenize(istream &in)
{
    vector<string> tokens;
    string line;
    while (getline(in, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        stringstream ss(line);
        string tok;
        while (ss >> tok)
            tokens.push_back(tok);
    }
    return tokens;
}

bool Jeu::lecture(const string &filename)
{
    ifstream f(filename);
    if (!f)
        return false;
    auto tokens = tokenize(f);
    size_t idx = 0;
    auto next_str = [&]() { return tokens[idx++]; };
    auto next_uint = [&]() { return static_cast<unsigned>(stoul(next_str())); };
    auto next_double = [&]() { return stod(next_str()); };

    score = next_uint();
    if (score == 0 || score > score_max)
    {
        cout << message::score_outside(score);
        exit(EXIT_FAILURE);
    }

    unsigned nb_part = next_uint();
    if (nb_part > nb_particule_max)
    {
        cout << message::nb_particule_outside(nb_part);
        exit(EXIT_FAILURE);
    }
    particules.clear();
    for (unsigned i = 0; i < nb_part; ++i)
    {
        Particule p;
        p.pos.x = next_double();
        p.pos.y = next_double();
        p.angle = next_double();
        p.displacement = next_double();
        p.counter = next_uint();

        if (p.displacement < 0 || p.displacement > d_max)
        {
            cout << message::mobile_displacement(p.displacement);
            exit(EXIT_FAILURE);
        }
        if (p.counter >= time_to_split)
        {
            cout << message::particule_counter(p.counter);
            exit(EXIT_FAILURE);
        }
        if (!circle_inside({p.pos, 0.0}, r_max, false))
        {
            cout << message::particule_outside(p.pos.x, p.pos.y);
            exit(EXIT_FAILURE);
        }
        particules.push_back(p);
    }

    unsigned nb_fai = next_uint();
    faiseurs.clear();
    faiseurs.resize(nb_fai);
    for (unsigned i = 0; i < nb_fai; ++i)
    {
        Faiseur &fa = faiseurs[i];
        fa.head.x = next_double();
        fa.head.y = next_double();
        fa.angle = next_double();
        fa.displacement = next_double();
        fa.radius = next_double();
        fa.nbe = next_uint();

        if (fa.displacement < 0 || fa.displacement > d_max)
        {
            cout << message::mobile_displacement(fa.displacement);
            exit(EXIT_FAILURE);
        }
        if (fa.radius < r_min_faiseur || fa.radius > r_max_faiseur)
        {
            cout << message::faiseur_radius(fa.radius);
            exit(EXIT_FAILURE);
        }
        if (fa.nbe == 0)
        {
            cout << message::faiseur_nbe(fa.nbe);
            exit(EXIT_FAILURE);
        }
        if (!circle_inside({fa.head, fa.radius}, r_max, false))
        {
            cout << message::faiseur_outside(fa.head.x, fa.head.y);
            exit(EXIT_FAILURE);
        }
        compute_faiseur_elements(fa);
    }

    unsigned nb_art = next_uint();
    chaine.articulations.clear();
    chaine.articulations.reserve(nb_art);
    for (unsigned i = 0; i < nb_art; ++i)
    {
        S2d a;
        a.x = next_double();
        a.y = next_double();
        if (!circle_inside({a, 0.0}, r_max, false))
        {
            cout << message::articulation_outside(a.x, a.y);
            exit(EXIT_FAILURE);
        }
        chaine.articulations.push_back(a);
    }

    string mode = next_str();
    (void)mode;

    if (!chaine.articulations.empty())
    {
        const S2d &root = chaine.articulations[0];
        if (norm(root) < r_max - r_capture)
        {
            cout << message::chaine_racine(root.x, root.y);
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i + 1 < chaine.articulations.size(); ++i)
        {
            if (distance(chaine.articulations[i], chaine.articulations[i + 1]) > r_capture)
            {
                cout << message::chaine_max_distance(i);
                exit(EXIT_FAILURE);
            }
        }
    }

    // collisions articulation vs faiseur
    for (size_t a = 0; a < chaine.articulations.size(); ++a)
    {
        for (size_t i = 0; i < faiseurs.size(); ++i)
        {
            const Faiseur &fa = faiseurs[i];
            for (size_t e = 0; e < fa.elements.size(); ++e)
            {
                if (circles_intersect({chaine.articulations[a], 0.0}, {fa.elements[e], fa.radius}, false))
                {
                    cout << message::chaine_articulation_collision(a, i, e);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // collisions faiseur vs faiseur
    for (size_t i = 0; i < faiseurs.size(); ++i)
    {
        for (size_t e1 = 0; e1 < faiseurs[i].elements.size(); ++e1)
        {
            for (size_t j = i + 1; j < faiseurs.size(); ++j)
            {
                for (size_t e2 = 0; e2 < faiseurs[j].elements.size(); ++e2)
                {
                    if (circles_intersect({faiseurs[i].elements[e1], faiseurs[i].radius},
                                           {faiseurs[j].elements[e2], faiseurs[j].radius}, false))
                    {
                        cout << message::faiseur_element_collision(i, e1, j, e2);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    cout << message::success();
    exit(0);
}

