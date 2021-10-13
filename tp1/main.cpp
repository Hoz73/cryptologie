#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <stack>
typedef unsigned char byte;

using namespace std;


void hash_MD5(const char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void hash_SHA1(const char* s, byte* empreinte)
{
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

string hexStr(byte *data, int len)
{
    stringstream ss;
    ss << hex;

    for( int i(0) ; i < len; ++i )
        ss << setw(2) << setfill('0') << (int)data[i];
    return ss.str();
}

struct Config{
    string alphabet;
    int taille_min;
    int taille_max;
    long n = 0;      // le nombre total de textes clairs valides
    string h;  //type de hashage
    int taille_alphabet;
    vector<int> tab_text_clair;

    Config(){}
};

void init_config(Config config){
    config.taille_alphabet = config.alphabet.size();

    for(int i = config.taille_min; i<= config.taille_max; i++){
        config.n += pow( config.taille_alphabet,i);
        config.tab_text_clair.push_back(pow( config.taille_alphabet,i));
    }
    cout<<config.n<<endl;
}

void i2c_min_equal_max(Config config,long pos){
    string msg;
    stack<string> q;
    msg = config.alphabet[pos %26];
    q.push(msg);
    long new_n = pos/26;
    for(int i =0; i<config.taille_max; i++){
        msg = config.alphabet[new_n % 26];
        q.push(msg);
        new_n = (new_n/26);
    }
    while (!q.empty()) {
        cout << q.top();
        q.pop();
    }
    cout << endl;
}

void i2c(Config config){

}

int main() {
    // Question 1 *************************************
//    cout << "Hello, World!" << endl;
//    byte res[24];
//    hash_SHA1("Bob",res);
//    string str;
//    str = hexStr(res,20);

//    cout<< str <<endl;
//    hash_MD5("Salut",res);
//    str = hexStr(res,16);
//    cout<< str <<endl;

// Question 2 *************************************
    Config config = Config();
    config.alphabet ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    config.taille_min = 4;
    config.taille_max = 4;
    init_config(config);
    i2c_min_equal_max(config,1234);
    return 0;


    //((int64*)e)[0] question 4
    //e: byte *
    //e : [00,01,02,03,...]
    //Jusqu'a question 6-7 pour la prochaine fois
}