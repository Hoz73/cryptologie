//TP1 Crypto
//Hamze AL RASHEED
//Rémi BOUVIER

#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <algorithm>
#include "classes.cpp"
#include <random>
#include <fstream>
#include <cstring>
#include <getopt.h>

typedef unsigned char byte;
using namespace std;

vector<uint64_t> EMPREINTE;
vector<char *> TEXTE;
vector<Chaine> TABLE;
int LARGEUR;
int HAUTEUR;

/* Question 1 */
void hash_MD5(const char *s, byte *empreinte) {
    MD5((unsigned char *) s, strlen(s), empreinte);
}

/* Question 1 */
void hash_SHA1(const char *s, byte *empreinte) {
    SHA1((unsigned char *) s, strlen(s), empreinte);
}

/* Question 1 */
string hexStr(byte *data, int len) {
    stringstream ss;
    ss << hex;

    for (int i(0); i < len; ++i)
        ss << setw(2) << setfill('0') << (int) data[i];
    return ss.str();
}

/* Question 2 */
Config init_config(int taille_min, int taille_max, string alphabet) {
    Config config = Config(taille_min, taille_max, alphabet);

    //Modification de n et de tab_text_clair
    for (int i = config.taille_min; i <= config.taille_max; i++) {
        config.n += pow(config.taille_alphabet, i);
        config.tab_text_clair.push_back(pow(config.taille_alphabet, i));
    }
    return config;
}

/* Question 3 */
string i2c(Config config, uint64_t number) {
    uint64_t num = number;
    int i = 0;
    int j;
    for (j = config.taille_min; j <= config.taille_max; j++) {
        if (config.tab_text_clair[i] < num) {
            num = num - config.tab_text_clair[i];
            i++;
        } else {
            break;
        }
    }
    uint64_t new_n;
    string res;
    for (int i = 0; i < j; i++) {
        new_n = num % config.taille_alphabet;
        res += (config.alphabet[new_n]);
        num = (num / config.taille_alphabet);
    }
    reverse(res.begin(), res.end());
    return res;
}

/* Question 5 */
uint64_t h2i(Config config, byte *hash, int colonne) {
    uint64_t *y = (uint64_t *) hash;
    return (*y + colonne) % config.n;
}

/* Question 6 */
uint64_t i2i(Config config, long number, int t) {
    string res = i2c(config, number);
    char *c = (char *) res.c_str();
    TEXTE.push_back(c);
    byte tab[20];
    hash_MD5(c, tab);
    uint64_t result = h2i(config, tab, t);
    EMPREINTE.push_back(result);
    return result;
}

/* Question 6 */
Chaine nouvelle_chaine(Config config, uint64_t index, long largeur) {
    uint64_t res = index;
    for (int i{1}; i < largeur; i++) {
        res = i2i(config, res, i);
    }
    Chaine c;
    c.debut = index;
    c.fin = res;
    return c;
}

/* Question 8 */
uint64_t index_aleatoire(Config config) {
    random_device rd;
    mt19937_64 eng(rd());
    uniform_int_distribution<uint64_t> distr;
    return (distr(eng)) % config.n;
}

/* Question 8 (compare 2 chaines) */
bool compare(Chaine bigger, Chaine smaller) {
    return bigger.fin < smaller.fin;
}

/* Question 8 */
void creer_table(Config config, int largeur, int hauteur) {
    for (int i{0}; i < hauteur; i++) {
        TABLE.push_back(nouvelle_chaine(config, index_aleatoire(config), largeur));
    }
    sort(TABLE.begin(), TABLE.end(), compare);
}

/* Question 9 */
vector<string> split(string s, string delimiter) {
    size_t pos;
    string token;
    vector<string> res;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}

/* Question 9 */
void sauve_table(Config config, int largeur, int hauteur, string file_name) {
    string const nomFichier(file_name);
    ofstream monFlux(nomFichier.c_str());
    if (monFlux) {
        //entête
        monFlux << config.h << endl;
        monFlux << config.alphabet << endl;
        monFlux << config.taille_min << endl;
        monFlux << config.taille_max << endl;
        monFlux << largeur << endl;
        monFlux << hauteur << endl;
        //chaines
        for (int i{0}; i < hauteur; i++) {
            monFlux << TABLE[i].debut << "--->" << TABLE[i].fin << endl;
        }
    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }
}

/* Question 9 */
void ouvre_table(Config config, string file_name) {
    ifstream fichier(file_name);
    string ligne;

    //entête
    getline(fichier, ligne);
    config.h = ligne;
    getline(fichier, ligne);
    config.alphabet = ligne;
    getline(fichier, ligne);
    config.taille_min = atoi(ligne.c_str());
    getline(fichier, ligne);
    config.taille_max = atoi(ligne.c_str());
    getline(fichier, ligne);
    LARGEUR = atoi(ligne.c_str());
    getline(fichier, ligne);
    HAUTEUR = atoi(ligne.c_str());

    //données
    Chaine chaine;
    vector<string> res;
    for (int i{0}; i < HAUTEUR; i++) {
        getline(fichier, ligne);
        res = split(ligne, "--->");
        istringstream iss((res[0]));
        iss >> chaine.debut;
        istringstream iss2(res[1]);
        iss2 >> chaine.fin;
        TABLE.push_back(chaine);
    }
}

/* Question 9 */
void affiche_config(Config config) {
    cout<<"Fonction de hash = "<< config.h  <<endl;
    cout<<"Alphabet = "<< config.alphabet  <<endl;
    cout<<"Taille min = "<< config.taille_min  <<endl;
    cout<<"Taille max = "<< config.taille_max  <<endl;
    cout<<"N = "<< config.n  <<endl;

}

/* Question 9 */
void affiche_table(Config config){
    affiche_config(config);
    cout << "height : " << HAUTEUR << " (M)" << endl;
    cout << "width : "  << LARGEUR << " (T)" << endl;
    cout << "\ncontent :" <<endl;
    for (int i{0}; i < 10; i++) {
        cout << TABLE[i].debut << " --> " << TABLE[i].fin << endl;
    }
    cout << "..." <<endl;
    for (int i = HAUTEUR-10; i < HAUTEUR; i++) {
        cout << TABLE[i].debut << " --> " << TABLE[i].fin << endl;
    }
}

/* Question 10 */
//int inverse(int hauteur, int largeur, int h, char *clair) {
//    int nb_candidats = 0;
//    for (int t = largeur - 1; t > 0; t--) {
//        int idx = h2i(h, t);
//        for (int i = t + 1; i < largeur; i++) {
//            idx = i2i(idx, i);
//        }
//        if (recherche(TABLE, hauteur, idx, &a, &b) > 0) {
//
//            for (int i = a; i <= b; i++) {
//                if (verifie_candidat(h, t, TABLE[i][0], clair) == 1) {
//                    return 1;
//                } else {
//                    nb_candidats++;
//                }
//            }
//        }
//    }
//}

/**Fonctions de tests et main****************************************************************************************/
void question1MD5(char* word) {
    byte res[24];
    string str;
    hash_MD5(word, res);
    str = hexStr(res, 16);
    cout << str << endl;
}

void question1SHA1(char* word) {
    byte res[24];
    string str;
    hash_SHA1(word, res);
    str = hexStr(res, 20);
    cout << str << endl;
}

void question2(Config config) {
    cout<<"Fonction de hash = "<< config.h  <<endl;
    cout<<"Alphabet = "<< config.alphabet  <<endl;
    cout<<"Taille min = "<< config.taille_min  <<endl;
    cout<<"Taille max = "<< config.taille_max  <<endl;
    cout<<"N = "<< config.n  <<endl;

}

void question3(Config config, uint64_t position) {
    string s  = i2c(config, position);
    question2(config);
    cout<< "i2c(" << position <<") = "<< s  <<endl;

}

void question5(Config config, char *text, int t) {
    byte res[30];
    hash_MD5(text, res);
    uint64_t result = h2i(config, res, t);

    question2(config);
    cout<< "h2i(MD5(\"" << text <<"\") , "<<  t << ") = " << result << endl;
}

void question6(Config config, long index, long largeur) {
    Chaine c = nouvelle_chaine(config, index, largeur);
    question2(config);
    cout << "\nchaine de taille : " << c.debut << " ... " << c.fin << endl;
}

void question8(Config config, int largeur, int hauteur) {
    creer_table(config, largeur, hauteur);
    question2(config);
    cout << "height : " << hauteur << " (M)" << endl;
    cout << "width : "  << largeur << " (T)" << endl;
    cout << "\ncontent :" <<endl;
    for (int i{0}; i < 10; i++) {
        cout << TABLE[i].debut << " --> " << TABLE[i].fin << endl;
    }
    cout << "..." <<endl;
    for (int i = hauteur-10; i < hauteur; i++) {
        cout << TABLE[i].debut << " --> " << TABLE[i].fin << endl;
    }
}

void question9_sauve_table(Config config, int largeur, int hauteur, string fileName) {
    creer_table(config, largeur, hauteur);
    sauve_table(config, largeur, hauteur, fileName);
}

void question9_ouvre_affiche_table(Config config, string fileName) {
    ouvre_table(config, fileName);
    affiche_table(config);
}

void affiche_help() {
    cout<<"Usage: ./main [OPTIONS] [ARGS] \n"<<endl;
    cout<<"Options possibles : "<<endl;
    cout<<"  -m  <CHAINE>                           applique MD5 sur CHAINE (Q1)"<<endl;
    cout<<"  -s  <CHAINE>                           applique SHA1 sur CHAINE (Q1)"<<endl;
    cout<<"  -c                                     affiche la config courante (Q2)"<<endl;
    cout<<"  -i  <INTEGER>                          calcule i2c(ENTIER) (Q3)"<<endl;
    cout<<"  -h  <CHAINE>-<T>                       calcule h2i(H(CHAINE), T) (Q5)"<<endl;
    cout<<"  -I  <IDX1>-<LARGEUR>                   calcule i2i(IDX1, LARGEUR) (Q6)"<<endl;
    cout<<"  -C  <LARGEUR>-<HAUTEUR>                crée une table LARGEUR*HAUTEUR (Q8)"<<endl;
    cout<<"  -F  <LARGEUR>-<HAUTEUR>-<FILENAME>     crée une table LARGEUR*HAUTEUR dans FILENAME (Q9)"<<endl;
    cout<<"  -A  <FILENAME>                         affiche infos de la table FILENAME (Q9)"<<endl;
    cout<<"  -H                                     affiche ce message"<<endl;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        affiche_help();
    }

    //Initialisation de la config
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    int taille_min = 5;
    int taille_max = 5;
    Config config = init_config(taille_min, taille_max, alphabet);

    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "m:s:ci:h:I:C:F:A:H")) != -1)
        switch (c)
        {
            case 'm':
                question1MD5(optarg);
                break;
            case 's':
                question1SHA1(optarg);
                break;
            case 'c':
                question2(config);
                break;
            case 'i': {
                uint64_t n = strtoull(optarg, NULL, 0);
                question3(config, n);
                break;
            }
            case 'h': {
                string s = optarg;
                vector<string> vs = split(s, "-");

                char *cstr = new char[vs[0].length() + 1];
                strcpy(cstr, vs[0].c_str());
                question5(config, cstr , stoi(vs[1]));
                break;
            }
            case 'I': {
                string s = optarg;
                vector<string> vs = split(s, "-");
                question6(config, stoi(vs[0]), stoi(vs[1]));
                break;
            }
            case 'C': {
                string s = optarg;
                vector<string> vs = split(s, "-");
                question8(config, stoi(vs[0]), stoi(vs[1]));
                break;
            }
            case 'F': {
                string s = optarg;
                vector<string> vs = split(s, "-");

                char *cstr = new char[vs[2].length() + 1];
                strcpy(cstr, vs[2].c_str());
                question9_sauve_table(config, stoi(vs[0]), stoi(vs[1]), cstr);
                break;
            }
            case 'A':
                question9_ouvre_affiche_table(config, optarg);
                break;
            case 'H':
                affiche_help();
                break;
            default:
                abort ();
        }
    return 0;
}


