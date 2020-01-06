#include "utils.h"
#include "definitions.h"

vector<string> str_split(string s) {
    istringstream iss(s);
    return vector<string>(std::istream_iterator < std::string > {iss}, std::istream_iterator<std::string>());
}