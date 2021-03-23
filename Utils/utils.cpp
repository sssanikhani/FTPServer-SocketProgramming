#include <string>
#include <vector>
#include <sstream>
#include <fstream>
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

string join(const vector<string> &v, char c)
{
    string s = "";
    for (vector<string>::const_iterator p = v.begin(); p != v.end(); ++p)
    {
        s += *p;
        if (p != v.end() - 1)
            s += c;
    }
    return s;
}

string file_to_string(const string &path)
{
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open())
        return "ERROR: can not read file";
    ss << input_file.rdbuf();
    return ss.str();
}