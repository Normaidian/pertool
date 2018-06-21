#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

class Group{
public:

    string offset;
    string range;
    string access;

    unsigned long hexToDec(string hexAdd){                                                            //! Convert hex number to decimal number
        unsigned long decAdd;
        std::stringstream ss;

        ss << std::hex << hexAdd;
        ss >> decAdd;

        return decAdd;
    }

    string decToHex(unsigned long decAdd){                                                            //! Convert decimal number to hex number
        string hexAdd;
        std::stringstream ss;

        ss << std::hex << decAdd;
        ss >> hexAdd;

        std::transform(hexAdd.begin(),hexAdd.end(),hexAdd.begin(), ::toupper);
        return hexAdd;
    }

    Group searching(string line);       //! Searching group in line from file
};
