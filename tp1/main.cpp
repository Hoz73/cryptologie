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


typedef unsigned char byte;

using namespace std;


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


Config init_config(Config config) {
    config.taille_alphabet = config.alphabet.size();

    for (int i = config.taille_min; i <= config.taille_max; i++) {
        config.n += pow(config.taille_alphabet, i);
        config.tab_text_clair.push_back(pow(config.taille_alphabet, i));
    }
    return config;
}


string i2c(Config config, long number) {
    long num = number;
    long new_n = num % config.taille_alphabet;
    int i = 0;
    for (int j = config.taille_min; j <= config.taille_max; j++) {
        if (config.tab_text_clair[i] < number) {
            num = num - config.tab_text_clair[i];
            i++;
        }
    }
    string res;
    for (int i = 0; i < config.taille_max; i++) {
        res += (config.alphabet[new_n]);
        num = (num / config.taille_alphabet);
        new_n = (num % config.taille_alphabet);
    }
    reverse(res.begin(),res.end());
    cout << res << endl;
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

uint64_t h2i(Config config, byte* hash, int colonne){
    uint64_t* y = (uint64_t*)hash;
    return (*y + colonne) % config.n;
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

void question3(Config config, int position) {
    i2c(config, position);
}

void question5(Config config, char *text, int t){
    byte res[30];
    hash_MD5(text,res);
    uint64_t result = h2i(config, res, t);
    cout <<result <<endl;
}

int main() {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Question 1 *************************************
    //question1();

// Question 2 *************************************
//    Config config = question2(4, 4, alphabet);
//    Config config2 = question2(4, 5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//    Config config3 = question2(1, 3, alphabet);
    Config config4 = question2(4, 5, "abcdefghijklmnopqrstuvwxyz");

// Question 3 *************************************
//    question3(config, 1234);
//    question3(config2, 142678997);
//    question3(config3, 12345);

// Question 4 **** un jour mais pas today *********
// ui
// Question 5 *************************************
    char* text = (char*)"oups";
    question5(config4,text,1);

// Question 6 *************************************
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