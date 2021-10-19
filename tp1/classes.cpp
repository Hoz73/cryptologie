#include <iostream>
#include <vector>

using namespace std;

struct Config{
    string alphabet;
    int taille_min;
    int taille_max;
    long n = 0;      // le nombre total de textes clairs valides
    string h;  //type de hashage
    int taille_alphabet;
    vector<int> tab_text_clair;

    Config(){}

    Config(int taille_min, int taille_max, string alphabet) {
        this->alphabet = alphabet;
        this->taille_max = taille_max;
        this->taille_min = taille_min;
    };
};