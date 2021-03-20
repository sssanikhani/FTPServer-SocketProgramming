#include <string>
#include <vector>
#include <sstream>
#include "utils.hpp"
using namespace std;

vector<string> split(string s)
{
    istringstream ss(s);
    string word;

    vector<string> res;
    while (ss >> word)
        res.push_back(word);

    return res;
}
