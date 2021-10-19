#include <iostream>

using namespace std;
void question3(string text,unsigned int size){
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int arr[26] ={0};
    for(int i = 0; i<size;i++){
        arr[alphabet.find((text[i]))] += 1;
    }
    for(int i = 0; i< size;i++){
        if(arr[i]>1){
            cout<< "prob de tomber sur 2 " << alphabet[i] << " est "
            << arr[i] -1 << "/" << size <<endl;
        }
    }
}

int main() {
    string text = "DEZAAZDS";
    question3(text, text.size());
    return 0;
}


