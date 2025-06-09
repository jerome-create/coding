#include "jeu.h"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
        return 0;
    Jeu jeu;
    jeu.lecture(argv[1]);
    return 0;
}
