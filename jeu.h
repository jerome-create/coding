#ifndef JEU_H
#define JEU_H

#include <string>
#include <vector>
#include "mobile.h"
#include "chaine.h"

class Jeu
{
public:
    bool lecture(const std::string &filename);

private:
    unsigned score{};
    std::vector<Particule> particules;
    std::vector<Faiseur> faiseurs;
    Chaine chaine;
};

#endif // JEU_H
