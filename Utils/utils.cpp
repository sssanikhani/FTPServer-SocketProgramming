#include "iostream"
#include <string>
#include <vector>
#include <sstream>
#include "utils.hpp"
using namespace std;

vector<string> split(string s) {
    istringstream ss(s);
    string word;

    vector<string> res;
    while (ss >> word)
        res.push_back(word);
    
    return res;
        
}

int main(int argc, char *argv[]) {
    vector<string> s = split("salam      khoobi chekhabar");
    vector<string> s2 = split("mamnoon to khoobi");
    vector<string> s3 = split("to chetori to ham khoobi?");
    for (int i = 0; i < s.size(); i++) {
        cout << s[i] << endl;
    }
    for (int i = 0; i < s2.size(); i++) {
        cout << s2[i] << endl;
    }
    for (int i = 0; i < s3.size(); i++) {
        cout << s3[i] << endl;
    }
}