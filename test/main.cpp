#include <iostream>

using namespace std;

string code_cesar(string texte, int decalage)
{
    string alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), crypte;

    for (string::iterator it=texte.begin(); it!=texte.end(); ++it)
    {
        if (isspace(*it))
        {
            crypte += " ";
        }
        else if (isupper(toupper(*it)))
        {
            string::iterator it2=find(alphabet.begin(), alphabet.end(), *it);
            auto res = (alphabet.find(*it2) + decalage ) % 26;
            crypte += alphabet[res];
        }
    }
    return crypte;
}

string reverse_code_cesar(string texte, int decalage){

    string alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), crypte;

    for (string::iterator it=texte.begin(); it!=texte.end(); ++it)
    {
        if (isspace(*it))
        {
            crypte += " ";
        }
        else if (isupper(toupper(*it)))
        {
            string::iterator it2=find(alphabet.begin(), alphabet.end(), *it);
            auto res = alphabet.find(*it2);
            auto dec = decalage;
            while(dec !=0){
                if(res == 0) {
                    res = 26;
                }
                res --;
                dec--;
            }
            crypte += alphabet[res];
        }
    }
    return crypte ;

}

int ex1(string text){
    int decalage(0);
    for(int i = 0; i<26;i++){
        cout<<reverse_code_cesar(text,i)<<endl;
        cout<<"suivant :0, bonne reponse :1"<<endl;
        cin >> decalage;
        if(decalage == 1){
            return i;
        }
    }
}
string exo2question1(string texte, int key){
    string alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), crypte;
    for (string::iterator it=texte.begin(); it!=texte.end(); ++it){
        auto res = alphabet.find(*it);
        auto dec = key;
        while(dec !=0){
            if(res == 0) {
                res = 26;
            }
            res --;
            dec--;
        }
        crypte += alphabet[res];
        key = alphabet.find(alphabet[res]);
    }
    return crypte;
}

int ex2(){
    int decalage(0);
    for(int i = 0; i<26;i++){
        cout<<exo2question1("PACUB",i)<<endl;
        cout<<"suivant :0, bonne reponse :1"<<endl;
        cin >> decalage;
        if(decalage == 1){
            return i;
        }
    }
}

int main() {
    //cout<<"HELLO WORLD" << endl;
    auto decalage = 6;
    //string res = code_cesar("HELLO WORLD", decalage);
    //cout<<res << endl;
    //cout<<reverse_code_cesar(res, decalage) << endl;
    //cout<<ex1("XFQZY")<<endl;
    cout<<ex2()<<endl;
    return 0;
}

