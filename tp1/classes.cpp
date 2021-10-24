//TP1 Crypto
//Hamze AL RASHEED
//Rémi BOUVIER

#include <iostream>
#include <vector>

using namespace std;

struct Config{
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    int taille_min = 4;
    int taille_max = 5;
    long n = 0;      // le nombre total de textes clairs valides
    string h = "MD5";  //type de hashage
    int taille_alphabet = this->alphabet.size();
    vector<uint64_t> tab_text_clair;

    Config(){}

    Config(int taille_min, int taille_max, string alphabet) {
        this->taille_min = taille_min;
        this->taille_max = taille_max;
        this->alphabet = alphabet;
        this->taille_alphabet = this->alphabet.size();
    };
};

typedef struct {
    uint64_t debut, fin;
}Chaine;

