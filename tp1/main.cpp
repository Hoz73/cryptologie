#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdint>
#include "classes.cpp"
#include <random>
#include <limits>
#include <fstream>
#include <tuple>

typedef unsigned char byte;

using namespace std;

vector<uint64_t> EMPREINTE;
vector<char *> TEXTE;

void question9();

void sauve_table();

void ouvre_table();

void hash_MD5(const char *s, byte *empreinte) {
    MD5((unsigned char *) s, strlen(s), empreinte);
}

void hash_SHA1(const char *s, byte *empreinte) {
    SHA1((unsigned char *) s, strlen(s), empreinte);
}

string hexStr(byte *data, int len) {
    stringstream ss;
    ss << hex;

    for (int i(0); i < len; ++i)
        ss << setw(2) << setfill('0') << (int) data[i];
    return ss.str();
}

uint64_t pow2(int x, int y) {
    uint64_t res = 1;
    for (int i{1}; i <= y; i++) {
        res *= x;
    }
    return res;
}

Config init_config(Config config) {
    config.taille_alphabet = config.alphabet.size();

    for (int i = config.taille_min; i <= config.taille_max; i++) {
        config.n += pow2(config.taille_alphabet, i);
        config.tab_text_clair.push_back(pow2(config.taille_alphabet, i));
    }
    return config;
}


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
    //cout << "mon i2c : "<<res << endl;
    return res;
}

// Todo not used
string i2c_min_equal_max(Config config, long pos) {
    string msg;
    stack<string> q;
    long new_pos = pos;
    msg = config.alphabet[new_pos % config.taille_alphabet];
    q.push(msg);
    long new_n = new_pos / config.taille_alphabet;
    for (int i = 1; i < config.taille_max; i++) {
        msg = config.alphabet[new_n % config.taille_alphabet];
        q.push(msg);
        new_n = (new_n / config.taille_alphabet);
    }
    string res;
    cout << "res avant" << msg << endl;
    while (!q.empty()) {
        res += q.top();
        q.pop();
    }
    cout << res << endl;
    return res;
}

// Todo not used
void i2c2(Config config, long number) {
    int i = 1;
    bool end = false;
    long num = config.taille_alphabet;
    string result;
    while (!end) {
        if (number >= num) {
            number -= num;
            i++;
            cout << "num : " << num << ", number : " << number << endl;
            num = pow(config.taille_alphabet, i);
        } else {
            if (i < config.taille_min) {
                while (i < config.taille_min) {
                    cout << "pasage 1" << endl;
                    result += "A";
                    i++;
                    config.taille_max -= 1;
                }
                result += i2c_min_equal_max(config, number);
                cout << "res : " << result << endl;
                end = true;
            } else {
                cout << "pasage 2" << endl;
                cout << "num : " << num << ", number : " << number << endl;
                result = i2c_min_equal_max(config, number);
                end = true;
            }
        }
    }
    cout << result << endl;
}

uint64_t h2i(Config config, byte *hash, int colonne) {
    uint64_t *y = (uint64_t *) hash;
    return (*y + colonne) % config.n;
}

uint64_t i2i(Config config, long number, int t) {
    string res = i2c(config, number);
    char *c = (char *) res.c_str();
    TEXTE.push_back(c);
    byte tab[20];
    hash_MD5(c, tab);
    string hash = hexStr(tab, 17);
    //cout<<"mon hash : "<<hash<<endl;
    uint64_t result = h2i(config, tab, t);
    EMPREINTE.push_back(result);
    //cout <<"mon h2i : "<<result <<endl;
    return result;
}

Chaine nouvelle_chaine(Config config, uint64_t index, long largeur) {
    uint64_t res = index;
    for (int i{1}; i < largeur; i++) {
        res = i2i(config, res, i);
        //cout<<"**************************************"<< i<<"******************"<<endl;
    }
    Chaine c;
    c.debut = index;
    c.fin = res;
    return c;
}

uint64_t index_aleatoire(Config config) {
    random_device rd;
    mt19937_64 eng(rd());
    uniform_int_distribution<uint64_t> distr;
    return (distr(eng)) % config.n;
}

bool compare(Chaine bigger, Chaine smaller) {
    return bigger.fin < smaller.fin;
}

vector<Chaine> creer_table(Config config, int largeur, int hauteur) {
    vector<Chaine> table;
    for (int i{0}; i < hauteur; i++) {
        table.push_back(nouvelle_chaine(config, index_aleatoire(config), largeur));
    }
    return table;
}

void sauve_table(vector<Chaine> table, Config config, int largeur, int hauteur, string file_name) {
    string const nomFichier(file_name);
    ofstream monFlux(nomFichier.c_str());
    if (monFlux) {
        monFlux << "fonction de hachage : " << endl;
        monFlux << config.focntion_de_hachage << endl;
        monFlux << "alphabet , taille min , taille max : " << endl;
        monFlux << config.alphabet << "," << config.taille_min << "," << config.taille_max << endl;
        monFlux << "largeur , hauteur : " << endl;
        monFlux << largeur << "," << hauteur << endl;
        monFlux << "debut ---> fin : " << endl;
        for (int i{0}; i < hauteur; i++) {
            monFlux << table[i].debut << "--->" << table[i].fin << endl;
        }
    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
    }
}

vector<string> split(string s, string delimiter) {
    size_t pos = 0;
    string token;
    vector<string> res;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        //cout << token << endl;
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}

tuple<vector<Chaine>, tuple<int, int>, Config> ouvre_table(Config config, string file_name) {
    ifstream fichier(file_name);

    string ligne;
    vector<string> res;

    getline(fichier, ligne);
    getline(fichier, ligne);
    config.focntion_de_hachage = ligne;
    getline(fichier, ligne);
    getline(fichier, ligne);

    res = split(ligne, ",");
    config.alphabet = res[0];
    config.taille_min = atoi(res[1].c_str());
    config.taille_max = atoi(res[2].c_str());


    getline(fichier, ligne);
    getline(fichier, ligne);
    res = split(ligne, ",");
    int largeur = atoi(res[0].c_str());
    int hauteur = atoi(res[1].c_str());

    getline(fichier, ligne);
    vector<Chaine> table;
    for (int i{0}; i < hauteur; i++) {
        getline(fichier, ligne);
        res = split(ligne, "--->");
        Chaine chaine;
        istringstream iss(res[0]);
        iss >> chaine.debut;
        istringstream iss2(res[1]);
        iss2 >> chaine.fin;
        table.push_back(chaine);
    }
    tuple<int, int> largeur_hauteur(largeur, hauteur);
    tuple<vector<Chaine>, tuple<int, int>, Config> result(table, largeur_hauteur, config);

    return result;
}
//********************************************************************************************

void question1() {
    cout << "Hello, World!" << endl;
    byte res[24];
    hash_SHA1("Bob", res);
    string str;
    str = hexStr(res, 20);

    cout << str << endl;
    hash_MD5("Salut", res);
    str = hexStr(res, 16);
    cout << str << endl;
}

Config question2(int taille_min, int taille_max, string alphabet) {
    Config config = Config(taille_min, taille_max, alphabet);
    return init_config(config);
}

void question3(Config config, uint64_t position) {
    i2c(config, position);
}

void question5(Config config, char *text, int t) {
    byte res[30];
    hash_MD5(text, res);
    uint64_t result = h2i(config, res, t);
    cout << result << endl;
}

void question6(Config config, long index, long largeur) {
    Chaine c = nouvelle_chaine(config, index, largeur);
    cout << "debut : " << c.debut << " fin : " << c.fin << endl;
}

vector<Chaine> question8(Config config, int largeur, int hauteur) {
    vector<Chaine> table = creer_table(config, largeur, hauteur);
    sort(table.begin(), table.end(), compare);
    return table;

}

void question9(vector<Chaine> table, Config config, int largeur, int hauteur) {
    string file_name = "fichier_table.txt";
    sauve_table(table, config, largeur, hauteur, file_name);
    tuple<vector<Chaine>, tuple<int, int>, Config> res = ouvre_table(config, file_name);
    cout<<"Voici la table récupérer depuis le fichier ( "<<file_name << " ): " <<endl;
    for (int i{0}; i < get<1>(get<1>(res)); i++) {
        cout << "debut : " << get<0>(res)[i].debut << ", fin : " << get<0>(res)[i].fin << endl;
    }
    cout<<"*********************************"<<endl;
    cout<<"alphabet : "<<get<2>(res).alphabet<<endl;
    cout<<"taille min : "<<get<2>(res).taille_max<<endl;
    cout<<"taille max : "<<get<2>(res).taille_max<<endl;
    cout<<"hauteur : "<<get<1>(get<1>(res))<<endl;
    cout<<"largeur : "<<get<0>(get<1>(res))<<endl;

}


int main() {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Question 1 *************************************
    //question1();

// Question 2 *************************************
    Config config = question2(4, 4, alphabet);
    Config config2 = question2(4, 5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    Config config3 = question2(1, 3, alphabet);
    Config config4 = question2(4, 5, "abcdefghijklmnopqrstuvwxyz");


// Question 3 *************************************
//    question3(config, 1234);
//    question3(config2, 382537153);
//    question3(config2, 373306112);
//    question3(config3, 12345);

// Question 4 **** un jour mais pas today *********
// ui
// Question 5 *************************************
//    char* text = (char*)"oups";
//    question5(config4,text,1);

// Question 6 *************************************
//    Config config5 = question2(4, 5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//    question6(config5,1,100);

// Question 7 *********** un jour mais pas today **
// Question 8 *************************************
    Config config6 = question2(5, 5, "abcdefghijklmnopqrstuvwxyz");
    config6.focntion_de_hachage = "MD5";
    int largeur = 200;
    int hauteur = 100;
    vector<Chaine> table = question8(config6, largeur, hauteur);

//    Question 9
    question9(table, config6, largeur, hauteur);
//  .
//  .
//  .
//  .
//  .
    //((int64*)e)[0] question 4
    //e: byte *
    //e : [00,01,02,03,...]
    //Jusqu'a question 6-7 pour la prochaine fois
    return 0;
}


